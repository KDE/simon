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
#include <KDebug>
#include <KConfigGroup>
#include <locale.h>

#ifdef FALSE
#undef FALSE
#endif
#ifdef TRUE
#undef TRUE
#endif

#include <stdio.h>
#include <KProcess>

JuliusControl::JuliusControl(const QString& username, QObject* parent) : RecognitionControl(username, parent),
recog(0),
jconf(0),
stopping(false),
m_initialized(false),
shouldBeRunning(false),
logFile(0)
{
}


Jconf* JuliusControl::setupJconf()
{
  if (!QFile::exists(KStandardDirs::locateLocal("appdata", "models/"+username+"/active/julius.jconf"))) {
    if (!QFile::copy(KStandardDirs::locate("appdata", "default.jconf"),
      KStandardDirs::locateLocal("appdata", "models/"+username+"/active/julius.jconf")))
      return 0;
  }

  QString dirPath =  KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
  QByteArray jConfPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/julius.jconf").toUtf8();
  QByteArray gram = dirPath.toUtf8()+"model";
  QByteArray tiedList = dirPath.toUtf8()+"tiedlist";

  //////BEGIN: Workaround
  //convert '.' in hmmdefs to its locale specific equivalent
  lconv * localeConv = localeconv();
  char *decimalPoint = localeConv->decimal_point;

  QFile hmm(dirPath+"hmmdefs");
  QFile hmmLoc(dirPath+"hmmdefs_loc");
  if (!hmm.open(QIODevice::ReadOnly) || !hmmLoc.open(QIODevice::WriteOnly))
    return 0;

  while (!hmm.atEnd())
    hmmLoc.write(hmm.readLine(3000).replace('.', decimalPoint));

  hmm.close();
  hmmLoc.close();
  //////END: Workaround

  QByteArray hmmDefs = dirPath.toUtf8()+"hmmdefs_loc";

  //	QString configPath = dirPath+"activerc";
  //	KConfig config( configPath, KConfig::SimpleConfig );
  //	KConfigGroup cGroup(&config, "");
  //	QByteArray smpFreq = QString(cGroup.readEntry("SampleRate")).toUtf8();
  //	this->sampleRate = cGroup.readEntry("SampleRate", 0);

  int argc=11;
  char* argv[] = {
    "simond", "-C", jConfPath.data(),
    "-gram", gram.data(),
    "-h", hmmDefs.data(),
    "-hlist", tiedList.data(),
    "-input", "file"
  };
  //			 "-smpFreq", smpFreq.data()};

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
  WORD_ID w;
  static char buf[MAX_HMMNAME_LEN];

  if (seq != 0) {
    for (int i=0;i<n;i++) {
      if (i > 0) result += " |";
      w = seq[i];
      for (int j=0;j<winfo->wlen[w];j++) {
        center_name(winfo->wseq[w][j]->name, buf);
        result += ' ';
        result += QString::fromUtf8(buf);
      }
    }
  }
  return result;
}


void
put_hypo_phoneme(WORD_ID *seq, int n, WORD_INFO *winfo)
{
  WORD_ID w;
  static char buf[MAX_HMMNAME_LEN];

  if (seq != 0) {
    for (int i=0;i<n;i++) {
      if (i > 0) printf(" |");
      w = seq[i];
      for (int j=0;j<winfo->wlen[w];j++) {
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
    if (r->result.status < 0) {                   /* no results obtained */
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

    QList<RecognitionResult> recognitionResults;

    for(n = 0; n < r->result.sentnum; n++) {      /* for all sentences */
      QString result, sampa, sampaRaw;
      s = &(r->result.sent[n]);
      seq = s->word;
      seqnum = s->word_num;

      /* output word sequence like Julius */
      //       printf("sentence%d:", n+1);
      for(i=0;i<seqnum;i++) {
        result += ' ';
                                                  // printf(" %s", );
        result += QString::fromUtf8(winfo->woutput[seq[i]]);
      }
      result.remove("<s> ");
      result.remove(" </s>");
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
      QList<float> confidenceScores;

      for (i=1;i<seqnum-1; i++) {
        confidenceScores << s->confidence[i];
      }
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
      kDebug() << "Recognized: " << result.trimmed();

      recognitionResults.append(RecognitionResult(result.trimmed(),
        sampa.trimmed(),
        sampaRaw.trimmed(), confidenceScores));
    }
    QString fileName = QString::fromUtf8(adin_file_get_current_filename());
    jControl->recognized(fileName, recognitionResults);
  }
}


/**
void juliusCallbackPoll(Recog *recog, void *control)
{
  Q_UNUSED(recog);

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

}
}
*/

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

  if (isRunning()) {
    stop();
    wait(1000);
  }

  if (this->recog)
    j_recog_free(recog);
  else
  if (this->jconf)
    j_jconf_free(jconf);

  this->recog=0;
  this->jconf=0;

  m_initialized=false;
  closeLog();
}


QByteArray JuliusControl::getBuildLog()
{
  QByteArray logPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/julius.log").toUtf8();
  QFile f(logPath);
  if (!f.open(QIODevice::ReadOnly))
    return QByteArray();

  QByteArray out = "<html><head /><body><p>"+f.readAll().replace('\n', "<br />")+"</p></body></html>";
  f.close();
  return out;
}


void JuliusControl::emitError(const QString& error)
{
  QString specificError = error;
  QByteArray buildLog = getBuildLog();

  //touchLastFailedStart();

  int indexStartVocaError = buildLog.indexOf("Error: voca_load_htkdict");
  if (indexStartVocaError != -1) {
    buildLog = buildLog.replace("<br />", "\n");
    indexStartVocaError = buildLog.indexOf("Error: voca_load_htkdict");
    int indexEndMissingPhones = buildLog.indexOf("end missing phones");

    QList<QByteArray> lines = buildLog.mid(indexStartVocaError, indexEndMissingPhones - indexStartVocaError).split('\n');

    QStringList missingPhones;
    QStringList affectedWords;

    bool thisLineMoreInfoForMissingTriphone = false;
    bool thisLineMoreMissingPhones = false;
    foreach (const QByteArray& lineByte, lines) {
      QString line = QString::fromUtf8(lineByte);
      if (line.contains(QRegExp("line [0-9]+: triphone \".*\" (or biphone \".*\" )?not found$"))) {
        //Error: voca_load_htkdict: line 33: triphone "*-dh+ix" or biphone "dh+ix" not found
        //Error: voca_load_htkdict: line 33: triphone "dh-ix+s" not found
        thisLineMoreInfoForMissingTriphone = true;
      }
      else {
        if (thisLineMoreInfoForMissingTriphone) {
          //Error: voca_load_htkdict: the line content was: 2       [Towel] t aw ax l
          line = line.mid(line.lastIndexOf("       ")).trimmed();
          QString word = line.mid(line.indexOf('[')+1);
          word = word.left(word.indexOf(']'));
          affectedWords << word;

          thisLineMoreInfoForMissingTriphone = false;
        }
        else {
          if (thisLineMoreMissingPhones) {
            if (line.contains("end missing phones"))
              break;

            //Error: voca_load_htkdict: ax-m+b
            missingPhones << line.mid(26).trimmed();
          } else
          {
            if (line.contains("begin missing phones")) {
              thisLineMoreMissingPhones = true;
            }
          }
        }
      }
    }

    QString missingPhonesStr = missingPhones.join(", ");
    QString affectedWordsStr = affectedWords.join(", ");
    if (missingPhonesStr.length() > 200)
      missingPhonesStr = missingPhonesStr.left(200)+"...";
    if (affectedWordsStr.length() > 200)
      affectedWordsStr = affectedWordsStr.left(200)+"...";

    specificError = i18n("The recognition could not be started because your model contains words that consists of sounds that are not covered by your acoustic model.\n\nYou need to either remove those words, transcribe them differently or train them.\n\nWarning: The latter will not work if you are using static base models!\n\nThis could also be a sign of a base model that uses a different phoneme set than your scenario vocabulary.\n\nThe following words are affected (list may not be complete):\n%1\n\nThe following phonemes are affected (list may not be complete):\n%2", affectedWordsStr, missingPhonesStr);
  }

  emit recognitionError(specificError, buildLog);
}


bool JuliusControl::initializeRecognition()
{
  if (isInitialized()) {
    kDebug() << "Initializing recognition that was already initialized; uninitializing...";
    stop();
  }

  emit recognitionReady();
  return true;
}


bool JuliusControl::startRecognition()
{
  uninitialize();

  QByteArray logPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/julius.log").toUtf8();

  logFile = fopen(logPath.data(), "w");
  if (logFile == 0) {
    emitError(i18n("Could not open log file. Please ignore detailed log output!"));
    return false;
  }
  jlog_set_output(logFile);

  Jconf *jconf = setupJconf();
  if (!jconf) {
    closeLog();
    emitError(i18n("Internal Jconf error"));
    return false;
  }

  this->jconf = jconf;

  this->recog = j_create_instance_from_jconf(jconf);
  if (!recog) {
    j_jconf_free(jconf);
    closeLog();
    this->jconf=0;
    this->recog=0;
    emitError(i18n("Could not initialize recognition"));
    return false;
  }

  callback_add(recog, CALLBACK_EVENT_SPEECH_READY, statusRecready, this);
  callback_add(recog, CALLBACK_EVENT_SPEECH_START, statusRecstart, this);
  callback_add(recog, CALLBACK_RESULT, outputResult, this);
  //	callback_add(recog, CALLBACK_POLL, juliusCallbackPoll, this);

  touchLastSuccessfulStart();

  m_initialized=true;

  //==========================0

  /**************************/
  if (j_adin_init(recog) == false) {              /* error */
    emitError(i18n("Could not start adin-thread"));
    return false;
  }

  /* output system information to log */
  j_recog_info(recog);

  emit recognitionStarted();
  return true;
}


void JuliusControl::run()
{
  Q_ASSERT(recog);
  shouldBeRunning=true;

  switch(j_open_stream(recog, currentFileName.toUtf8().data())) {
    case 0:
      kDebug() << "Recognizing...";
      break;
    case -1:
      emitError(i18n("Unknown error"));
      return;
    case -2:
      emitError(i18n("Error with the audio stream"));
      return;
  }

  /* Recognization */
  int ret = j_recognize_stream(recog);
  switch (ret) {
    case 0:
      //client exited
      //shouldBeRunning=false;
      break;
    case -1:
      //emitError("recognize_stream: -1");
      shouldBeRunning=false;
      break;
  }

  #if 0
  while (shouldBeRunning) {
    switch(j_open_stream(recog, 0)) {
      case 0:
        kDebug() << "Recognizing...";
        break;
      case -1:
        emitError(i18n("Unknown error"));
        return;
      case -2:
        emitError(i18n("Error with the audio stream"));
        return;
    }

    touchLastSuccessfulStart();

    /**********************/
    /* Recognization Loop */
    /**********************/
    /* enter main loop to recognize the input stream */
    /* finish after whole input has been processed and input reaches end */
    int ret = j_recognize_stream(recog);
    switch (ret) {
      case 0:
        //client exited
        //shouldBeRunning=false;
        break;
      case -1:
        //emitError("recognize_stream: -1");
        shouldBeRunning=false;
        break;
    }
    usleep(300);
  }
  emit recognitionStopped();
  #endif
}


void JuliusControl::stop()
{
  if (!recog) return;

  shouldBeRunning=false;

  if (!isRunning()) return;

  if (recog) {
    j_request_terminate(recog);
    j_close_stream(recog);
  }

  if (!wait(1000)) {
    kDebug() << "ARGH STILL RUNNING!";

    while (isRunning()) {
      kDebug() << "Terminating";
      terminate();
      wait(500);
    }
  }
}


void JuliusControl::recognized(const QString& fileName, const QList<RecognitionResult>& recognitionResults)
{
  emit recognitionResult(fileName, recognitionResults);
}


void JuliusControl::pushRequest(JuliusControl::Request request)
{
  nextRequests << request;
}


void JuliusControl::closeLog()
{
  if (!logFile) return;

  jlog_flush();
  fclose(logFile);
  logFile = 0;
}


void JuliusControl::recognize(const QString& fileName)
{
  kDebug() << "Recognizing " << fileName;
  //adin_file_begin(fileName.toUtf8().data());

  QByteArray fileByte = fileName.toUtf8();
  if (isRunning()) {
    kDebug() << "Waiting for previous request...";
    wait(3000);
    if (isRunning())
      return;                                     // if this is taken so long ignore the current input...
  }

  currentFileName = fileName;
  start();
}


JuliusControl::~JuliusControl()
{
  uninitialize();
}
