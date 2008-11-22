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
#include <KDebug>
#include <QString>
#include <QByteArray>

extern "C" {
#include <julius/juliuslib.h>
#include <signal.h>
}

static int rewind_msec = 0;
static int socketDescriptor = 0;
static int unknown_command_counter = 0;	///< Counter to detect broken connection
static int speechlen=0;
static long sfreq = 16000;
static bool stop_at_next=false;


static int adin_callback_adinnet(SP16 *now, int len, Recog *recog)
{
	int count;
	int start, w;

	start = 0;

	if (speechlen == 0) {
		/* this is first up-trigger */
		if (rewind_msec > 0 && !recog->adin->is_valid_data) {
			/* not spoken currently but has data to process at first trigger */
			/* it means that there are old spoken segments */
			/* disgard them */
			printf("disgard already recorded %d samples\n", len);
			return 0;
		}
		/* erase "<<<please speak>>>" text on tty */
// 		fprintf(stderr, "\r                    \r");
		if (rewind_msec > 0) {
			/* when -rewind value set larger than 0, the speech data spoken
				while pause will be considered back to the specified msec.
				*/
			printf("buffered samples=%d\n", len);
			w = rewind_msec * sfreq / 1000;
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
	count = wt(socketDescriptor, (char *)&(now[start]), len * sizeof(SP16));
	if (count < 0) {
		perror("adintool: cannot write");
		fprintf(stderr, "failed to send data to server\n");
	}
	#ifdef WORDS_BIGENDIAN
		swap_sample_bytes(&(now[start]), len);
	#endif
	/* accumulate sample num of this segment */
	speechlen += len;
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
	char p;

	if (wt(socketDescriptor, &p,  0) < 0) {
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
//	fprintf(stderr, "O");
	fd_set rfds;
	struct timeval tv;
	int status;
	int cnt, ret;
	char com;
	
	/* check if some commands are waiting in queue */
	FD_ZERO(&rfds);
	FD_SET(socketDescriptor, &rfds);
	tv.tv_sec = 0;
	tv.tv_usec = 1;
	status = select(socketDescriptor+1, &rfds, NULL, NULL, &tv);
	if (status < 0) {           /* error */
		fprintf(stderr, "adintool: cannot check command from adinnet server\n");
		return -2;                        /* error return */
	}
	if (status > 0) {           /* there are some data */
		if (FD_ISSET(socketDescriptor, &rfds)) {
			ret = rd(socketDescriptor, &com, &cnt, 1); /* read in command */
			switch (com) {
				case '0':                       /* pause */
					fprintf(stderr, "<PAUSE>\n");
					stop_at_next = TRUE;	/* mark to pause at the end of this input */
					/* tell caller to stop recording */
					return -1;
				case '1':                       /* resume */
					fprintf(stderr, "<RESUME - already running, ignored>\n");
					/* we are already running, so just continue */
					break;
				case '2':                       /* terminate */
					fprintf(stderr, "<TERMINATE>\n");
					stop_at_next = TRUE;	/* mark to pause at the end of this input */
					/* tell caller to stop recording immediately */
					return -2;
					break;
				default:
					fprintf(stderr, "adintool: unknown command adinnet_check_command: %d\n", com);
					return -1;
					unknown_command_counter++;
					/* avoid infinite loop in that case... */
					/* this may happen when connection is terminated from server side  */
					if (unknown_command_counter > 100) {
						close(socketDescriptor);
						fprintf(stderr, "killed by a flood of unknown commands from server\n");
						exit(1);
					}
			}
		}
	}
	return 0;	/* continue ad-in */
}


/** 
 * <JA>
 * サーバから再開コマンドを受信するまで待つ．再開コマンドを受信したら
 * 終了する．
 * 
 * @return エラー時 -1, 通常終了は 0 を返す．
 * </JA>
 * <EN>
 * Wait for resume command from server.
 * 
 * @return 0 on normal exit, or -1 on error.
 * </EN>
 */
static int adinnet_wait_command()
{
	fd_set rfds;
	int status;
	int cnt, ret;
	char com;

	fprintf(stderr, "<<< waiting RESUME >>>");

	while(true) {
		FD_ZERO(&rfds);
		FD_SET(socketDescriptor, &rfds);
		
		//ich glaub das blockt...
		status = select(socketDescriptor+1, &rfds, NULL, NULL, NULL); /* block when no command */
		if (status < 0) {         /* error */
			fprintf(stderr, "adintool: cannot check command from adinnet server\n");
			return -1;                      /* error return */
		} else {                  /* there are some data */
			if (FD_ISSET(socketDescriptor, &rfds)) {
				ret = rd(socketDescriptor, &com, &cnt, 1);
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
						unknown_command_counter++;
						/* avoid infinite loop in that case... */
						/* this may happen when connection is terminated from server side  */
						if (unknown_command_counter > 100) {
						fprintf(stderr, "killed by a flood of unknown commands from server\n");
						exit(1);
					}
				}
			}
		}
	}
	return 0;
} 



AdinStreamer::AdinStreamer(QObject* parent) : QThread(parent)
{
	shouldBeRunning=true;
}

void AdinStreamer::init(const QHostAddress& address, qint32 port)
{
	this->address = address;
	this->port = port;
}

void AdinStreamer::run()
{
	kWarning() << "starting to stream to " << address << port;
	shouldBeRunning=true;
	stop_at_next=false;
	
	Recog *recog;
	Jconf *jconf;
	int ret;

	/* create instance */
	recog = j_recog_new();
	jconf = j_jconf_new();
	recog->jconf = jconf;

	/* register additional options */
	jconf->input.plugin_source = -1;
	jconf->input.type = INPUT_WAVEFORM;
	jconf->input.speech_input = SP_MIC;

	jconf->amnow->analysis.para.smp_freq = sfreq;
	jconf->amnow->analysis.para.smp_period = freq2period(sfreq);

	/* read arguments and set parameters */
	int argc=1;
	char* argv[] = {"simon"};
	//FIXME: device, samplerate!
	if (j_config_load_args(jconf, argc, argv) == -1) {
		fprintf(stderr, "Error reading arguments\n");
		return;
	}

	apply_para(&(jconf->am_root->analysis.para), &(jconf->am_root->analysis.para_default));

	jconf->input.sfreq = jconf->am_root->analysis.para.smp_freq;
	jconf->input.period = jconf->am_root->analysis.para.smp_period;
	jconf->input.frameshift = jconf->am_root->analysis.para.frameshift;
	jconf->input.framesize = jconf->am_root->analysis.para.framesize;

// 	printf("connecting...");
	QByteArray addressByte = address.toString().toUtf8();
	socketDescriptor = make_connection(addressByte.data(), port);
// 	fprintf(stderr, "connected\n");


	if (j_adin_init(recog) == FALSE) {
		printf("Could not init recog... ERROR");
		return;
	}

	// put_status(recog);

	printf("[start recording]");

	while(shouldBeRunning) {
		/* begin A/D input of a stream */
		ret = j_open_stream(recog, NULL);
		switch(ret) {
			case 0:		/* succeeded */
				break;
			case -1:      	/* error */
				/* go on to next input */
				continue;
			case -2:	/* end of recognition process */
				fprintf(stderr, "failed to begin input stream");
				/* exit recording */
				break;
		}
		
		
		do {
			/* process one segment with segmentation */
			/* for incoming speech input, speech detection and segmentation are
				performed and, adin_callback_* is called for speech output for each segment block.
			*/
			/* adin_go() return when input segmented by long silence, or input stream reached to the end */
			speechlen = 0;
			stop_at_next = FALSE;
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
				stop_at_next is TRUE here  */
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
			
			if (ret == -1) {
				/* error in input device or callback function, so terminate program here */
				return;
			}

			if (speechlen > 0) {
				if (ret >= 0 || stop_at_next) { /* segmented by adin-cut or end of stream or server-side command */
					/* send end-of-segment ack to client */
					adin_send_end_of_segment();
				}
				/* output info */
				printf("sent: %d samples (%.2f sec.)\n", speechlen, (float)speechlen / (float)sfreq);
			}

			/***************************************************/
			/* with adinnet server, if terminated by           */
			/* server-side PAUSE command, wait for RESUME here */
			/***************************************************/
			if ((stop_at_next) && (adinnet_wait_command() < 0)) {
					/* command error: terminate program here */
					return;
			}
// 			kWarning() << "Inner while";
		} while ((shouldBeRunning) && (ret > 0 || ret == -2)); /* to the next segment in this input stream */
// 		kWarning() << "Outer while";
	}

	close(socketDescriptor);
	socketDescriptor=0;
	
	recog->adin->ad_end();
	j_request_terminate(recog);
	
	j_recog_free(recog);
// 	kWarning() << "DONE exec";
}


void AdinStreamer::stop()
{
	shouldBeRunning=false;
	wait(1000);
	while (isRunning())
	{
		kWarning() << "Stream still running";
		terminate();
		wait(1000);
	}
}

AdinStreamer::~AdinStreamer()
{

}
