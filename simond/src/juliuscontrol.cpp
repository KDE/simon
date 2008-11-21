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

#include "juliuscontrol.h"
#include <QFile>
#include <KLocalizedString>
#include <KStandardDirs>
#include <KConfig>
#include <KConfigGroup>

#ifdef FALSE
#undef FALSE
#endif
#ifdef TRUE
#undef TRUE
#endif

#include <stdio.h>


JuliusControl::JuliusControl(const QString& username, QObject *parent) : RecognitionControl(username, parent)
{
	recog=0;
	jconf=0;
	isLocal=false;
	m_initialized=false;
}

Jconf* JuliusControl::setupJconf()
{
	if (!QFile::exists(KStandardDirs::locateLocal("appdata", "models/"+username+"/active/julius.jconf")))
	{
		if (!QFile::copy(KStandardDirs::locate("appdata", "default.jconf"), 
			KStandardDirs::locateLocal("appdata", "models/"+username+"/active/julius.jconf")))
			return 0;
	}
	
	
	QString dirPath =  KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
	QByteArray jConfPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/julius.jconf").toUtf8();
	QByteArray gram = dirPath.toUtf8()+"model";
	QByteArray hmmDefs = dirPath.toUtf8()+"hmmdefs";
	QByteArray tiedList = dirPath.toUtf8()+"tiedlist";
	
	QString configPath = dirPath+"activerc";
	KConfig config( configPath, KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	QByteArray smpFreq = QString(cGroup.readEntry("SampleRate")).toUtf8();

	int argc=13;
	char* argv[] = {"simond", "-C", jConfPath.data(),
			"-gram", gram.data(),
			 "-h", hmmDefs.data(),
			 "-hlist", tiedList.data(),
			 //"-input", "mic", //only for local input -.- 
			 "-input", "adinnet", //only for local input -.- 
			 "-smpFreq", smpFreq.data()};
			 

	return j_config_load_args_new(argc, argv);
}

void statusRecstart(Recog *recog, void *)
{
  if (recog->jconf->input.speech_input == SP_MIC || recog->jconf->input.speech_input == SP_NETAUDIO) {
    kDebug() << "\nListening...\n";
  }
}

void statusRecready(Recog *recog, void *)
{
  if (recog->jconf->input.speech_input == SP_MIC || recog->jconf->input.speech_input == SP_NETAUDIO) {
    kDebug() << "<<< please speak >>>";
  }
}

QString getHypoPhoneme(WORD_ID *seq, int n, WORD_INFO *winfo)
{
	QString result;
	int i,j;
	WORD_ID w;
	static char buf[MAX_HMMNAME_LEN];

	if (seq != NULL) {
		for (i=0;i<n;i++) {
			if (i > 0) result += " |";
				w = seq[i];
			for (j=0;j<winfo->wlen[w];j++) {
				center_name(winfo->wseq[w][j]->name, buf);
				result += " ";
				result += buf;
			}
		}
	}
	return result;
}

void
put_hypo_phoneme(WORD_ID *seq, int n, WORD_INFO *winfo)
{
  int i,j;
  WORD_ID w;
  static char buf[MAX_HMMNAME_LEN];

  if (seq != NULL) {
    for (i=0;i<n;i++) {
      if (i > 0) printf(" |");
      w = seq[i];
      for (j=0;j<winfo->wlen[w];j++) {
	center_name(winfo->wseq[w][j]->name, buf);
	printf(" %s", buf);
      }
    }
  }
  printf("\n");  
}

void outputResult(Recog *recog, void *control)
{
	int i;
	WORD_INFO *winfo;
	WORD_ID *seq;
	int seqnum;
	int n;
	Sentence *s;
	RecogProcess *r;
	JuliusControl *jControl = (JuliusControl*) control;
	Q_ASSERT(jControl);

	/* all recognition results are stored at each recognition process
	instance */
	for(r=recog->process_list;r;r=r->next) {
		/* skip the process if the process is not alive */
		if (! r->live) continue;

		/* result are in r->result.  See recog.h for details */

		/* check result status */
		if (r->result.status < 0) {      /* no results obtained */
			/* outout message according to the status code */
			switch(r->result.status) {
				case J_RESULT_STATUS_REJECT_POWER:
					printf("<input rejected by power>\n");
					break;
				case J_RESULT_STATUS_TERMINATE:
					printf("<input teminated by request>\n");
					break;
				case J_RESULT_STATUS_ONLY_SILENCE:
					printf("<input rejected by decoder (silence input result)>\n");
					break;
				case J_RESULT_STATUS_REJECT_GMM:
					printf("<input rejected by GMM>\n");
					break;
				case J_RESULT_STATUS_REJECT_SHORT:
					printf("<input rejected by short input>\n");
					break;
				case J_RESULT_STATUS_FAIL:
					printf("<search failed>\n");
					break;
			}
			/* continue to next process instance */
			continue;
		}

		/* output results for all the obtained sentences */
		winfo = r->lm->winfo;

		for(n = 0; n < r->result.sentnum; n++) { /* for all sentences */
			QString result, sampa, sampaRaw;
			s = &(r->result.sent[n]);
			seq = s->word;
			seqnum = s->word_num;

			/* output word sequence like Julius */
			//       printf("sentence%d:", n+1);
			for(i=0;i<seqnum;i++) {
				result += " ";
				result += winfo->woutput[seq[i]];// printf(" %s", );
			}
			//       printf("\n");

			/* LM entry sequence */
			//       printf("wseq%d:", n+1);
			//       for(i=0;i<seqnum;i++) printf(" %s", winfo->wname[seq[i]]);
			//       printf("\n");

			/* phoneme sequence */
			//       printf("phseq%d:", n+1);
			sampaRaw = sampa = getHypoPhoneme(seq, seqnum, winfo);
			//       printf("\n");
			
			/* confidence scores */
			//       printf("cmscore%d:", n+1);
			//       for (i=0;i<seqnum; i++) printf(" %5.3f", s->confidence[i]);
			//       printf("\n");
			//       /* AM and LM scores */
			//       printf("score%d: %f", n+1, s->score);
			//       if (r->lmtype == LM_PROB) { /* if this process uses N-gram */
			// 	printf(" (AM: %f  LM: %f)", s->score_am, s->score_lm);
			//       }
			//       printf("\n");
			//       if (r->lmtype == LM_DFA) { /* if this process uses DFA grammar */
			// 	/* output which grammar the hypothesis belongs to
			// 	   when using multiple grammars */
			// 	if (multigram_get_all_num(r->lm) > 1) {
			// 	  printf("grammar%d: %d\n", n+1, s->gram_id);
			// 	}
			//       }
			jControl->recognized(result.trimmed(), sampa.trimmed(), sampaRaw.trimmed());
		}
	}
}


void pauseWaiter(Recog *, void *control)
{
	JuliusControl *jControl = (JuliusControl*) control;
	Q_ASSERT(jControl);
	jControl->waitForResumed();
}


void juliusCallbackPoll(Recog *recog, void *control)
{
	JuliusControl *juliusControl = (JuliusControl*) control;
	Q_ASSERT(juliusControl);
	
	JuliusControl::Request request = juliusControl->popNextRequest();
	switch (request)
	{
		case JuliusControl::None:
			return; //speed
		
		case JuliusControl::Stop:
			kDebug() << "Stopping...";
			break;
		
		case JuliusControl::Pause:
			kDebug() << "Pausing...";
			j_request_pause(recog);
			break;
	}
}

JuliusControl::Request JuliusControl::popNextRequest()
{
	if (nextRequests.count() == 0)
		return JuliusControl::None;
	
	JuliusControl::Request req = nextRequests.takeAt(0);
	kDebug() << nextRequests.count();
	return req;
}


bool JuliusControl::isInitialized()
{
	return m_initialized;
}

void JuliusControl::uninitialize()
{
	if (!m_initialized) return;

	if (isRunning())
	{
		stop();
		wait(1000);
	}

	if (this->recog)
		j_recog_free(recog);
	else 
		if (this->jconf)
			j_jconf_free(jconf);

	m_initialized=false;
}


bool JuliusControl::initializeRecognition(bool isLocal)
{
	bool wasRunning = isRunning();
	uninitialize();
	
	this->isLocal = isLocal;
	if (isLocal) 
		kDebug() << "Initializing local recognition";
	else {
		kDebug() << "Initializing remote recognition";
		emit recognitionError(i18n("Remote recognition not yet implemented"));
		return false;
	}
	
 	//FILE *fp;
	//QByteArray logPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/julius.log").toUtf8();
	//kDebug () << logPath.data();
 	//fp = fopen(logPath.data(), "w");
 	//if (fp == NULL) 
 //		return false;
 //	jlog_set_output(fp);
	
	Jconf *jconf = setupJconf();
	if (!jconf)
	{
		emit recognitionError(i18n("Internal Jconf error"));
		return false;
	}

	this->jconf = jconf;
	
	this->recog = j_create_instance_from_jconf(jconf);
	if (!recog)
	{
		emit recognitionError(i18n("Could not initialize recognition"));
		j_jconf_free(jconf);
		this->jconf=0;
		this->recog=0;
		return false;
	}
	
	callback_add(recog, CALLBACK_EVENT_SPEECH_READY, statusRecready, this);
	callback_add(recog, CALLBACK_EVENT_SPEECH_START, statusRecstart, this);
	callback_add(recog, CALLBACK_RESULT, outputResult, this);
	callback_add(recog, CALLBACK_POLL, juliusCallbackPoll, this);
	callback_add(recog, CALLBACK_PAUSE_FUNCTION, pauseWaiter, this);

	m_initialized=true;
	emit recognitionReady();
	if (wasRunning) start();
	return true;
}



void JuliusControl::run()
{
	Q_ASSERT(recog);
	
	/**************************/
	/* Initialize audio input */
	/**************************/
	/* initialize audio input device */
	/* ad-in thread starts at this time for microphone */
	if (j_adin_init(recog) == false) {    /* error */
		emit recognitionError(i18n("Couldn't start adin-thread"));
		return;
	}

	/* output system information to log */
	j_recog_info(recog);
	
	 switch(j_open_stream(recog, NULL)) {
		case 0:
			emit recognitionStarted();
			break;
		case -1:
			emit recognitionError(i18n("Unbekannter Fehler"));
			return;
		case -2:
			if (isLocal)
				emit recognitionError(i18n("Konnte Mikrofon nicht initialisieren"));
			else emit recognitionError(i18n("Fehler im Netzwerk-Sprachstream"));
			return;
	}
	
	/**********************/
	/* Recognization Loop */
	/**********************/
	/* enter main loop to recognize the input stream */
	/* finish after whole input has been processed and input reaches end */
	if (j_recognize_stream(recog) == -1)
	{
		emit recognitionError("recognize_stream: -1");
	}
	
}

void JuliusControl::stop()
{
	if (!recog) return;
	
	pauseMutex.unlock();
	if (!isRunning()) return;

	if (recog->adin)
		recog->adin->ad_end();
	j_request_terminate(recog);
	quit();
	if (!wait(1000))
		kWarning() << "ARGH STILL RUNNING!";
	else 
		emit recognitionStopped();
}


void JuliusControl::recognized(const QString& sequence, const QString& sampa, const QString& samparaw)
{
	emit recognitionResult(sequence, sampa, samparaw);
}

void JuliusControl::pause()
{
	kDebug() << "Locking";
	pauseMutex.lock();
	pushRequest(JuliusControl::Pause);
}

void JuliusControl::resume()
{
	kDebug() << "Unlocking";
	pauseMutex.unlock();
	emit recognitionResumed();
}

void JuliusControl::waitForResumed()
{
	emit recognitionPaused();
	pauseMutex.lock();
	j_request_resume(recog);
	pauseMutex.unlock();
}

void JuliusControl::pushRequest(JuliusControl::Request request)
{
	nextRequests << request;
}

JuliusControl::~JuliusControl()
{
	stop();
	
	j_recog_free(recog);
// 	if (recog)
// 	{
// 		j_recog_free(recog);
// 	} else if (jconf)
// 		j_jconf_free(jconf);
}
