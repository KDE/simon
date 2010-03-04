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

#include "modelcompilationmanager.h"

#include <simonlogging/logger.h>

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QString>
#include <QVector>

#include <KUrl>
#include <KConfig>
#include <KConfigGroup>
#include <KStandardDirs>
#include <KComponentData>
#include <KAboutData>
#include <KLocale>
#include <KDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#endif


ModelCompilationManager::ModelCompilationManager(const QString& user_name,
			     QObject* parent) : QThread(parent),
	userName(user_name)
{
	connect(this, SIGNAL(status(const QString&, int, int)), this, SLOT(addStatusToLog(const QString&)));
}



QString ModelCompilationManager::htkIfyPath(const QString& in)
{
	QString out = in;
	
	#ifdef Q_OS_WIN
	long     length = 0;
	char*   buffer = NULL;
	
	// First obtain the size needed by passing NULL and 0.
	length = GetShortPathNameA(out.toLocal8Bit().data(), NULL, 0);
	if (length == 0) return QByteArray();
	
	// Dynamically allocate the correct size 
	// (terminating null char was included in length)
	buffer = new char[length];

	// Now simply call again using same long path.
	length = GetShortPathNameA(out.toLocal8Bit().data(), buffer, length);
	if (length == 0) return QByteArray();

	out = QString::fromLocal8Bit(buffer);
	delete [] buffer;	
	#endif
	
	return out;
}


bool ModelCompilationManager::createDirs()
{
	tempDir = KStandardDirs::locateLocal("tmp", KGlobal::mainComponent().aboutData()->appName()+"/"+userName+"/compile/");
	
	if (tempDir.isEmpty()) return false;

	QDir tempDirHandle(tempDir);
	if (!tempDirHandle.exists())
		return false;

	if (!tempDirHandle.exists("xforms") && !tempDirHandle.mkdir("xforms"))
		return false;
	if (!tempDirHandle.exists("classes") && !tempDirHandle.mkdir("classes"))
		return false;

	for (int i=0; i < 17; i++)
	{
		if (!tempDirHandle.exists("hmm"+QString::number(i)) && 
			!tempDirHandle.mkdir("hmm"+QString::number(i)))
			return false;
	}
	return true;
}


bool ModelCompilationManager::parseConfiguration()
{
	KConfig config( KStandardDirs::locateLocal("config", "simonmodelcompilationrc"), KConfig::FullConfig );
	KConfigGroup programGroup(&config, "Programs");
	
	if ((compilationType & ModelCompilationManager::CompileSpeechModel)||
	   (compilationType & ModelCompilationManager::AdaptSpeechModel)){
		hDMan = programGroup.readEntry("HDMan", KUrl(KStandardDirs::findExe("HDMan"))).path();
		hLEd = programGroup.readEntry("HLEd", KUrl(KStandardDirs::findExe("HLEd"))).path();
		hCopy = programGroup.readEntry("HCopy", KUrl(KStandardDirs::findExe("HCopy"))).path();
		hCompV = programGroup.readEntry("HCompV", KUrl(KStandardDirs::findExe("HCompV"))).path();
		hERest = programGroup.readEntry("HERest", KUrl(KStandardDirs::findExe("HERest"))).path();
		hHEd = programGroup.readEntry("HHEd", KUrl(KStandardDirs::findExe("HHEd"))).path();
		hVite = programGroup.readEntry("HVite", KUrl(KStandardDirs::findExe("HVite"))).path();
	}
	if (compilationType & ModelCompilationManager::CompileLanguageModel) {
		mkfa = programGroup.readEntry("mkfa", KUrl(KStandardDirs::findExe("mkfa"))).path();
		dfaMinimize = programGroup.readEntry("dfa_minimize", KUrl(KStandardDirs::findExe("dfa_minimize"))).path();
	}

	if ((compilationType & ModelCompilationManager::CompileSpeechModel) && 
		(!QFile::exists(hDMan) || !QFile::exists(hCopy) ||
		!QFile::exists(hCompV) || !QFile::exists(hERest) ||
		!QFile::exists(hVite)))
	{
		//HTK not found
		QString errorMsg = i18n("The HTK can not be found. Please make sure it is installed correctly.\n\n"
#ifdef Q_OS_WIN32
		"More information: http://www.cyber-byte.at/wiki/index.php/English:_Setup#Windows"
#else
		"More information: http://www.cyber-byte.at/wiki/index.php/English:_Setup#HTK_Installation"
#endif
		);
		emit error(errorMsg);
		return false;
	}

	if ((compilationType & ModelCompilationManager::CompileLanguageModel) && 
		(!QFile::exists(mkfa) || !QFile::exists(dfaMinimize)))
	{
		//julius grammar tools not found
		emit error(i18n("The julius related grammar tools mkfa and dfa_minimize can not be found.\n\nA reinstallation of simon could solve this problem."));
		return false;
	}

	return true;
}

bool ModelCompilationManager::execute(const QString& command)
{
	QProcess proc;
	proc.setWorkingDirectory(tempDir);
	proc.start(command);

	activeProcesses << &proc;
	
	buildLog += "<p><span style=\"font-weight:bold; color:#00007f;\">"+command+"</span></p>";
	proc.waitForFinished(-1);

	activeProcesses.removeAll(&proc);
	
	QString err = QString::fromLocal8Bit(proc.readAllStandardError());
	QString out = QString::fromLocal8Bit(proc.readAllStandardOutput());
	
	if (!out.isEmpty())
		buildLog += "<p>"+out+"</p>";

	if (!err.isEmpty())
		buildLog += "<p><span style=\"color:#aa0000;\">"+err+"</span></p>";
	
	if (proc.exitCode() != 0) 
		return false;
	else return true;
}

void ModelCompilationManager::addStatusToLog(const QString& status)
{
	buildLog += "<p><span style=\"font-weight:bold; color:#358914;\">"+status+"</span></p>";

}

bool ModelCompilationManager::hasBuildLog()
{
	return (buildLog.count() > 0);
}

QString ModelCompilationManager::getGraphicBuildLog()
{
	QString htmlLog = buildLog;
	htmlLog=htmlLog.replace("\n", "<br />");
	return "<html><head /><body>"+htmlLog+"</body></html>";
}

QString ModelCompilationManager::getBuildLog()
{
	QString plainLog = buildLog;
	plainLog.remove("<p>");
	plainLog.remove("</p>");
	plainLog.remove("<span style=\"color:#aa0000;\">");
	plainLog.remove("<span style=\"font-weight:bold; color:#00007f;\">");
	plainLog.remove("<span style=\"font-weight:bold; color:#358914;\">");
	plainLog.remove("</span>");
	return plainLog;
}

/**
 * \brief Gets a user-readable error and checks if we have not run into a common problem; Generates the final error and emits it using error()
 * \author Peter Grasch
 * 
 * This method is kind of a proxy: It gets its error message from the compilation process itself. Such a message might be:
 * 	"Could not generate HMM13. Check paths to..."
 * 
 * Before emitting that exakt error to the controlling process, we will check with processError which in turn will inspect the build protocol
 * to find common mistakes.
 * Only if processError does not know how to handle this error (i.e. not a common mistake) we will emit the given readableError.
 */
void ModelCompilationManager::analyseError(QString readableError)
{
	if (!processError())
		emit error(readableError);
}

/**
 * \brief Processes an error (reacts on it some way)
 * \author Peter Grasch
 * @return If this is true we knew what to do; if this is false you'd better throw an error message
 */
bool ModelCompilationManager::processError()
{
	QString err = getBuildLog().trimmed();

	int startIndex=0;
	if (err.contains("ERROR [+2019]"))
	{ // no trainings samples
		
		emit error(i18n("No training material available.\n\nPlease train your acoustic model by recording samples."));
		return true;
	}
	if ((startIndex = err.indexOf("ERROR [+1232]")) != -1) //word missing
	{
		//ERROR [+1232]  NumParts: Cannot find word DARAUFFOLGEND in dictionary
		int wordstart = 42+startIndex;
		QString word = err.mid(wordstart, err.indexOf(' ', wordstart)-wordstart);
		
		//this error ONLY occurs when there are samples for the word but the word itself was not added
		//to the dictionary so - ADD THE WORD!
		emit wordUndefined(word);
		return true;
	}
	if ((startIndex = err.indexOf("ERROR [+2662]")) != -1)
	{
// 		"ERROR [+2662]  FindProtoModel: no proto for E in hSet
// 		 FindProtoModel: no proto for n
		QString phoneme = err.mid(44+startIndex);
		phoneme = phoneme.left(phoneme.indexOf(' '));
 		emit phonemeUndefined(phoneme);
		return true;
	}
	if ((startIndex = err.indexOf("undefined class \"")) != -1)
/*		Error:       undefined class "NOM"*/
	{
		QString undefClass = err.mid(startIndex+17);
		undefClass = undefClass.left(undefClass.indexOf('"'));
		emit classUndefined(undefClass);
		return true;
	}

	return false;
}

void ModelCompilationManager::abort()
{
	if (isRunning()) {
		keepGoing=false;

		foreach (QProcess *proc, activeProcesses)
			//tell any running process to commit suicide
			proc->kill(); 

		wait();

		emit activeModelCompilationAborted();
	}
}

bool ModelCompilationManager::startCompilation(ModelCompilationManager::CompilationType compilationType,
			     const QString& hmmDefsPath, const QString& tiedListPath,
			     const QString& dictPath, const QString& dfaPath, 
			     const QString& baseHmmDefsPath, const QString& baseTiedlistPath,
			     const QString& baseMacrosPath, const QString& baseStatsPath,
			     const QString& samplePath,
			     const QString& lexiconPath, const QString& grammarPath, 
			     const QString& vocabPath, const QString& promptsPath, 
			     const QString& treeHedPath, const QString& wavConfigPath)
{
	abort();
	wait();

	this->hmmDefsPath = hmmDefsPath;
	this->tiedListPath = tiedListPath;
	this->dictPath = dictPath;
	this->dfaPath = dfaPath;

	this->baseHmmDefsPath = baseHmmDefsPath;
	this->baseTiedlistPath = baseTiedlistPath;
	this->baseStatsPath = baseStatsPath;
	this->baseMacrosPath = baseMacrosPath;

	this->compilationType = compilationType;

	this->samplePath = samplePath;

	this->lexiconPath = lexiconPath;
	this->grammarPath = grammarPath;
	this->vocabPath = vocabPath;
	this->promptsPath = promptsPath;
	this->treeHedPath = treeHedPath;
	this->wavConfigPath = wavConfigPath;

	keepGoing=true;

	buildLog="";

	if (!parseConfiguration())
		return false;

	start();
	return true;
}

void ModelCompilationManager::run()
{
	if (!createDirs())
		analyseError(i18n("Couldn't generate temporary folders.\n\nPlease check your permissions for \"%1\".", tempDir));

	fprintf(stderr, "Temp dir in run(): %s\n", tempDir.toUtf8().data());

	if (!keepGoing) return;
	Logger::log("[INF] Compiling model...");
	emit status(i18n("Preperation"), 0,2300);
	

	if ((compilationType & ModelCompilationManager::CompileSpeechModel) ||
			(compilationType & ModelCompilationManager::AdaptSpeechModel))
	{
		if (!generateInputFiles()) return;
		if (!makeTranscriptions()) return;
		if (!codeAudioData()) return;
	}

	if (compilationType & ModelCompilationManager::AdaptSpeechModel)
		if (!adaptBaseModel()) return;

	if (compilationType & ModelCompilationManager::CompileSpeechModel)
		if (!buildHMM()) return;

	if (compilationType & ModelCompilationManager::CompileLanguageModel) {
		if (!compileGrammar()) return;
	}
	//sync model
	if (!keepGoing) return;
	
	emit status(i18n("Finished"), 2300, 2300);
	emit modelCompiled();
}


bool ModelCompilationManager::compileGrammar()
{
	if (!keepGoing) return false;
	emit status(i18n("Generating reverse grammar..."), 2000);
	if (!generateReverseGrammar())
	{
		analyseError(i18n("Couldn't create reverse grammar.\n\nDid you define a grammar?"));
		return false;
	}

	if (!keepGoing) return false;
	emit status(i18n("Generating termporary vocabulary..."), 2100);
	if (!makeTempVocab())
	{
		analyseError(i18n("Couldn't create temporary vocabular."));
		return false;
	}

	if (!keepGoing) return false;
	emit status(i18n("Generating DFA..."), 2250);
	if (!makeDfa())
	{
		analyseError(i18n("Couldn't generate dfa. Please check the paths to mkfa and dfa_minimize (%1, %2).", mkfa, dfaMinimize));
		return false;
	}
	
	if (!keepGoing) return false;
	emit status(i18n("Generating grammar dictionary..."), 2299);
	if (!generateDict())
	{
		analyseError(i18n("Couldn't generate grammatical lexicon. Please check the output path (%1).", dictPath));
		return false;
	}
	
	return true;
}

bool ModelCompilationManager::makeTempVocab()
{
	QFile vocab ( vocabPath );

	QString terminal;
	if ( !vocab.open ( QFile::ReadOnly ) ) return false;
	
	QFile tmpVocab ( tempDir+"/tempvoca" );
	if ( !tmpVocab.open ( QFile::WriteOnly ) ) return false;

	QFile term ( tempDir+"/term" );
	if ( !term.open ( QFile::WriteOnly ) ) return false;
	
	QString vocabEntry;

	int termid=0;
	while ( !vocab.atEnd() )
	{
		vocabEntry = QString::fromUtf8(vocab.readLine ( 1024 ));
		vocabEntry.remove ( QRegExp ( "\r+$" ) );
		vocabEntry.remove ( QRegExp ( "#.*" ) );
		vocabEntry = vocabEntry.trimmed();
		if ( vocabEntry.isEmpty() ) continue;
		if ( vocabEntry.startsWith ( '%' ) )
		{
			terminal = vocabEntry.mid ( 1 ).trimmed();
			tmpVocab.write ( '#'+terminal.toUtf8() +'\n' );

			term.write ( QString::number(termid).toUtf8()+'\t'+terminal.toUtf8() +'\n' );
			termid++;
		}
	}
	vocab.close();
	tmpVocab.close();
	term.close();
	return true;
}


bool ModelCompilationManager::makeDfa()
{
	QString execStr = '"'+mkfa+"\" -e1 -fg \""+tempDir+"/reverseGrammar"+"\" -fv \""+tempDir+"/tempvoca\" -fo \""+tempDir+"/dfaTemp.tmp"+"\" -fh \""+tempDir+"/dfaTemp.h\"";
	if (!execute(execStr)) return false;

	execStr = '"'+dfaMinimize+'"'+" \""+tempDir+"/dfaTemp.tmp\" -o \""+dfaPath+'"';
	return execute(execStr);
}

bool ModelCompilationManager::generateReverseGrammar()
{
	QFile grammar(grammarPath);
	if (!grammar.open(QFile::ReadOnly)) return false;

	QFile reverseGrammar(tempDir+"/reverseGrammar");
	if (!reverseGrammar.open(QFile::WriteOnly)) return false;

	QString reverseGrammarEntry;
	QString grammarEntry;
	QStringList terminals;
	QString identifier;
	
	int structureCount=0;
	
	while (!grammar.atEnd())
	{
		grammarEntry = QString::fromUtf8(grammar.readLine()).trimmed();
		grammarEntry.remove(QRegExp("\r+$"));
		grammarEntry.remove(QRegExp("#.*"));

		if (grammarEntry.trimmed().isEmpty()) continue;
		
		//example: "S:NS_B NOM NS_E"
		int splitter =grammarEntry.indexOf(":");
		if (splitter == -1) continue;
		reverseGrammarEntry = grammarEntry.left(splitter+1);
		//reverse = "S:"
		
		terminals = grammarEntry.mid(splitter+1).split(' ');
		for (int j=terminals.count()-1; j >= 0; j--)
			reverseGrammarEntry += terminals[j]+' ';
		
		structureCount++;
		// reverse = "S:NS_E NOM NS_B "
		reverseGrammar.write(reverseGrammarEntry.toUtf8()+'\n');
	}
	reverseGrammar.close();
	grammar.close();
	
	return (structureCount > 0);
}

bool ModelCompilationManager::generateDict()
{
	int nowId = -1;
	QFile vocab(vocabPath);
	if (!vocab.open(QFile::ReadOnly)) return false;
	QFile dict(dictPath);
	if (!dict.open(QFile::WriteOnly)) return false;
	QString vocabEntry;
	QStringList entryPart;
	
	while (!vocab.atEnd())
	{
		vocabEntry = vocab.readLine(1024);
		vocabEntry.remove(QRegExp("\r+$"));
		vocabEntry.remove(QRegExp("#.*"));
		vocabEntry = vocabEntry.trimmed();
		if (vocabEntry.isEmpty()) continue;
		
		if (vocabEntry.startsWith('%'))
		{
			nowId++;
			continue;
		} else
		{
			int splitter = vocabEntry.indexOf('\t');
			if (splitter == -1) continue;
			
			dict.write(QString(QString::number(nowId)+"\t["+vocabEntry.left(splitter)+"]\t"+vocabEntry.mid(splitter).trimmed()+'\n').toAscii());
		}
	}
	
	vocab.close();
	dict.close();
	return true;
}

bool ModelCompilationManager::codeAudioData()
{
	if (!keepGoing) return false;
	emit status(i18n("Coding audio files..."), 150);
	
	//creating codetrain
	if (!generateCodetrainScp())
	{
		analyseError(i18n("Couldn't create codetrain-file."));
		return false;
	}

	QString codetrainPath = tempDir+"/codetrain.scp";

	//TODO: implement some sort of caching (maybe with an file/hash combination?)
	QString execStr = '"'+hCopy+"\" -A -D -T 1 -C \""+htkIfyPath(wavConfigPath)+"\" -S \""+htkIfyPath(codetrainPath)+'"';
	if (!execute(execStr)) 
	{
		analyseError(i18n("Error while coding the samples!\n\nPlease check the path to HCopy (%1) and the wav config (%2)", hCopy, wavConfigPath));
		return false;
	}
	return true;
}

bool ModelCompilationManager::generateCodetrainScp()
{
	QString codetrainPath = tempDir+"/codetrain.scp";
	QString trainPath = tempDir+"/train.scp";

	QFile promptsFile(promptsPath);
	if (!promptsFile.open(QIODevice::ReadOnly))
		return false;
	
	QString pathToMFCs =tempDir+"/mfcs";
	QDir().mkpath(pathToMFCs);
	
	QFile scpFile(codetrainPath);
	QFile trainScpFile(trainPath);
	if (!scpFile.open(QIODevice::WriteOnly|QIODevice::Truncate) || !trainScpFile.open(QIODevice::WriteOnly|QIODevice::Truncate))
	{
		return false;
	}

	QString fileBase;
	QString mfcFile;
	

	
	while (!promptsFile.atEnd())
	{
		QString line = QString::fromUtf8(promptsFile.readLine());
		
		fileBase =  line.left(line.indexOf(' '));
		mfcFile = htkIfyPath(pathToMFCs)+'/'+fileBase+".mfc";

		QString wavFile = htkIfyPath(samplePath)+'/'+fileBase+".wav";
		scpFile.write(QString('"'+wavFile+ "\" \"" +mfcFile+"\"\n").toLocal8Bit());
		#ifdef Q_OS_WIN
		trainScpFile.write(mfcFile.toLocal8Bit()+'\n');
		#else
		trainScpFile.write(mfcFile.toUtf8()+'\n');
		#endif
	}
	promptsFile.close();
	scpFile.close();
	trainScpFile.close();
	return true;
}

bool ModelCompilationManager::generateInputFiles()
{	
	if (!keepGoing) return false;
	emit status(i18n("Generating wordlist..."), 35);
	
	//wlist
	if (!generateWlist())
	{
		analyseError(i18n("Failed to create wordlist. Please check the path to your prompts file."));
		return false;
	}

	//monophones
	if (!keepGoing) return false;
	emit status(i18n("Creating monophones..."), 40);

	
	if (!makeMonophones())
	{
		analyseError(i18n("Failed to generate monophones. Please check that you have installed the Program HDMan correctly and that the lexicon contains all words referenced by your trainings corpus."));
		return false;
	}

	return true;
}

bool ModelCompilationManager::makeTranscriptions()
{
	//mlf
	if (!keepGoing) return false;
	emit status(i18n("Generating Master Label File..."), 55);
	if (!generateMlf())
	{
		analyseError(i18n("Couldn't generate Master Label File. Please check your prompts file (%1).", promptsPath));
		return false;
	}
	
	
	if (!execute('"'+hLEd+"\" -A -D -T 1 -l \"*\" -d \""+htkIfyPath(tempDir)+"/dict\" -i \""+htkIfyPath(tempDir)+"/phones0.mlf\" \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/mkphones0.led"))+"\" \""+htkIfyPath(tempDir)+"/words.mlf"+"\"") || !execute('"'+hLEd+"\" -A -D -T 1 -l \"*\" -d \""+htkIfyPath(tempDir)+"/dict"+"\" -i \""+htkIfyPath(tempDir)+"/phones1.mlf\" \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/mkphones1.led"))+"\" \""+htkIfyPath(tempDir)+"/words.mlf\""))
	{
		analyseError(i18n("Generation of the transcription failed. Please check if you have correctly specified the paths to mkphones0.led and mkphons1.led. (%1, %2)", KStandardDirs::locate("data", "simon/scripts/mkphones0.led"), KStandardDirs::locate("data", "simon/scripts/mkphones1.led")));
		return false;
	}
	return true;
}

bool ModelCompilationManager::createMonophones()
{
	if (!keepGoing) return false;
	emit status(i18n("Generating hmm0..."), 550);
	if (!buildHMM0())
	{
		analyseError(i18n("Error when generating the HMM0.\n\nPlease check if there is enough training material and that the path tos HCompV(%1), the config (%2) and the phoneme prototype (%3) are correct.", hCompV, KStandardDirs::locate("data", "simon/scripts/config"), KStandardDirs::locate("data", "simon/scripts/proto")));
		return false;
	}
	if (!keepGoing) return false;
	emit status(i18n("Generating hmm1..."), 800);
	if (!buildHMM1())
	{
		analyseError(i18n("Could not generate the HMM1.\n\nPlease check the path to HERest (%1) and to the config (%2)", hERest, KStandardDirs::locate("data", "simon/scripts/config")));
		return false;
	}
	if (!keepGoing) return false;
	emit status(i18n("Generating hmm2..."), 850);
	if (!buildHMM2())
	{
		analyseError(i18n("Could not generate the HMM2.\n\nPlease check the path to HERest (%1) and to the config (%2)", hERest, KStandardDirs::locate("data", "simon/scripts/config")));
		return false;
	}
	if (!keepGoing) return false;
	emit status(i18n("Generating hmm3..."), 900);
	if (!buildHMM3())
	{
		analyseError(i18n("Could not generate the HMM3.\n\nPlease check the path to HERest (%1) and to the config (%2)", hERest, KStandardDirs::locate("data", "simon/scripts/config")));
		return false;
	}
	return true;
}

bool ModelCompilationManager::fixSilenceModel()
{
	if (!keepGoing) return false;
	emit status(i18n("Generating Pause-Model (hmm4)..."), 950);
	if (!buildHMM4())
	{
		analyseError(i18n("Could not generate HMM4.\n"
"\n"
"Please check the HMM3."));
		return false;
	}
	if (!keepGoing) return false;
	emit status(i18n("Generating hmm5..."), 1000);
	if (!buildHMM5())
	{
		analyseError(i18n("Could not generate HMM5. Please check the paths to HHEd (%1) and to the silence-model (%2).", hHEd, KStandardDirs::locate("data", "simon/scripts/sil.hed")));
		return false;
	}
	if (!keepGoing) return false;
	emit status(i18n("Generating hmm6..."), 1080);
	if (!buildHMM6())
	{
		analyseError(i18n("Could not generate the HMM6.\n\nPlease check the path to HERest (%1) and to the config (%2)", hERest, KStandardDirs::locate("data", "simon/scripts/config")));
		return false;
	}
	if (!keepGoing) return false;
	emit status(i18n("Generating hmm7..."), 1150);
	if (!buildHMM7())
	{
		analyseError(i18n("Could not generate the HMM6.\n\nPlease check the path to HERest (%1) and to the config (%2)", hERest, KStandardDirs::locate("data", "simon/scripts/config")));
		return false;
	}
	
	return true;
}

bool ModelCompilationManager::realign()
{
	if (!keepGoing) return false;
	emit status(i18n("Generating dict1..."), 1160);
	if (!makeDict1())
	{
		analyseError(i18n("Could not generate the dict1"));
		return false;
	}

	if (!keepGoing) return false;
	emit status(i18n("Re-Aligning HMM7..."), 1160);
	if (!realignHMM7())
	{
		analyseError(i18n("Couldn't re-align hmm7. Please check your paths to HVite (%1), the config (%2) and to the HMM7.", hVite, KStandardDirs::locate("data", "simon/scripts/config")));
		return false;
	}

	if (!keepGoing) return false;
	emit status(i18n("Generating hmm8..."), 1230);
	if (!buildHMM8())
	{
		analyseError(i18n("Could not generate the HMM8.\n\nPlease check the path to HERest (%1) and to the config (%2)", hERest, KStandardDirs::locate("data", "simon/scripts/config")));
		return false;
	}

	if (!keepGoing) return false;
	emit status(i18n("Generating hmm9..."),1300);
	if (!buildHMM9())
	{
		analyseError(i18n("Could not generate the HMM9.\n\nPlease check the path to HERest (%1) and to the config (%2)", hERest, KStandardDirs::locate("data", "simon/scripts/config")));
		return false;
	}
	
	return true;
}

bool ModelCompilationManager::tieStates()
{
	if (!keepGoing) return false;
	emit status(i18n("Generating triphone..."),1700);
	
	if (!execute('"'+hDMan+"\" -A -D -T 1 -b sp -n \""+htkIfyPath(tempDir)+"/fulllist" +"\" -g \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/global.ded"))+"\" \""+htkIfyPath(tempDir)+"/dict-tri\" \""+htkIfyPath(tempDir)+"/lexicon\""))
	{
		analyseError(i18n("Couldn't bind triphones.\n\nPlease check the paths to HDMan (%1), global.ded (%2) and to the lexicon (%3).", hDMan, KStandardDirs::locate("data", "simon/scripts/global.ded"), lexiconPath));
		return false;
	}

	if (!keepGoing) return false;
	emit status(i18n("Generating list of triphones..."),1705);
	if (!makeFulllist())
	{
		analyseError(i18n("Couldn't generate list of triphones."));
		return false;
	}
	if (!keepGoing) return false;
	emit status(i18n("Generating tree.hed..."), 1750);
	if (!makeTreeHed())
	{
		analyseError(i18n("Couldn't generate tree.hed."));
		return false;
	}
	
	if (!keepGoing) return false;
	emit status(i18n("Generating hmm13..."),1830);
	if (!buildHMM13())
	{
		analyseError(i18n("Could not generate HMM13.\n\nPlease check the path to HHEd (%1).", hHEd));
		return false;
	}
	
	
	if (!keepGoing) return false;
	emit status(i18n("Generating hmm14..."),1900);
	if (!buildHMM14())
	{
		analyseError(i18n("Couldn't generate HMM14. Please check the paths to HERest (%1), the config (%2) and to the stats-file (%3).", hERest, KStandardDirs::locate("data", "simon/scripts/config"), tempDir+"/stats"));
		return false;
	}
	
	if (!keepGoing) return false;
	emit status(i18n("Generating hmm15..."),1990);
	if (!buildHMM15())
	{
		analyseError(i18n("Could not generate the HMM15.\n\nPlease check the path to HERest (%1), to the config (%2) and to the stats-file (%3).", hERest, KStandardDirs::locate("data", "simon/scripts/config"), tempDir+"/stats"));
		return false;
	}

	
	return true;
}

bool ModelCompilationManager::buildHMM13()
{
	QString execString = '"'+hHEd+"\" -A -D -T 1 -H \""+htkIfyPath(tempDir)+"/hmm12/macros\" -H \""+htkIfyPath(tempDir)+"/hmm12/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm13/\" \""+htkIfyPath(tempDir)+"/tree.hed\" \""+htkIfyPath(tempDir)+"/triphones1\"";
	return execute(execString);
}


bool ModelCompilationManager::buildHMM14()
{
	return execute('"'+hERest+"\" -A -D -T 1 -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -I \""+htkIfyPath(tempDir)+"/wintri.mlf\" -t 250.0 150.0 3000.0 -s \""+htkIfyPath(tempDir)+"/stats\" -S \""+htkIfyPath(tempDir)+"/train.scp\" -H \""+htkIfyPath(tempDir)+"/hmm13/macros\" -H \""+htkIfyPath(tempDir)+"/hmm13/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm14/\" \""+htkIfyPath(tempDir)+"/tiedlist\"");
}


bool ModelCompilationManager::buildHMM15()
{
	return execute('"'+hERest+"\" -A -D -T 1 -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -I \""+htkIfyPath(tempDir)+"/wintri.mlf\" -t 250.0 150.0 3000.0 -s \""+htkIfyPath(tempDir)+"/stats\" -S \""+htkIfyPath(tempDir)+"/train.scp\" -H \""+htkIfyPath(tempDir)+"/hmm14/macros\" -H \""+htkIfyPath(tempDir)+"/hmm14/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm15/\" \""+htkIfyPath(tempDir)+"/tiedlist\"");
}

bool ModelCompilationManager::makeFulllist()
{	
	QFile::remove(tempDir+"/fulllist-original");
	if (!QFile::copy(tempDir+"/fulllist", tempDir+"/fulllist-original")) return false;
	
	QFile triphones1(tempDir+"/triphones1");
	QFile fulllist(tempDir+"/fulllist");
	
	
	//copy the triphones from triphones1 to fulllist
	//BUT: omit duplicates!
	
	QStringList fulllistEntries;
	if (!fulllist.open(QIODevice::ReadWrite)) return false;
	while (!fulllist.atEnd()) fulllistEntries << fulllist.readLine(150);
	
	if (!triphones1.open(QIODevice::ReadOnly)) return false;
	
	while (!triphones1.atEnd())
	{
		QByteArray triphone = triphones1.readLine(500);
		if (!fulllistEntries.contains(triphone))
			fulllist.write(triphone);
	}
	triphones1.close();
		
	return true;
}

bool ModelCompilationManager::makeTreeHed()
{	
	QFile::remove(tempDir+"/tree.hed");

// 	if (!QFile::copy(treeHedPath, tempDir+"/tree.hed"))) return false;
	
	QFile tree1Hed(treeHedPath);
	QFile treeHed(tempDir+"/tree.hed");
	
	if ((!treeHed.open(QIODevice::WriteOnly)) || 
		(!tree1Hed.open(QIODevice::ReadOnly))) return false;
	
	//HTK uses Latin1 instead of UTF-8 :/
	treeHed.write("RO 100 \""+tempDir.toUtf8()+"/stats\"\n");
	
	//copy tree1.hed content to tree.hed
	treeHed.write(tree1Hed.readAll());
	
	tree1Hed.close();
	
	QString command = "TB";
	int threshold = 350;
	QFile hmmlist(tempDir+"/monophones0");
	if (!hmmlist.open(QIODevice::ReadOnly)) return false;
	
	QStringList phonemeList;
	while (!hmmlist.atEnd())
	{ phonemeList << hmmlist.readLine().trimmed(); }
	hmmlist.close();
	
	for (int i=0; i < phonemeList.count(); i++)
		treeHed.write(QString("%1 %2 \"ST_%3_2_\" {(\"%3\",\"*-%3+*\",\"%3+*\",\"*-%3\").state[2]}\n").arg(command).arg(threshold).arg(phonemeList[i]).toUtf8());
	
	for (int i=0; i < phonemeList.count(); i++)
		treeHed.write(QString("%1 %2 \"ST_%3_3_\" {(\"%3\",\"*-%3+*\",\"%3+*\",\"*-%3\").state[3]}\n").arg(command).arg(threshold).arg(phonemeList[i]).toUtf8());
	
	for (int i=0; i < phonemeList.count(); i++)
		treeHed.write(QString("%1 %2 \"ST_%3_4_\" {(\"%3\",\"*-%3+*\",\"%3+*\",\"*-%3\").state[4]}\n").arg(command).arg(threshold).arg(phonemeList[i]).toUtf8());	
	
	treeHed.write(QString(" \nTR 1\n \nAU "+htkIfyPath(tempDir)+"/fulllist \nCO "+htkIfyPath(tempDir)+"/tiedlist \n \nST "+htkIfyPath(tempDir)+"/trees \n").toUtf8());
	
	treeHed.close();
	
	return true;
}

bool ModelCompilationManager::buildHMM()
{
	if (!createMonophones()) return false;
	if (!fixSilenceModel()) return false;
	if (!realign()) return false;
	if (!makeTriphones()) return false;
	if (!tieStates()) return false;

	if (QFile::exists(hmmDefsPath))
		if (!QFile::remove(hmmDefsPath)) return false;
	if (!QFile::copy(tempDir+"/hmm15/hmmdefs", hmmDefsPath))
		return false;

	if (QFile::exists(tiedListPath))
		if (!QFile::remove(tiedListPath)) return false;
	if (!QFile::copy(tempDir+"/tiedlist", tiedListPath))
		return false;

	return true;
}

bool ModelCompilationManager::makeTriphones()
{
	if (!keepGoing) return false;
	emit status(i18n("Generating triphone..."),1380);
	if (!execute('"'+hLEd+"\" -A -D -T 1 -n \""+htkIfyPath(tempDir)+"/triphones1\" -l * -i \""+htkIfyPath(tempDir)+"/wintri.mlf\" \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/mktri.led"))+"\" \""+htkIfyPath(tempDir)+"/aligned.mlf\""))
	{
		analyseError(i18n("Could not generate triphones.\n\nPlease check your path to the files mktri.led and HLEd (%1, %2)", KStandardDirs::locate("data", "simon/scripts/mktri.led"), hLEd));
		return false;
	}
	
	if (!keepGoing) return false;
	emit status(i18n("Generating mktri.hed..."),1400);
	if (!makeMkTriHed())
	{
		analyseError(i18n("Could not generate mktri.hed"));
		return false;
	}
	
	if (!keepGoing) return false;
	emit status(i18n("Generating hmm10..."),1470);
	if (!buildHMM10())
	{
		analyseError(i18n("Could not generate HMM10. Please check th e path to HHEd (%1).", hHEd));
		return false;
	}
	
	if (!keepGoing) return false;
	emit status(i18n("Generating hmm11..."),1550);
	if (!buildHMM11())
	{
		analyseError(i18n("Could not generate HMM11. Please check your paths to HERest (%1) and to the config (%2).", hERest, KStandardDirs::locate("data", "simon/scripts/config")));
		return false;
	}
	
	if (!keepGoing) return false;
	emit status(i18n("Generating hmm12..."),1620);
	if (!buildHMM12())
	{
		analyseError(i18n("Could not generate the HMM12.\n\nPlease check the path to HERest (%1), to the config (%2) and to the stats-file (%3).", hERest, KStandardDirs::locate("data", "simon/scripts/config"), tempDir+"/stats"));
		return false;
	}
	
	return true;
}


bool ModelCompilationManager::buildHMM12()
{
	return execute('"'+hERest+"\" -A -D -T 1 -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -I \""+htkIfyPath(tempDir)+"/wintri.mlf\" -t 250.0 150.0 3000.0 -s \""+htkIfyPath(tempDir)+"/stats\" -S \""+htkIfyPath(tempDir)+"/train.scp\" -H \""+htkIfyPath(tempDir)+"/hmm11/macros\" -H \""+htkIfyPath(tempDir)+"/hmm11/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm12/"+"\" \""+htkIfyPath(tempDir)+"/triphones1"+"\"");
}


bool ModelCompilationManager::buildHMM11()
{
	QString execStr = '"'+hERest+"\" -A -D -T 1 -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -I \""+htkIfyPath(tempDir)+"/wintri.mlf\" -t 250.0 150.0 3000.0 -S \""+htkIfyPath(tempDir)+"/train.scp\" -H \""+htkIfyPath(tempDir)+"/hmm10/macros\" -H \""+htkIfyPath(tempDir)+"/hmm10/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm11/\" \""+htkIfyPath(tempDir)+"/triphones1\"";
	return execute(execStr);
}


bool ModelCompilationManager::buildHMM10()
{
	return execute('"'+hHEd+"\" -A -D -T 1 -H \""+htkIfyPath(tempDir)+"/hmm9/macros\" -H \""+htkIfyPath(tempDir)+"/hmm9/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm10/\" \""+htkIfyPath(tempDir)+"/mktri.hed\" \""+htkIfyPath(tempDir)+"/monophones1\"");
}


bool ModelCompilationManager::makeMkTriHed()
{
	QFile monophones1(tempDir+"/monophones1");
	if (!monophones1.open(QIODevice::ReadOnly))
		return false;
		
	
	QFile mktriHed(tempDir+"/mktri.hed");
	if (!mktriHed.open(QIODevice::WriteOnly)) return false;
	
	mktriHed.write("CL "+htkIfyPath(tempDir).toUtf8()+"/triphones1\n");
	QByteArray phone="";
	while (!monophones1.atEnd())
	{
		phone = monophones1.readLine(150).trimmed();
		mktriHed.write("TI T_"+phone+" {(*-"+phone+"+*,"+phone+"+*,*-"+phone+").transP}\n");
	}
	
	monophones1.close();
	mktriHed.close();
	return true;
}


bool ModelCompilationManager::buildHMM9()
{
	return execute('"'+hERest+"\" -A -D -T 1 -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -I \""+htkIfyPath(tempDir)+"/aligned.mlf\" -t 250.0 150.0 3000.0 -S \""+htkIfyPath(tempDir)+"/train.scp\" -H \""+htkIfyPath(tempDir)+"/hmm8/macros\" -H \""+htkIfyPath(tempDir)+"/hmm8/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm9/"+"\" \""+htkIfyPath(tempDir)+"/monophones1\"");
}


bool ModelCompilationManager::buildHMM8()
{
	return execute('"'+hERest+"\" -A -D -T 1 -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -I \""+htkIfyPath(tempDir)+"/aligned.mlf\" -t 250.0 150.0 3000.0 -S \""+htkIfyPath(tempDir)+"/train.scp\" -H \""+htkIfyPath(tempDir)+"/hmm7/macros\" -H \""+htkIfyPath(tempDir)+"/hmm7/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm8/\" \""+htkIfyPath(tempDir)+"/monophones1\"");
}

bool ModelCompilationManager::realignHMM7()
{
	return execute('"'+hVite+"\" -A -D -T 1 -l \"*\"  -o SWT -b silence -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -H \""+htkIfyPath(tempDir)+"/hmm7/macros\" -H \""+htkIfyPath(tempDir)+"/hmm7/hmmdefs\" -i \""+htkIfyPath(tempDir)+"/aligned.mlf\" -m -t 250.0 150.0 1000.0 -y lab -a -I \""+htkIfyPath(tempDir)+"/words.mlf\" -S \""+htkIfyPath(tempDir)+"/train.scp\" \""+htkIfyPath(tempDir)+"/dict1\" \""+htkIfyPath(tempDir)+"/monophones1\"");
}

bool ModelCompilationManager::makeDict1()
{
	QFile::remove(tempDir+"/dict1");
	if (!QFile::copy(tempDir+"/dict", tempDir+"/dict1")) return false;
	QFile dict1(tempDir+"/dict1");
	if (!dict1.open(QIODevice::WriteOnly|QIODevice::Append)) return false;

	dict1.write("silence  []  sil\n");
	dict1.close();
	return true;
}

bool ModelCompilationManager::buildHMM7()
{
	return execute('"'+hERest+"\" -A -D -T 1 -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -I \""+htkIfyPath(tempDir)+"/phones1.mlf\" -t 250.0 150.0 3000.0 -S \""+htkIfyPath(tempDir)+"/train.scp\" -H \""+htkIfyPath(tempDir)+"/hmm6/macros\" -H \""+htkIfyPath(tempDir)+"/hmm6/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm7/\" \""+htkIfyPath(tempDir)+"/monophones1\"");
}

bool ModelCompilationManager::buildHMM6()
{
	return execute('"'+hERest+"\" -A -D -T 1 -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -I \""+htkIfyPath(tempDir)+"/phones1.mlf\" -t 250.0 150.0 3000.0 -S \""+htkIfyPath(tempDir)+"/train.scp\" -H \""+htkIfyPath(tempDir)+"/hmm5/macros\" -H \""+htkIfyPath(tempDir)+"/hmm5/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm6/\" \""+htkIfyPath(tempDir)+"/monophones1\"");
}

bool ModelCompilationManager::buildHMM5()
{
	return execute('"'+hHEd+"\" -A -D -T 1 -H \""+htkIfyPath(tempDir)+"/hmm4/macros\" -H \""+htkIfyPath(tempDir)+"/hmm4/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm5/\" \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/sil.hed"))+"\" \""+htkIfyPath(tempDir)+"/monophones1\"");
}

bool ModelCompilationManager::buildHMM4()
{
	QFile::copy(tempDir+"/hmm3/macros", tempDir+"/hmm4/macros");

	QStringList  tmp2;

	QFile hmmdefs3(tempDir+"/hmm3/hmmdefs");
	if (!hmmdefs3.open(QIODevice::ReadOnly)) return false;
	QFile hmmdefs4(tempDir+"/hmm4/hmmdefs");
	if (!hmmdefs4.open(QIODevice::WriteOnly)) return false;

	QByteArray line;
	while (!hmmdefs3.atEnd())
	{
		line = hmmdefs3.readLine(3000);
		if (line.contains("\"sil\""))
		{
			while ((line != "<ENDHMM>\n") && (true /*!hmmdefs3.atEnd()*/))
			{
				hmmdefs4.write(line);
				tmp2 << line;
				line = hmmdefs3.readLine(3000);
			}
			hmmdefs4.write(line);
			hmmdefs4.write(tmp2[0].replace("~h \"sil\"", "~h \"sp\"").toUtf8());
			hmmdefs4.write(tmp2[1].toUtf8());
			hmmdefs4.write(tmp2[2].replace('5', '3').toUtf8());
			hmmdefs4.write(tmp2[9].replace('3', '2').toUtf8());
			hmmdefs4.write(tmp2[10].toUtf8());
			hmmdefs4.write(tmp2[11].toUtf8());
			hmmdefs4.write(tmp2[12].toUtf8());
			hmmdefs4.write(tmp2[13].toUtf8());
			hmmdefs4.write(tmp2[14].toUtf8());
			hmmdefs4.write(tmp2[21].replace('5', '3').toUtf8());
			hmmdefs4.write("0.000000e+000 1.000000e+000 0.000000e+000\n");
			hmmdefs4.write("0.000000e+000 0.900000e+000 0.100000e+000\n");
			hmmdefs4.write("0.000000e+000 0.000000e+000 0.000000e+000\n");
		}
		hmmdefs4.write(line);
	}
	return true;
}

bool ModelCompilationManager::buildHMM3()
{
	return execute('"'+hERest+"\" -A -D -T 1 -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -I \""+htkIfyPath(tempDir)+"/phones0.mlf\" -t 250.0 150.0 1000.0 -S \""+htkIfyPath(tempDir)+"/train.scp\" -H \""+htkIfyPath(tempDir)+"/hmm2/macros\" -H \""+htkIfyPath(tempDir)+"/hmm2/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm3/\" \""+htkIfyPath(tempDir)+"/monophones0\"");
}

bool ModelCompilationManager::buildHMM2()
{
	return execute('"'+hERest+"\" -A -D -T 1 -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -I \""+htkIfyPath(tempDir)+"/phones0.mlf\" -t 250.0 150.0 1000.0 -S \""+htkIfyPath(tempDir)+"/train.scp\" -H \""+htkIfyPath(tempDir)+"/hmm1/macros\" -H \""+htkIfyPath(tempDir)+"/hmm1/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm2/\" \""+htkIfyPath(tempDir)+"/monophones0\"");
}

bool ModelCompilationManager::buildHMM1()
{
	QString execStr = '"'+hERest+"\" -A -D -T 1 -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -I \""+htkIfyPath(tempDir)+"/phones0.mlf\" -t 250.0 150.0 1000.0 -S \""+htkIfyPath(tempDir)+"/train.scp\" -H \""+htkIfyPath(tempDir)+"/hmm0/macros\" -H \""+htkIfyPath(tempDir)+"/hmm0/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm1/\" \""+htkIfyPath(tempDir)+"/monophones0\"";
	return execute(execStr);
}

bool ModelCompilationManager::buildHMM0()
{
	if (!execute('"'+hCompV+"\" -A -D -T 1 -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -f 0.01 -m -S \""+htkIfyPath(tempDir)+"/train.scp\" -M \""+htkIfyPath(tempDir)+"/hmm0/\" \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/proto"))+'"'))
		return false;

	QString protoBody="";
	QString protoHead="";
	QString line;
	QFile protoFile(tempDir+"/hmm0/proto");
	if (!protoFile.open(QIODevice::ReadOnly)) return false;
	
	//extracting proto
	QString protoPreamble="";
	while (!protoFile.atEnd())
	{
		line = protoFile.readLine(3000);
		if (line.startsWith("~h")) { protoHead = line; break; }
		else protoPreamble += line;
	}
	while (!protoFile.atEnd())
		protoBody += protoFile.readLine(3000);
	protoFile.close();

	QStringList monophones;
	QFile monophones0(tempDir+"/monophones0");
	if (!monophones0.open(QIODevice::ReadOnly)) return false;

	while (!monophones0.atEnd())
		monophones.append(monophones0.readLine(50).trimmed());
	monophones0.close();
	
	Logger::log(i18n("[INF] Used Monophones of the model: %1", monophones.join(", ")));

	QFile hmmdefs(tempDir+"/hmm0/hmmdefs");
	if (!hmmdefs.open(QIODevice::WriteOnly)) return false;
	QString phoneHead;
	QString currentHead="";
	for (int i=0; i < monophones.count(); i++)
	{
		currentHead = protoHead;
		hmmdefs.write(currentHead.replace("proto", monophones[i]).toUtf8());
		hmmdefs.write(protoBody.toUtf8());
	}
	hmmdefs.close();

	//generating macros
	QFile macros(tempDir+"/hmm0/macros");
	if (!macros.open(QIODevice::WriteOnly)) return false;
	macros.write(protoPreamble.toUtf8());

	QFile vFloors(tempDir+"/hmm0/vFloors");
	if (!vFloors.open(QIODevice::ReadOnly)) return false;
	while (!vFloors.atEnd()) macros.write(vFloors.readLine(1000));
	vFloors.close();

	macros.close();

	return true;
}

bool ModelCompilationManager::makeMonophones()
{
	QString latinLexiconpath = htkIfyPath(tempDir)+"/lexicon";
	if (QFile::exists(latinLexiconpath))
		if (!QFile::remove(latinLexiconpath)) return false;

#ifdef Q_OS_WIN
	QFile utfLexicon(lexiconPath);
		
	QFile latinLexicon(latinLexiconpath);
	if (!utfLexicon.open(QIODevice::ReadOnly) || !latinLexicon.open(QIODevice::WriteOnly))
		return false;
	
	while (!utfLexicon.atEnd())
	{
		QByteArray utfByte = utfLexicon.readLine(3000);
		QByteArray latinByte;
		latinByte = (QString::fromUtf8(utfByte)).toLatin1();
		latinLexicon.write(latinByte);
	}
	
	utfLexicon.close();
	latinLexicon.close();
#else
	if (!QFile::copy(lexiconPath, latinLexiconpath)) return false;
#endif
	
	fprintf(stderr, "Temp dir in makemonophones(): %s\n", tempDir.toUtf8().data());

	//make monophones1
	QString execStr = '"'+hDMan+"\" -A -D -T 1 -m -w \""+htkIfyPath(tempDir)+"/wlist\" -g \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/global.ded"))+"\" -n \""+htkIfyPath(tempDir)+"/monophones1\" -i \""+htkIfyPath(tempDir)+"/dict\" \""+htkIfyPath(tempDir)+"/lexicon\"";
	kDebug() << execStr;
	exit(0);
	if (!execute(execStr)) return false;

	//make monophones0
	//ditch the "sp" phoneme
	QFile monophones1(tempDir+"/monophones1");
	QFile monophones0(tempDir+"/monophones0");
	if (!monophones1.open(QIODevice::ReadOnly))
		return false;
	if (!monophones0.open(QIODevice::WriteOnly|QIODevice::Truncate))
		return false;

	QString phoneme;
	while (!monophones1.atEnd())
	{
		phoneme = monophones1.readLine(50);
		if ((phoneme.trimmed() != "sp") && (!phoneme.trimmed().isEmpty()))
			monophones0.write(phoneme.toUtf8());
	}
	monophones1.close();
	monophones0.close();
	return true;
}

bool ModelCompilationManager::generateWlist()
{
	QFile promptsFile(promptsPath);
	if (!promptsFile.open(QIODevice::ReadOnly))
		return false;
	
	QStringList words;
	QStringList lineWords;
	QString line;
	while (!promptsFile.atEnd())
	{
		line = QString::fromUtf8(promptsFile.readLine(3000));
		lineWords = line.split(QRegExp("( |\n)"), QString::SkipEmptyParts);
		lineWords.removeAt(0); //ditch the file-id
		words << lineWords;
	}
	promptsFile.close();
	
	words << "SENT-END" << "SENT-START";
	words.sort();
	
	//remove doubles
	int i=1;
	while (i < words.count())
	{
		if (words[i] == words[i-1])
			words.removeAt(i);
		else i++;
	}
	QFile wlistFile(tempDir+"/wlist");
	if (!wlistFile.open(QIODevice::WriteOnly))
		return false;
	for (int i=0; i < words.count(); i++)
	{
		wlistFile.write(words[i].toLocal8Bit()+'\n');
	}
	wlistFile.close();
	return true;
}

bool ModelCompilationManager::generateMlf()
{
	QFile promptsFile(promptsPath);
	QFile mlf(tempDir+"/words.mlf");

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

bool ModelCompilationManager::adaptBaseModel()
{
	if (!keepGoing) return false;
	emit status(i18n("Re-Aligninging to base model..."), 550);
	if (!realignToBaseModel())
	{
		analyseError(i18n("Couldn't re-align model to the selected base model.\n\nThis error is often caused when you are trying to adapt to a model with a different phoneme set. Ideally you should use the same dictionary as the basemodel.\n\nThis error also occurs if you have no training data to adapt to!"));
		return false;
	}
	emit status(i18n("Generating regtree.hed..."), 900);
	if (!makeRegTreeHed())
	{
		analyseError(i18n("Couldn't create regtree.hed."));
		return false;
	}
	emit status(i18n("Creating regression class tree..."), 950);
	if (!createRegressionClassTree())
	{
		analyseError(i18n("Couldn't create regression class tree. Please check your paths to HHEd (%1).", hHEd));
		return false;
	}
	emit status(i18n("Performing static adaption..."), 1500);
	if (!staticAdaption())
	{
		analyseError(i18n("Couldn't adapt the base model. Please check your paths to HERest (%1).", hERest));
		return false;
	}

	emit status(i18n("Switching to new model..."), 1500);

	if ( (((QFile::exists(hmmDefsPath)) && (!QFile::remove(hmmDefsPath))) ||
			(!QFile::copy(tempDir+"/xforms/basehmmdefs.mfc.mllr2", hmmDefsPath))) || 
		 (((QFile::exists(tiedListPath)) && (!QFile::remove(tiedListPath))) ||
			(!QFile::copy(baseTiedlistPath, tiedListPath)))) {
		analyseError(i18n("Couldn't switch to new model."));
		return false;
	}

	return true;
}

bool ModelCompilationManager::realignToBaseModel()
{
	return execute('"'+hVite+"\" -A -D -T 1 -l \"*\"  -o SWT -b SENT-END -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -H \""+htkIfyPath(baseMacrosPath)+"\" -H \""+htkIfyPath(baseHmmDefsPath)+"\" -i \""+htkIfyPath(tempDir)+"/adaptPhones.mlf\" -m -t 250.0 150.0 1000.0 -y lab -a -I \""+htkIfyPath(tempDir)+"/words.mlf\" -S \""+htkIfyPath(tempDir)+"/train.scp\" \""+htkIfyPath(tempDir)+"/dict\" \""+htkIfyPath(baseTiedlistPath)+"\"");
}

bool ModelCompilationManager::createRegressionClassTree()
{
	return execute('"'+hHEd+"\" -H \""+htkIfyPath(baseMacrosPath)+"\" -H \""+htkIfyPath(baseHmmDefsPath)+"\" -M \""+htkIfyPath(tempDir)+"/classes/\" \""+htkIfyPath(tempDir)+"/regtree.hed\" \""+htkIfyPath(baseTiedlistPath)+"\"");
}


bool ModelCompilationManager::makeRegTreeHed()
{	
	QFile::remove(tempDir+"/regtree.hed");

	QFile regTreeHed(tempDir+"/regtree.hed");
	
	if (!regTreeHed.open(QIODevice::WriteOnly)) return false;
	
	//RN "models"
	//LS "stats"
	//RC 32 "rtree"
	regTreeHed.write("RN \"models\"\n");
	regTreeHed.write("LS \""+baseStatsPath.toUtf8()+"\"\n");
	regTreeHed.write("RC 32 \"rtree\"\n");
	regTreeHed.close();
	
	return true;
}



bool ModelCompilationManager::staticAdaption()
{
	QString staticAdaptionPath = tempDir+QDir::separator()+"classes/global";
	if (QFile::exists(staticAdaptionPath) &&
		!QFile::remove(staticAdaptionPath))
		return false;
	if (!QFile::copy(KStandardDirs::locate("data", "simon/scripts/global"), staticAdaptionPath)) return false;

	QString adaptFromHMM = htkIfyPath(tempDir)+"/classes/basehmmdefs";
	QString adaptFromTiedlist = baseTiedlistPath;
	QString adaptFromMacros = htkIfyPath(tempDir)+"classes/basemacros";

	if (!execute('"'+hERest+"\" -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config.global"))+"\" -I \""+htkIfyPath(tempDir)+"/adaptPhones.mlf\" -S \""+htkIfyPath(tempDir)+"/train.scp\" -H \""+adaptFromMacros+"\" -u a -J \""+htkIfyPath(tempDir)+"/classes\" -K \""+htkIfyPath(tempDir)+"/xforms\" mllr1 -H \""+adaptFromHMM+"\" \""+adaptFromTiedlist+"\""))
		return false;

	if (!execute('"'+hERest+"\" -a -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config"))+"\" -C \""+htkIfyPath(KStandardDirs::locate("data", "simon/scripts/config.rc"))+"\" -I \""+htkIfyPath(tempDir)+"/adaptPhones.mlf\" -S \""+htkIfyPath(tempDir)+"/train.scp\" -H \""+adaptFromMacros+"\" -u a -J \""+htkIfyPath(tempDir)+"/xforms\" mllr1 -J \""+htkIfyPath(tempDir)+"/classes\" -K \""+htkIfyPath(tempDir)+"/xforms\" mllr2 -H \""+adaptFromHMM+"\" \""+adaptFromTiedlist+"\""))
		return false;

	return true;
}


ModelCompilationManager::~ModelCompilationManager()
{
}

