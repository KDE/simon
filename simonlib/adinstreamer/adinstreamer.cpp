/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Powered By:
 *
 *   Large Vocabulary Continuous Speech Recognition Engine Julius
 *   Copyright (c) 1997-2000 Information-technology Promotion Agency, Japan
 *   Copyright (c) 1991-2008 Kawahara Lab., Kyoto University
 *   Copyright (c) 2000-2005 Shikano Lab., Nara Institute of Science and Technology
 *   Copyright (c) 2005-2008 Julius project team, Nagoya Institute of Technology
 *
 */


#include "adinstreamer.h"

#include <simonsound/soundcontrol.h>

#include <QString>
#include <QByteArray>

#include <KDebug>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KMessageBox>




AdinStreamer* AdinStreamer::instance;
static bool adin_shouldBeRunning = false;
static int adinstreamer_unknown_command_counter = 0;
static int adinstreamer_rewind_msec = 0;
static int adinstreamer_socketDescriptor = 0;
static int adinstreamer_speechlen=0;
static long adinstreamer_sfreq = 16000;
static bool adinstreamer_stop_at_next=false;


static int adin_callback_adinnet(SP16 *now, int len, Recog *recog)
{
	if (!adin_shouldBeRunning) return -1;

	int count;
	int start, w;

	start = 0;

	if (adinstreamer_speechlen == 0) {
		/* this is first up-trigger */
		if (adinstreamer_rewind_msec > 0 && !recog->adin->is_valid_data) {
			/* not spoken currently but has data to process at first trigger */
			/* it means that there are old spoken segments */
			/* disgard them */
			printf("disgard already recorded %d samples\n", len);
			return 0;
		}
		/* erase "<<<please speak>>>" text on tty */
// 		fprintf(stderr, "\r                    \r");
		if (adinstreamer_rewind_msec > 0) {
			/* when -rewind value set larger than 0, the speech data spoken
				while pause will be considered back to the specified msec.
				*/
			printf("buffered samples=%d\n", len);
			w = adinstreamer_rewind_msec * adinstreamer_sfreq / 1000;
			if (len > w) {
				start = len - w;
				len = w;
			} else {
				start = 0;
			}
			printf("will process from %d\n", start);
		}
	}

	#ifdef WORDS_BIGENDIAN
		swap_sample_bytes(&(now[start]), len);
	#endif
	count = wt(adinstreamer_socketDescriptor, (char *)&(now[start]), len * sizeof(SP16));
	if (count < 0) {
		perror("adintool: cannot write");
		fprintf(stderr, "failed to send data to server\n");
	}
	#ifdef WORDS_BIGENDIAN
		swap_sample_bytes(&(now[start]), len);
	#endif
	/* accumulate sample num of this segment */
	adinstreamer_speechlen += len;
	#ifdef HAVE_PTHREAD
	if (recog->adin->enable_thread) {
		/* if input length reaches limit, rehash the ad-in buffer */
		if (recog->adin->speechlen > MAXSPEECHLEN - 16000) {
			recog->adin->rehash = TRUE;
// 			fprintf(stderr, "+");
		}
	}
	#endif
  
	/* display progress in dots */
// 	fprintf(stderr, ".");
	return(0);
}


static void
adin_send_end_of_segment()
{
	if (!adin_shouldBeRunning) return;
	char p;

	if (wt(adinstreamer_socketDescriptor, &p,  0) < 0) {
		perror("adintool: cannot write");
		fprintf(stderr, "failed to send EOS to server\n");
	}
}



/**********************************************************************/
/* reveice resume/pause command from adinnet server */
/* (for SPOUT_ADINNET only) */
/*'1' ... resume  '0' ... pause */

static int adinnet_check_command(Recog *)
{
	if (!adin_shouldBeRunning) return -1;

//	fprintf(stderr, "O");
	fd_set rfds;
	struct timeval tv;
	int status;
	int cnt, ret;
	char com;
	
	/* check if some commands are waiting in queue */
	FD_ZERO(&rfds);
	FD_SET(adinstreamer_socketDescriptor, &rfds);
	tv.tv_sec = 0;
	tv.tv_usec = 1;
	status = select(adinstreamer_socketDescriptor+1, &rfds, NULL, NULL, &tv);
	if (status < 0) {           /* error */
		fprintf(stderr, "adintool: cannot check command from adinnet server\n");
		adin_shouldBeRunning=false;
		return -2;                        /* error return */
	}
	if (status > 0) {           /* there are some data */
		if (FD_ISSET(adinstreamer_socketDescriptor, &rfds)) {
			ret = rd(adinstreamer_socketDescriptor, &com, &cnt, 1); /* read in command */
			switch (com) {
				case '0':                       /* pause */
					fprintf(stderr, "<PAUSE>\n");
					adinstreamer_stop_at_next = TRUE;	/* mark to pause at the end of this input */
					/* tell caller to stop recording */
					return -1;
				case '1':                       /* resume */
					fprintf(stderr, "<RESUME - already running, ignored>\n");
					/* we are already running, so just continue */
					break;
				case '2':                       /* terminate */
					fprintf(stderr, "<TERMINATE>\n");
					adinstreamer_stop_at_next = TRUE;	/* mark to pause at the end of this input */
					/* tell caller to stop recording immediately */
					return -2;
					break;
				default:
					fprintf(stderr, "adintool: unknown command adinnet_check_command: %d\n", com);
					adinstreamer_unknown_command_counter++;
					if (adinstreamer_unknown_command_counter > 10)
					{
						fprintf(stderr, "adintool: I've got enough");
						adin_shouldBeRunning= false;
					}
					return -1;
			}
		}
	}
	return 0;	/* continue ad-in */
}


/** 
 * Wait for resume command from server.
 * 
 * @return 0 on normal exit, or -1 on error.
 */
static int adinnet_wait_command()
{
	if (!adin_shouldBeRunning) return -1;

	fd_set rfds;
	int status;
	int cnt, ret;
	char com;

	fprintf(stderr, "<<< waiting RESUME >>>");

	while(adin_shouldBeRunning) {
		FD_ZERO(&rfds);
		FD_SET(adinstreamer_socketDescriptor, &rfds);
		
		//ich glaub das blockt...
		status = select(adinstreamer_socketDescriptor+1, &rfds, NULL, NULL, NULL); /* block when no command */
		if (status < 0) {         /* error */
			adin_shouldBeRunning= false;
			fprintf(stderr, "adintool: cannot check command from adinnet server\n");
			return -1;                      /* error return */
		} else {                  /* there are some data */
			if (FD_ISSET(adinstreamer_socketDescriptor, &rfds)) {
				ret = rd(adinstreamer_socketDescriptor, &com, &cnt, 1);
				switch (com) {
					case '0':                       /* pause */
						/* already paused, so just wait for next command */
						fprintf(stderr, "<PAUSE - already paused, ignored>\n");
						break;
					case '1':                       /* resume */
						/* do resume */
						printf("Resume...");
						return 1; // resume
						break;
					case '2':                       /* terminate */
						/* already paused, so just wait for next command */
						fprintf(stderr, "<TERMINATE - already paused, ignored>\n");
						break;
					default:
						fprintf(stderr, "adintool: unknown command adinnet_wait_command: %d\n", com);
						return 1;
				}
			}
		}
	}
	return 0;
} 



AdinStreamer::AdinStreamer(QObject* parent) : QThread(parent)
{
	recog = NULL;
	shouldReStart=false;
	adin_shouldBeRunning=false;
	connect(this, SIGNAL(audioDeviceError()), this, SLOT(reportSoundDeviceError()));
}

void AdinStreamer::init(const QHostAddress& address, qint32 port, qint32 sampleRate)
{
	this->address = address;
	this->port = port;
	adinstreamer_sfreq = sampleRate;
}

void AdinStreamer::run()
{
	adin_shouldBeRunning=true;
	adinstreamer_stop_at_next=false;

	Jconf *jconf;
	int ret;

	adinstreamer_unknown_command_counter = 0;

	KSharedConfig::Ptr config = KSharedConfig::openConfig("simonsoundrc");
	KConfigGroup group(config, "Devices");
	QString device = group.readEntry("SoundInputDevice", "0");
	setenv("SIMONDEV", device.toUtf8().data(), 1/*overwrite*/);

	/* create instance */
	recog = j_recog_new();
	jconf = j_jconf_new();
	recog->jconf = jconf;
	

	/* register additional options */
	jconf->input.plugin_source = -1;
	jconf->input.type = INPUT_WAVEFORM;
	jconf->input.speech_input = SP_MIC;

	jconf->amnow->analysis.para.smp_freq = adinstreamer_sfreq;
	jconf->amnow->analysis.para.smp_period = freq2period(adinstreamer_sfreq);

	/* read arguments and set parameters */
	int argc=1;
	char* argv[] = {"simon"};
	if (j_config_load_args(jconf, argc, argv) == -1) {
		fprintf(stderr, "Error reading arguments\n");
		return;
	}

	apply_para(&(jconf->am_root->analysis.para), &(jconf->am_root->analysis.para_default));

	jconf->input.sfreq = jconf->am_root->analysis.para.smp_freq;
	jconf->input.period = jconf->am_root->analysis.para.smp_period;
	jconf->input.frameshift = jconf->am_root->analysis.para.frameshift;
	jconf->input.framesize = jconf->am_root->analysis.para.framesize;

	QByteArray addressByte = address.toString().toUtf8();
	if (adinstreamer_socketDescriptor == 0)
		adinstreamer_socketDescriptor = make_connection(addressByte.data(), port);


	if (j_adin_init(recog) == FALSE) {
		Recog *realRecog = recog;
		recog=NULL;
		j_recog_free(realRecog);

		fprintf(stderr, "Could not init recog... ERROR\n");
		emit audioDeviceError();
		return;
	}

	// put_status(recog);

	printf("[start recording]");
	emit started();

	while(adin_shouldBeRunning) {
		/* begin A/D input of a stream */
		ret = j_open_stream(recog, NULL);
		switch(ret) {
			case 0:		/* succeeded */
				break;
			case -1:      	/* error */
				/* go on to next input */
				continue;
			case -2:	/* end of recognition process */
				adin_shouldBeRunning=false;		//KMessageBox::sorry(0, i18n("Couldn't start input stream."));
				emit audioDeviceError();
				fprintf(stderr, "failed to begin input stream\n");
				/* exit recording */
				break;
		}
		
		if (!adin_shouldBeRunning) 
			break;
		do {
			/* process one segment with segmentation */
			/* for incoming speech input, speech detection and segmentation are
				performed and, adin_callback_* is called for speech output for each segment block.
			*/
			/* adin_go() return when input segmented by long silence, or input stream reached to the end */
			adinstreamer_speechlen = 0;
			adinstreamer_stop_at_next = FALSE;
// 			fprintf(stderr, "<<< please speak >>>");
			ret = adin_go(adin_callback_adinnet, adinnet_check_command, recog);
			/* return value of adin_go:
				-2: input terminated by pause command from adinnet server
				-1: input device read error or callback process error
				0:  paused by input stream (end of file, etc..)
				>0: detected end of speech segment:
				by adin-cut, or by callback process
				(or return value of ad_check (<0) (== not used in this program))
				
			if PAUSE or TERMINATE command has been received while input,
				adinstreamer_stop_at_next is TRUE here  */
			switch(ret) {
				case -2:	     /* terminated by terminate command from server */
					fprintf(stderr, "[terminated by server]\n");
					break;
				case -1:		     /* device read error or callback error */
					fprintf(stderr, "[error]\n");
					break;
				case 0:			/* reached to end of input */
					fprintf(stderr, "[eof]\n");
					break;
				default:	  /* input segmented by silence or callback process */
					fprintf(stderr, "[segmented]\n");
					break;
			}
			
			if ((ret == -1)||/*(ret==-2)||*/(ret==0)) {
				/* error in input device or callback function, so terminate program here */
				adin_shouldBeRunning=false;
				break;
			}

			if (adinstreamer_speechlen > 0) {
				if (ret >= 0 || adinstreamer_stop_at_next) { /* segmented by adin-cut or end of stream or server-side command */
					/* send end-of-segment ack to client */
					adin_send_end_of_segment();
				}
				/* output info */
				printf("sent: %d samples (%.2f sec.)\n", adinstreamer_speechlen, (float)adinstreamer_speechlen / (float)adinstreamer_sfreq);
			}

			/***************************************************/
			/* with adinnet server, if terminated by           */
			/* server-side PAUSE command, wait for RESUME here */
			/***************************************************/
			if ((adinstreamer_stop_at_next) && (adinnet_wait_command() < 0)) {
					/* command error: terminate program here */
					Recog *realRecog = recog;
					recog=NULL;
					j_recog_free(realRecog);
					return;
			}
		} while ((adin_shouldBeRunning) && (ret > 0 || ret == -2)); /* to the next segment in this input stream */
	}

	Recog *realRecog = recog;
	recog=NULL;
	j_recog_free(realRecog);

	fprintf(stderr, "CLOSING SOCKET!\n");
	close(adinstreamer_socketDescriptor);
	fprintf(stderr, "Socket closed\n");
	adinstreamer_socketDescriptor=0;
	
	emit stopped();
}


void AdinStreamer::stop()
{
	adin_shouldBeRunning=false;
	shouldReStart=false;
	adinstreamer_stop_at_next=false;

	if (recog && (recog->adin))
	{
		recog->adin->ad_end();
//		adin_mic_end();
	}
	
	int tries=0; 
	while ((isRunning()) && (tries++ < 20))
	{
		wait(200);
	}
	if (isRunning()) //the recognition did not end - ugh...
	{
		terminate();

		if (recog)
		{
			j_request_terminate(recog);
			j_recog_free(recog);
			recog=NULL;
			emit stopped();
		}
	}
}



void AdinStreamer::reportSoundDeviceError()
{
	shouldReStart=true;
	emit stopped();
	KMessageBox::error(0, i18n("Couldn't open sound device.\n\nPlease check your configuration."));
}

void AdinStreamer::stopSoundStream()
{
	if (isRunning())
	{
		stop();
		shouldReStart=true;
	}
}

void AdinStreamer::restartSoundStream()
{
	if (shouldReStart)
	{
		start();
	}
}

AdinStreamer::~AdinStreamer()
{

}
