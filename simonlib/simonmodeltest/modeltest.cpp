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
 */

#include "modeltest.h"

#include <simonlogging/logger.h>

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QProcess>

#include <KUrl>
#include <KConfig>
#include <KConfigGroup>
#include <KStandardDirs>
#include <KLocalizedString>
#include <KComponentData>
#include <KAboutData>
#include <KLocale>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <locale.h>



ModelTest::ModelTest(const QString& userName,  
			     QObject *parent) : QThread(parent)
{
	KLocale::setMainCatalog("simonlib");
	this->userName = userName;
	connect(this, SIGNAL(status(const QString&, int, int)), this, SLOT(addStatusToLog(const QString&)));
	connect(this, SIGNAL(recognitionInfo(const QString&)), this, SLOT(addRecognitionInfoToLog(const QString&)));

	recog = NULL;

	promptsTable.clear();
	wordRates.clear();
	sentenceRates.clear();
	fileResults.clear();
}

QString ModelTest::htkIfyPath(const QString& in)
{
	QString out = in;
	#ifdef Q_OS_WIN
	const char *in_data = out.toUtf8().constData();
	
	long length = 0;
	length = GetShortPathName(in_data, NULL, 0);
	if (length ==0) return QString();
	
	char *out_data = new TCHAR[length];
	length = GetShortPathName(in_data, out_data, length);
	if (length ==0) return QString();
	

	#ifdef UNICODE
	out = QString::fromUtf16((ushort*)out_data);
	#else
	out = QString::fromLocal8Bit(out_data);
	#endif
	
	delete[] out_data;
	#endif
	
	return out;
}

bool ModelTest::createDirs()
{
	tempDir = KStandardDirs::locateLocal("tmp", KGlobal::mainComponent().aboutData()->appName()+"/"+userName+"/test/");
	
	if (tempDir.isEmpty()) return false;

	QDir tempDirHandle(tempDir);
	if (!tempDirHandle.exists())
		return false;

	tempDirHandle.mkdir("samples");

	return true;
}


bool ModelTest::parseConfiguration()
{
	KConfig config( KStandardDirs::locateLocal("config", "simonmodeltestrc"), KConfig::FullConfig );
	KConfigGroup programGroup(&config, "Programs");
	
	sox = programGroup.readEntry("SOX", KUrl(KStandardDirs::findExe("sox"))).path();
	julius = programGroup.readEntry("Julius", KUrl(KStandardDirs::findExe("juliusexe"))).path();
	hResults = programGroup.readEntry("HResults", KUrl(KStandardDirs::findExe("HResults"))).path();

	if (!QFile::exists(sox) ||
			!QFile::exists(julius) ||
			!QFile::exists(hResults))
	{
		//HTK not found
		QString errorMsg = i18n("The HTK, SOX or Julius can not be found. Please make sure it is installed correctly.\n\n");
#ifdef Q_OS_WIN32
		errorMsg += i18n("More information: http://www.cyber-byte.at/wiki/index.php/English:_Setup#Windows");
#else
		errorMsg += i18n("More information: http://www.cyber-byte.at/wiki/index.php/English:_Setup#HTK_Installation");
#endif
		emit error(errorMsg);
		return false;
	}

	return true;
}

bool ModelTest::execute(const QString& command, const QString& outputFilePath, const QString& errorFilePath)
{
	QProcess proc;
	proc.setWorkingDirectory(QCoreApplication::applicationDirPath());
	proc.start(command);
	
	buildLog += "<p><span style=\"font-weight:bold; color:#00007f;\">"+command+"</span></p>";
	proc.waitForFinished(-1);
	
	QByteArray errByte = proc.readAllStandardError();
	QByteArray outByte = proc.readAllStandardOutput();
	QString err = QString::fromLocal8Bit(errByte);
	QString out = QString::fromLocal8Bit(outByte);
	
	if (!out.isEmpty()) {
		QString htmlOut = out;
		htmlOut.remove("<s>");
		htmlOut.remove("</s>");
		buildLog += "<p>"+htmlOut+"</p>";
	}

	if (!err.isEmpty()) {
		QString htmlErr = err;
		htmlErr.remove("<s>");
		htmlErr.remove("</s>");
		buildLog += "<p><span style=\"color:#aa0000;\">"+htmlErr+"</span></p>";
	}

	if (!outputFilePath.isEmpty()) {
		QFile outFile(outputFilePath);
		if (!outFile.open(QIODevice::WriteOnly))
			return false;

		outFile.write(outByte);
	}

	if (!errorFilePath.isEmpty()) {
		QFile outFile(errorFilePath);
		if (!outFile.open(QIODevice::WriteOnly))
			return false;

		outFile.write(errByte);
	}
	
	if (proc.exitCode() != 0) 
		return false;
	else return true;
}

void ModelTest::addStatusToLog(const QString& status)
{
	buildLog += "<p><span style=\"font-weight:bold; color:#358914;\">"+status+"</span></p>";
}

void ModelTest::addRecognitionInfoToLog(const QString& status)
{
	buildLog += "<p>"+status+"</p>";
}

bool ModelTest::hasLog()
{
	return (buildLog.count() > 0);
}

QString ModelTest::getGraphicLog()
{
	QString htmlLog = buildLog;
	htmlLog=htmlLog.replace("\n", "<br />");
	return "<html><head /><body>"+htmlLog+"</body></html>";
}

QString ModelTest::getLog()
{
	QString plainLog = buildLog;
	plainLog.remove("<p>");
	plainLog.remove("</p>");
	plainLog.remove("<span style=\"color:#aa0000;\">");
	plainLog.remove("<span style=\"font-weight:bold; color:#358914;\">");
	plainLog.remove("<span style=\"font-weight:bold; color:#00007f;\">");
	plainLog.remove("</span>");
	return plainLog;
}


bool ModelTest::startTest(const QString& hmmDefsPath, const QString& tiedListPath,
			     const QString& dictPath, const QString& dfaPath,
			     const QString& samplePath, const QString& promptsPath,
			     int sampleRate, const QString& juliusJConf)
{
	if (isRunning()) {
		keepGoing=false;

		//terminate();

		wait();
		emit testAborted();
	}

	this->hmmDefsPath = hmmDefsPath;
	this->tiedListPath = tiedListPath;
	this->dictPath = dictPath;
	this->dfaPath = dfaPath;

	this->samplePath = samplePath;

	this->promptsPath = promptsPath;

	this->sampleRate = sampleRate;
	this->juliusJConf = juliusJConf;

	keepGoing=true;

	buildLog="";

	if (!parseConfiguration())
		return false;

	start();
	return true;
}

void ModelTest::run()
{
	if (!createDirs())
		emit error(i18n("Couldn't generate temporary folders.\n\nPlease check your permissions for \"%1\".", tempDir));

	promptsTable.clear();
	wordRates.clear();
	sentenceRates.clear();
	fileResults.clear();

	if (!keepGoing) return;
	Logger::log(i18n("[INF] Testing model..."));
	emit status(i18n("Preperation"), 0,100);
	
	if (!recodeAudio()) return;
	if (!generateMLF()) return;
	if (!recognize()) return;
//	if (!processJuliusOutput()) return;
	if (!analyzeResults()) return;

	//sync model
	if (!keepGoing) return;
	
	emit status(i18n("Finished"), 100, 100);
	emit testComplete();
}


bool ModelTest::recodeAudio()
{
	emit status(i18n("Recoding audio..."), 5, 100);

	QFile promptsF(promptsPath);
	if (!promptsF.open(QIODevice::ReadOnly)) {
		emit error(i18n("Couldn't open prompts file for reading: %1", promptsPath));
		return false;
	}

	QFile wavListF(tempDir+"wavlist");
	if (!wavListF.open(QIODevice::WriteOnly)) {
		emit error(i18n("Couldn't open wavlist file for writing: %1", tempDir+"wavlist"));
		return false;
	}

	while (!promptsF.atEnd()) {
		QString line = QString::fromUtf8(promptsF.readLine (1024));
		if (line.trimmed().isEmpty()) continue;
		int splitter = line.indexOf(" ");
		QString fileName = line.left(splitter)+".wav";
		QString prompt = line.mid(splitter+1).trimmed();
		QString fullPath = samplePath+QDir::separator()+fileName;
		QString targetPath = tempDir+"samples"+QDir::separator()+fileName;

		execute(QString("%1 -2 -s -L %2 %3").arg(sox).arg(fullPath).arg(targetPath));
		wavListF.write(targetPath.toUtf8()+"\n");

		promptsTable.insert(targetPath, prompt);
	}

	promptsF.close();
	wavListF.close();
	return true;
}


bool ModelTest::generateMLF()
{
	emit status(i18n("Generating MLF..."), 10, 100);
	//echo "Step 1 of 7: Generating MLF"
	//perl `dirname $0`/prompts2mlf testref.mlf prompts
	
	QFile promptsFile(promptsPath);
	QFile mlf(tempDir+"/testref.mlf");

	if (!promptsFile.open(QIODevice::ReadOnly))
		return false;
	if (!mlf.open(QIODevice::WriteOnly))
		return false;
	
	mlf.write("#!MLF!#\n");
	QStringList lineWords;
	QString line;
	while (!promptsFile.atEnd())
	{
		line = QString::fromUtf8(promptsFile.readLine(3000));
		if (line.trimmed().isEmpty()) continue;
		lineWords = line.split(QRegExp("( |\n)"), QString::SkipEmptyParts);
		QString labFile = "\"*/"+lineWords.takeAt(0)+".lab\""; //ditch the file-id 
#ifdef Q_OS_WIN
		mlf.write(labFile.toLatin1()+"\n");
#else
		mlf.write(labFile.toUtf8()+"\n");
#endif
		for (int i=0; i < lineWords.count(); i++)
#ifdef Q_OS_WIN
			mlf.write(lineWords[i].toLatin1()+"\n");
#else
			mlf.write(lineWords[i].toUtf8()+"\n");
#endif
		mlf.write(".\n");
	}
	promptsFile.close();
	mlf.close();
	return true;
}


void processRecognitionResult(Recog *recog, void *test)
{
	int i;
	WORD_INFO *winfo;
	WORD_ID *seq;
	int seqnum;
	int n;
	Sentence *s;
	RecogProcess *r;
	ModelTest *modelTest = (ModelTest*) test;
	Q_ASSERT(modelTest);

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
					modelTest->searchFailed();
					break;
			}
			/* continue to next process instance */
			continue;
		}

		/* output results for all the obtained sentences */
		winfo = r->lm->winfo;

		QList<RecognitionResult> recognitionResults;

		for(n = 0; n < r->result.sentnum; n++) { /* for all sentences */
			QString result, sampa, sampaRaw;
			s = &(r->result.sent[n]);
			seq = s->word;
			seqnum = s->word_num;

			/* output word sequence like Julius */
			//       printf("sentence%d:", n+1);
			for(i=0;i<seqnum;i++) {
				result += " ";
				result += QString::fromUtf8(winfo->woutput[seq[i]]);// printf(" %s", );
			}
			result.remove("<s> ");
			result.remove(" </s>");
			//       printf("\n");

			/* confidence scores */
			//       printf("cmscore%d:", n+1);
			QList<float> confidenceScores;

			for (i=1;i<seqnum-1; i++) {
				confidenceScores << s->confidence[i];
			}

			recognitionResults.append(RecognitionResult(result.trimmed(),
							sampa.trimmed(),
							sampaRaw.trimmed(), confidenceScores));
		}
		modelTest->recognized(recognitionResults);
	}
}


bool ModelTest::recognize()
{
//	return execute(QString("%1 -C %2 -realtime -dfa %3 -v %4 -input rawfile -filelist %5 -smpFreq %6").arg(julius)
//			.arg(juliusJConf).arg(dfaPath).arg(dictPath).arg(tempDir+"wavlist").arg(sampleRate), tempDir+"juliusOutput");

	emit status(i18n("Recognizing..."), 35, 100);


 	FILE *fp;
	QByteArray logPath = tempDir.toUtf8()+"julius.log";

 	fp = fopen(logPath.data(), "w");
 	if (fp == NULL) 
		return false;
	jlog_set_output(fp);
	
	if (!QFile::exists(juliusJConf)) {
		emit error(i18n("Couldn't open julius jconf file: \"%1\".", juliusJConf));
	}
	
	
	//////BEGIN: Workaround
	//convert "." in hmmdefs to its locale specific equivalent
	lconv * localeConv = localeconv();
	char *decimalPoint = localeConv->decimal_point;

	QFile hmm(hmmDefsPath);
	QFile hmmLoc(tempDir+"hmmdefs");
	if (!hmm.open(QIODevice::ReadOnly) || !hmmLoc.open(QIODevice::WriteOnly))
		return NULL;

	while (!hmm.atEnd())
		hmmLoc.write(hmm.readLine(3000).replace(".", decimalPoint));
	
	hmm.close();
	hmmLoc.close();
	//////END: Workaround


	QByteArray hmmDefs = tempDir.toUtf8()+"hmmdefs";
	int argc=15;

	char* argv[] = {"sam", "-C", juliusJConf.toLocal8Bit().data(),
			"-dfa", dfaPath.toLocal8Bit().data(),
			"-v", dictPath.toLocal8Bit().data(),
			 "-h", QString(tempDir+"hmmdefs").toLocal8Bit().data(),
			 "-hlist", tiedListPath.toLocal8Bit().data(),
			 "-input", "rawfile", 
			 "-filelist", QString(tempDir+"wavlist").toLocal8Bit().data(),
			 "-smpFreq", QString::number(sampleRate).toLocal8Bit().data()};

	Jconf *jconf = j_config_load_args_new(argc, argv);
	if (!jconf) {
		emit error(i18n("Internal Jconf error"));
		return false;
	}

	recog = j_create_instance_from_jconf(jconf);
	if (!recog)
	{
		emit error(i18n("Could not initialize recognition"));
		j_jconf_free(jconf);
		this->recog=0;
		return false;
	}
	
	callback_add(recog, CALLBACK_RESULT, processRecognitionResult, this);
	
	/**************************/
	/* Initialize audio input */
	/**************************/
	/* initialize audio input device */
	/* ad-in thread starts at this time for microphone */
	if (j_adin_init(recog) == false) {    /* error */
		emit error(i18n("Couldn't start adin-thread"));
		j_recog_free(recog);
		this->recog=0;
		return false;
	}

	/* output system information to log */
	j_recog_info(recog);
	
	bool shouldBeRunning = true;
	while (shouldBeRunning)
	{
		 switch(j_open_stream(recog, NULL)) {
			case 0:
			//	fprintf(stderr, "j_open_stream returned 0\n");
			//	emit recognitionStarted();
				break;
			case -1:
			//	fprintf(stderr, "j_open_stream returned -1\n");
				break;
			//	emit recognitionError(i18n("Unknown error"));
				//return false;
				//
				//skipping to -2 for freeing...
			case -2:
			//	fprintf(stderr, "j_open_stream returned -2\n");
				shouldBeRunning = false;
				//filelist input somehow returns -2 on finish
			//	emit error(i18n("Couldn't recognize samples"));
			//	if (isLocal)
			//		emit recognitionError(i18n("Couldn't initialize microphone"));
			//	else emit recognitionError(i18n("Error with the audio stream"));
				break;
		}

		 if (!shouldBeRunning)
			 break;
	
		/**********************/
		/* Recognization Loop */
		/**********************/
		/* enter main loop to recognize the input stream */
		/* finish after whole input has been processed and input reaches end */
		int ret = j_recognize_stream(recog);
 		switch (ret)
		{
			case 0:
				//client exited
				//shouldBeRunning=false;
				break;
			case -1:
				//emit recognitionError("recognize_stream: -1");
				shouldBeRunning=false;
				break;
		}
		//usleep(300);
	}

	j_recog_free(recog);

	return true;
}





void ModelTest::searchFailed()
{
	QString fileName = QString::fromUtf8(QByteArray(j_get_current_filename(recog)));

	fileResults.insert(fileName, RecognitionResultList());

	emit recognitionInfo(i18n("Search failed for: %1", fileName));

	QString prompt = promptsTable.value(fileName);
	QStringList promptWordList = prompt.split(" ");

	FloatList list = sentenceRates.value(prompt);
	sentenceRates.insert(prompt, list << 0.0f);

	foreach (const QString& word, promptWordList) {
		FloatList list2 = wordRates.value(word);
		wordRates.insert(word, list2 << 0.0f);
	}
}

void ModelTest::recognized(RecognitionResultList results)
{
	QString fileName = QString::fromUtf8(QByteArray(j_get_current_filename(recog)));

	fileResults.insert(fileName, results);

	QString prompt = promptsTable.value(fileName);

	emit recognitionInfo(i18n("Prompts entry: %1", prompt));
	emit recognitionInfo(i18n("Received recognition result for: %1: %2", fileName, results.at(0).sentence()));

	QStringList promptWordList = prompt.split(" ");

	bool sentenceFound = false;
	QList<bool> wordFound;
	for (int i=0; i< promptWordList.count(); i++)
		wordFound << false;

	foreach (const RecognitionResult& result, results) {
		QStringList resultWordList = result.sentence().split(" ");
		QList<float> confidenceScores = result.confidenceScores();

		if (promptWordList.count() != resultWordList.count()) {
			//if different word count skip this result
			//wrong segmentation is "punished" with a 0% rate
			continue;
		}

		int i=0;
		foreach (const QString& resultWord, resultWordList) {
			//word recognition rates
			if (wordFound[i]) continue;

			if (resultWord.toUpper() == promptWordList[i]) {
				float wordConfidence = confidenceScores[i];
				FloatList previousWordConfidenceList = wordRates.value(resultWord.toUpper());

				wordRates.insert(resultWord.toUpper(), previousWordConfidenceList << wordConfidence);
				wordFound.replace(i, true);
			} 

			i++;
		}

		if ((!sentenceFound) && (prompt == result.sentence().toUpper())) {
			//correct sentence
			FloatList list = sentenceRates.value(result.sentence().toUpper());
			
			sentenceRates.insert(result.sentence().toUpper(), list << result.averageConfidenceScore());
			sentenceFound = true;
		}
	}

	if (!sentenceFound) {
		FloatList list = sentenceRates.value(prompt);
		sentenceRates.insert(prompt, list << 0.0f);
	}

	int k=0;
	foreach (bool found, wordFound) {
		if (!found) {
			FloatList list = wordRates.value(promptWordList[k]);

			wordRates.insert(promptWordList[k], list << 0.0f);
		}
		k++;
	}
}




bool ModelTest::processJuliusOutput()
{
	emit status(i18n("Processing recognition results..."), 85, 100);


	return true;
}

bool ModelTest::analyzeResults()
{
	emit status(i18n("Analyzing recognition results..."), 90, 100);

	//Open the file: tempDir+juliusOutput
	//Search for:
	//	### read waveform input
	//	Stat: adin_file: input speechfile: /tmp/kde-bedahr/sam/internalsamuser/test/samples/Acht_S9_2009-04-23_12-39-47_45_2009-07-30_16-46-21.wav
	//
	//
	
	
	return true;
}

ModelTest::~ModelTest()
{
}

