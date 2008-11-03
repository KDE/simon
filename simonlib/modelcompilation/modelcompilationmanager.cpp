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
#include <QCoreApplication>
// #include <KMessageBox>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QDebug>
#include <KStandardDirs>
#include <KLocalizedString>
// #include <KDebug>

#include <simonlogging/logger.h>

#include "modelcompilationconfiguration.h"



ModelCompilationManager::ModelCompilationManager(const QString& userName, const QString& samplePath,
			     const QString& lexiconPath, const QString& grammarPath, 
			     const QString& vocabPath, const QString& promptsPath, 
			     const QString& treeHedPath, const QString& wavConfigPath, 
			     const QString& hmmDefsPath, const QString& tiedListPath,
			     const QString& dictPath, const QString& dfaPath, QObject *parent) : QThread(parent)
{
	this->userName = userName;
	this->samplePath = samplePath;
	
	this->lexiconPath = lexiconPath;
	this->grammarPath = grammarPath;
	this->vocabPath = vocabPath;
	this->promptsPath = promptsPath;
	this->treeHedPath = treeHedPath;
	this->wavConfigPath = wavConfigPath;
	
	this->hmmDefsPath = hmmDefsPath;
	this->tiedListPath = tiedListPath;
	this->dictPath = dictPath;
	this->dfaPath = dfaPath;
	
	connect(this, SIGNAL(error(QString)), this, SLOT(processError(QString)));
	
	proc=0;
}

bool ModelCompilationManager::startCompilation()
{
	if (isRunning()) {
		terminate();
		wait();
	}
	lastError="";
	start();
	return true;
}

void ModelCompilationManager::run()
{
	if (proc) proc->deleteLater();
	
	proc = new QProcess();
	proc->setWorkingDirectory(QCoreApplication::applicationDirPath());
	connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(logInfo()));
	connect(this, SIGNAL(finished()), proc, SLOT(deleteLater()));

	Logger::log(i18n("[INF] Modell wird generiert..."));
	emit status(i18n("Vorbereitung"), 0,2300);
	

	if (!generateInputFiles()) return;
	if (!makeTranscriptions()) return;
	if (!codeAudioData()) return;
	if (!buildHMM()) return;
	if (!compileGrammar()) return;

	//sync model
	
	emit status(i18n("Fertig."), 2300, 2300);
	emit modelCompiled();
}


bool ModelCompilationManager::compileGrammar()
{
	emit status(i18n("Generiere Umkehr-Grammatik..."), 2000);
	if (!generateReverseGrammar())
	{
		emit error(i18n("Konnte Umkehr-Grammatik nicht erstellen.\n\nIst eine Grammatik definiert?"));
		return false;
	}

	emit status(i18n("Generiere temporäre Vokabeln..."), 2100);
	if (!makeTempVocab())
	{
		emit error(i18n("Konnte Temporäre Vokabeln nicht erstellen.\n\nBitte überprüfen Sie die Pfade zur Vokabulardatei (%1).", this->vocabPath));
		return false;
	}
	

	emit status(i18n("Generiere DFA..."), 2250);
	if (!makeDfa())
	{
		emit error(i18n("Konnte dfa nicht generieren.\n\nBitte überprüfen Sie die Pfade zur mkfa und dfa_minimize Datei (%1, %2).", ModelCompilationConfiguration::mkfa().path(), ModelCompilationConfiguration::dfa_minimize().path()));
		return false;
	}
	
	emit status(i18n("Generiere Grammatikalisches Wörterbuch..."), 2299);
	if (!generateDict())
	{
		emit error(i18n("Konnte das grammatikalische Wörterbuch nicht generieren. \nBitte überprüfen Sie die Pfade zur Ausgabedatei. (%1).", dictPath));
		return false;
	}
	
	return true;
}

bool ModelCompilationManager::makeTempVocab()
{
	QFile vocab ( vocabPath );

	QString terminal;
	if ( !vocab.open ( QFile::ReadOnly ) ) return false;
	QFile tmpVocab ( KStandardDirs::locateLocal("tmp", userName+"/tempvoca") );
	if ( !tmpVocab.open ( QFile::WriteOnly ) ) return false;

	QFile term ( KStandardDirs::locateLocal("tmp", userName+"/term") );
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
	QString mkfa = ModelCompilationConfiguration::mkfa().path();
	QString dfaMinimize= ModelCompilationConfiguration::dfa_minimize().path();

	QString execStr = '"'+mkfa+"\" -e1 -fg \""+KStandardDirs::locateLocal("tmp", userName+"/reverseGrammar")+"\" -fv \""+KStandardDirs::locateLocal("tmp", userName+"/tempvoca")+"\" -fo \""+KStandardDirs::locateLocal("tmp", userName+"/dfaTemp.tmp")+"\" -fh \""+KStandardDirs::locateLocal("tmp", userName+"/dfaTemp.h")+"\"";
// 	kDebug() << execStr;
	proc->start(execStr);
	proc->waitForFinished(-1);
	if (proc->exitCode() != 0) 
		return false;

	execStr = '"'+dfaMinimize+'"'+" \""+KStandardDirs::locateLocal("tmp", userName+"/dfaTemp.tmp")+"\" -o \""+dfaPath+'"';
// 	kDebug() << execStr;
	proc->start(execStr);
	proc->waitForFinished(-1);
	if (proc->exitCode()!= 0) 
		return false;

	return true;	
}

bool ModelCompilationManager::generateReverseGrammar()
{
	QFile grammar(grammarPath);
	if (!grammar.open(QFile::ReadOnly)) return false;

	QFile reverseGrammar(KStandardDirs::locateLocal("tmp", userName+"/reverseGrammar"));
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
			
			dict.write(QString(QString::number(nowId)+"\t["+vocabEntry.left(splitter)+"]\t"+vocabEntry.mid(splitter).trimmed()+'\n').toUtf8());
		}
	}
	
	vocab.close();
	dict.close();
	return true;
}

bool ModelCompilationManager::codeAudioData()
{
	emit status(i18n("Kodiere Audiodaten..."), 150);
	
	//creating codetrain
	if (!generateCodetrainScp())
	{
		emit error(i18n("Konnte CodeTrain-Datei nicht erstellen."));
		return false;
	}

	QString codetrainPath = KStandardDirs::locateLocal("tmp", userName+"/codetrain.scp");

	//TODO: implement some sort of caching (maybe with an file/hash combination?)
	proc->start('"'+ModelCompilationConfiguration::hCopy().path()+"\" -A -D -T 1 -C \""+wavConfigPath+"\" -S \""+codetrainPath+'"');
	proc->waitForFinished(-1);
	if (!proc->exitCode()==0)
	{
		emit error(i18n("Fehler beim kodieren der samples! Bitte überprüfen Sie den Pfad zu HCopy (%1) und der wav config (%2)", ModelCompilationConfiguration::hCopy().path(), wavConfigPath));
		return false;
	}

	return true;
}

bool ModelCompilationManager::generateCodetrainScp()
{
	QString codetrainPath = KStandardDirs::locateLocal("tmp", userName+"/codetrain.scp");
	QString trainPath = KStandardDirs::locateLocal("tmp", userName+"/train.scp");

	QDir wavDir(samplePath);
	QStringList wavs = wavDir.entryList(QStringList() << ("*.wav"));
	
	QString pathToMFCs =KStandardDirs::locateLocal("tmp", userName+"/mfcs");
	wavDir.mkpath(pathToMFCs);
	
	QString file;
	QFile scpFile(codetrainPath);
	QFile trainScpFile(trainPath);
	if (!scpFile.open(QIODevice::WriteOnly|QIODevice::Truncate) || !trainScpFile.open(QIODevice::WriteOnly|QIODevice::Truncate))
	{
		return false;
	}

	QString fileBase;
	QString mfcFile;
	

	
	for (int i=0; i < wavs.count();i++)
	{
		file = wavs[i];
		fileBase = file.left(file.lastIndexOf('.'));
		mfcFile = pathToMFCs+'/'+fileBase+".mfc";

		
		scpFile.write(QString('"'+samplePath+'/'+file + "\" \"" + mfcFile +"\"\n").toUtf8());
		trainScpFile.write(mfcFile.toUtf8()+'\n');
	}
	scpFile.close();
	trainScpFile.close();
	return true;
}

bool ModelCompilationManager::generateInputFiles()
{	
	emit status(i18n("Generiere Wordliste..."), 35);
	//wlist
	if (!generateWlist())
	{
		emit error(i18n("Erstellen der Wortliste fehlgeschlagen. Bitte überprüfen Sie die Berechtigungen für den Temporären Pfad."));
		return false;
	}

	//monophones
	emit status(i18n("Erstelle Monophone..."), 40);

	
	if (!makeMonophones())
	{
		emit error(i18n("Erstellen der Monophone fehlgeschlagen. Bitte überprüfen Sie ob das Programm HDMan richtig eingerichtet ist und das Lexicon alle verwendeten Wörter beinhaltet."));
		return false;
	}

	return true;
}

bool ModelCompilationManager::makeTranscriptions()
{
	//mlf
	emit status(i18n("Erstelle Master Label File..."), 55);
	if (!generateMlf())
	{
		emit error(i18n("Erstellen der Master Label File fehlgeschlagen. Bitte überprüfen Sie die prompts-Datei (%1)", promptsPath));
		return false;
	}
	
	
	proc->start('"'+ModelCompilationConfiguration::hLEd().path()+"\" -A -D -T 1 -l \"*\" -d \""+KStandardDirs::locateLocal("tmp", userName+"/dict")+"\" -i \""+KStandardDirs::locateLocal("tmp", userName+"/phones0.mlf")+"\" \""+KStandardDirs::locate("appdata", "scripts/mkphones0.led")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/words.mlf")+"\"");
	proc->waitForFinished(-1);
	kDebug() << '"'+ModelCompilationConfiguration::hLEd().path()+"\" -A -D -T 1 -l \"*\" -d \""+KStandardDirs::locateLocal("tmp", userName+"/dict")+"\" -i \""+KStandardDirs::locateLocal("tmp", userName+"/phones0.mlf")+"\" \""+KStandardDirs::locate("appdata", "scripts/mkphones0.led")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/words.mlf")+"\"";
	if (proc->exitCode() ==0)
	{
		proc->start('"'+ModelCompilationConfiguration::hLEd().path()+"\" -A -D -T 1 -l \"*\" -d \""+KStandardDirs::locateLocal("tmp", userName+"/dict")+"\" -i \""+KStandardDirs::locateLocal("tmp", userName+"/phones1.mlf")+"\" \""+KStandardDirs::locate("appdata", "scripts/mkphones1.led")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/words.mlf")+"\"");
		kDebug() << '"'+ModelCompilationConfiguration::hLEd().path()+"\" -A -D -T 1 -l \"*\" -d \""+KStandardDirs::locateLocal("tmp", userName+"/dict")+"\" -i \""+KStandardDirs::locateLocal("tmp", userName+"/phones1.mlf")+"\" \""+KStandardDirs::locate("appdata", "scripts/mkphones1.led")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/words.mlf")+"\"";
		proc->waitForFinished(-1);
	}
	
	
	if (proc->exitCode() != 0)
	{
		emit error(i18n("Erstellen der Transkriptionsdateien fehlgeschlagen. Bitte überprüfen Sie ob Sie den Pfad für die Dateien mkphones0.led und mkphones1.led richtig angegeben haben. (%1, %2)", KStandardDirs::locate("appdata", "scripts/mkphones0.led"), KStandardDirs::locate("appdata", "scripts/mkphones1.led")));
		return false;
	}
	return true;
}

/**
 * \brief Processes an error (reacts on it some way)
 * \author Peter Grasch
 * \param userError A Human-Readable error-description
 * @return If this is true we knew what to do; if this is false you'd better throw an error message
 */
bool ModelCompilationManager::processError(const QString& userError)
{
	//FIXME: completeley broken atm
	//can't control the systems console-charset so we use the Local8Bit setting to import
	//instead of UTF-8
	lastError = QString(lastError+'\n'+QString::fromLocal8Bit(proc->readAllStandardError())).right(400);
	
	Logger::log(i18n("[ERR]")+' '+lastError);
	
	QString err = lastError.trimmed();

	if (err.startsWith(("ERROR [+1232]"))) //word missing
	{
		//ERROR [+1232]  NumParts: Cannot find word DARAUFFOLGEND in dictionary
		int wordstart = 45;
		QString word = lastError.mid(wordstart, lastError.indexOf(' ', wordstart)-wordstart);
		
		//this error ONLY occurs when there are samples for the word but the word itself was not added
		//to the dictionary so - ADD THE WORD!
		emit missingWord(word);
	} else if (err.startsWith("ERROR [+2662]"))
	{
// 		"ERROR [+2662]  FindProtoModel: no proto for E in hSet
		QString phoneme = err.mid(44,err.indexOf(' ', 44)-44);
		//FIXME propagate error:
// 		emit error(i18n("Phonem %1 kommt in den Trainingsdaten nicht vor.\n\nBitte trainieren Sie ein Wort welches das Phonem %1 beinhaltet.\n\nSie können zum Beispiel in der Wortliste ein beliebiges Wort wählen welches diesen Phonem beinhaltet, ihn zu einem speziellen Training hinzufügen und dieses Training dann durchführen.", phoneme));
// 		emit missingPhoneme(phoneme);
	} else
	if (err.startsWith("ERROR [+6510]"))  //sample without prompts-entry
	{ //LOpen: Unable to open label file /path/to/missing-sample.lab
// 		err = err.mid(48); //err.left(err.indexOf("\n"));
// ERROR [+6510]  LOpen: Unable to open label file ./tmp//modeltmp/mfcs/Test_1_2008-03-19_13-45-08_2008-03-24_22-23-22.lab

		err = err.left(err.indexOf('\n'));
		QString label = err.mid(48);
		label = label.mid(label.lastIndexOf('/'));
		QString sampleName = label.left(label.count()-4);
		if (!sampleName.isEmpty())
		{
			QString filename = samplePath+'/'+sampleName+".wav";
			
// 			emit sampleWithoutWord(filename);
		}
	}
	else if (err.contains("Error:       undefined class \""))
	{
		int startIndex = err.indexOf("Error:       undefined class \"") + 30;
		
		QString undefClass = err.mid(startIndex);
		undefClass = undefClass.left(undefClass.indexOf('"'));
		emit unknownGrammarClass(undefClass);
	}

	emit status(i18n("Abgebrochen"), 1, 1);
	return false;
}

bool ModelCompilationManager::createMonophones()
{
	emit status(i18n("Erstelle hmm0..."), 550);
	if (!buildHMM0())
	{
		emit error(i18n("Fehler beim Generieren des HMM0. \n\nBitte überprüfen Sie, ob ausreichend Trainingsmaterial vorhanden ist.\n\nSollten Sie sicher sein, das Modell wurde ausreichend trainiert, überprüfen Sie bitte den Pfad zu HCompV (%1), der config (%2) und des Prototypen (%3).", ModelCompilationConfiguration::hCompV().path(), KStandardDirs::locate("appdata", "scripts/config"), KStandardDirs::locate("appdata", "scripts/proto")));
		return false;
	}
	emit status(i18n("Erstelle hmm1..."), 800);
	if (!buildHMM1())
	{
		emit error(i18n("Fehler beim Generieren des HMM1. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2)", ModelCompilationConfiguration::hERest().path(), KStandardDirs::locate("appdata", "scripts/config")));
		return false;
	}
	emit status(i18n("Erstelle hmm2..."), 850);
	if (!buildHMM2())
	{
		emit error(i18n("Fehler beim Generieren des HMM2. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2)", ModelCompilationConfiguration::hERest().path(), KStandardDirs::locate("appdata", "scripts/config")));
		return false;
	}
	emit status(i18n("Erstelle hmm3..."), 900);
	if (!buildHMM3())
	{
		emit error(i18n("Fehler beim Generieren des HMM3. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2)", ModelCompilationConfiguration::hERest().path(), KStandardDirs::locate("appdata", "scripts/config")));
		return false;
	}
	return true;
}

bool ModelCompilationManager::fixSilenceModel()
{
	emit status(i18n("Erstelle Pausenmodell (hmm4)..."), 950);
	if (!buildHMM4())
	{
		emit error(i18n("Fehler beim Generieren des HMM4. Bitte überprüfen Sie das HMM3"));
		return false;
	}
	emit status(i18n("Erstelle hmm5..."), 1000);
	if (!buildHMM5())
	{
		emit error(i18n("Fehler beim Generieren des HMM5. Bitte überprüfen Sie den Pfad zu HHEd (%1) und  des Silence-Modells (%2)", ModelCompilationConfiguration::hHEd().path(), KStandardDirs::locate("appdata", "scripts/sil.hed")));
		return false;
	}
	emit status(i18n("Erstelle hmm6..."), 1080);
	if (!buildHMM6())
	{
		emit error(i18n("Fehler beim Generieren des HMM6. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2", ModelCompilationConfiguration::hERest().path(), KStandardDirs::locate("appdata", "scripts/config")));
		return false;
	}
	emit status(i18n("Erstelle hmm7..."), 1150);
	if (!buildHMM7())
	{
		emit error(i18n("Fehler beim Generieren des HMM7. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2", ModelCompilationConfiguration::hERest().path(), KStandardDirs::locate("appdata", "scripts/config")));
		return false;
	}
	
	return true;
}

bool ModelCompilationManager::realign()
{
	emit status(i18n("Erstellte dict1..."), 1160);
	if (!makeDict1())
	{
		emit error(i18n("Fehler beim erstellen des dict1"));
		return false;
	}

	emit status(i18n("Hmm7 neu ausrichten..."), 1160);
	if (!realignHMM7())
	{
		emit error(i18n("Konnte HMM7 nicht neu ausrichten. Bitte überprüfen Sie den Pfad zu HVite (%1), der config (%2) und das HMM7.", ModelCompilationConfiguration::hVite().path(), KStandardDirs::locate("appdata", "scripts/config")));
		return false;
	}

	emit status(i18n("Erstelle hmm8..."), 1230);
	if (!buildHMM8())
	{
		emit error(i18n("Fehler beim Generieren des HMM8. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2", ModelCompilationConfiguration::hERest().path(), KStandardDirs::locate("appdata", "scripts/config")));
		return false;
	}

	emit status(i18n("Erstelle hmm9..."),1300);
	if (!buildHMM9())
	{
		emit error(i18n("Fehler beim Generieren des HMM9. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2", ModelCompilationConfiguration::hERest().path(), KStandardDirs::locate("appdata", "scripts/config")));
		return false;
	}
	
	return true;
}

bool ModelCompilationManager::tieStates()
{
	emit status(i18n("Erstelle triphone..."),1700);
	
	proc->start('"'+ModelCompilationConfiguration::hDMan().path()+"\" -A -D -T 1 -b sp -n \""+KStandardDirs::locateLocal("tmp", userName+"/fulllist")+"\" -g \""+KStandardDirs::locate("appdata", "scripts/global.ded")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/dict-tri")+"\" \""+lexiconPath+'"');
	proc->waitForFinished(-1);
	if ((proc->exitCode() != 0))
	{
		emit error(i18n("Konnte Triphone nicht binden. Bitte überprüfen Sie den Pfad zu HDMan (%1), global.ded (%2) und dem Lexikon (%3).", ModelCompilationConfiguration::hDMan().path(), KStandardDirs::locate("appdata", "scripts/global.ded"), lexiconPath));
		return false;
	}

	emit status(i18n("Erstelle Liste der Triphone..."),1705);
	if (!makeFulllist())
	{
		emit error(i18n("Konnte Liste der Triphone nicht erstellen."));
		return false;
	}
	emit status(i18n("Erstelle tree.hed..."), 1750);
	if (!makeTreeHed())
	{
		emit error(i18n("Konnte tree.hed nicht erstellen."));
		return false;
	}
	
	emit status(i18n("Erstelle hmm13..."),1830);
	if (!buildHMM13())
	{
		emit error(i18n("Fehler beim Generieren des HMM13. Bitte überprüfen Sie den Pfad zu HHEd (%1).", ModelCompilationConfiguration::hHEd().path()));
		return false;
	}
	
	
	emit status(i18n("Erstelle hmm14..."),1900);
	if (!buildHMM14())
	{
		emit error(i18n("Fehler beim Generieren des HMM14. Bitte überprüfen Sie den Pfad zu HERest (%1), der config (%2), und die stats-Datei (%3)", ModelCompilationConfiguration::hERest().path(), KStandardDirs::locate("appdata", "scripts/config"), KStandardDirs::locateLocal("tmp", userName+"/stats")));
		return false;
	}
	
	emit status(i18n("Erstelle hmm15..."),1990);
	if (!buildHMM15())
	{
		emit error(i18n("Fehler beim Generieren des HMM15. Bitte überprüfen Sie den Pfad zu HERest (%1), der config (%2), und die stats-Datei (%3)", ModelCompilationConfiguration::hERest().path(), KStandardDirs::locate("appdata", "scripts/config"), KStandardDirs::locateLocal("tmp", userName+"/stats")));
		return false;
	}

	
	return true;
}

bool ModelCompilationManager::buildHMM13()
{
	proc->start('"'+ModelCompilationConfiguration::hHEd().path()+"\" -A -D -T 1 -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm12/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm12/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm13/")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/tree.hed")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/triphones1")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelCompilationManager::buildHMM14()
{
	proc->start('"'+ModelCompilationConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", userName+"/wintri.mlf")+"\" -t 250.0 150.0 3000.0 -s \""+KStandardDirs::locateLocal("tmp", userName+"/stats")+"\" -S \""+KStandardDirs::locateLocal("tmp", userName+"/train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm13/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm13/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm14/")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/tiedlist")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelCompilationManager::buildHMM15()
{
	proc->start('"'+ModelCompilationConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", userName+"/wintri.mlf")+"\" -t 250.0 150.0 3000.0 -s \""+KStandardDirs::locateLocal("tmp", userName+"/stats")+"\" -S \""+KStandardDirs::locateLocal("tmp", userName+"/train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm14/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm14/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm15/")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/tiedlist")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelCompilationManager::makeFulllist()
{	
	QFile::remove(KStandardDirs::locateLocal("tmp", userName+"/fulllist-original"));
	if (!QFile::copy(KStandardDirs::locateLocal("tmp", userName+"/fulllist"), KStandardDirs::locateLocal("tmp", userName+"/fulllist-original"))) return false;
	
	QFile triphones1(KStandardDirs::locateLocal("tmp", userName+"/triphones1"));
	QFile fulllist(KStandardDirs::locateLocal("tmp", userName+"/fulllist"));
	
	
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
	QFile::remove(KStandardDirs::locateLocal("tmp", userName+"/tree.hed"));

// 	if (!QFile::copy(treeHedPath, KStandardDirs::locateLocal("tmp", userName+"/tree.hed"))) return false;
	
	QFile tree1Hed(treeHedPath);
	QFile treeHed(KStandardDirs::locateLocal("tmp", userName+"/tree.hed"));
	
	if ((!treeHed.open(QIODevice::WriteOnly)) || 
		(!tree1Hed.open(QIODevice::ReadOnly))) return false;
	
	//HTK uses Latin1 instead of UTF-8 :/
	treeHed.write("RO 100 \""+KStandardDirs::locateLocal("tmp", userName+"/stats").toUtf8()+"\"\n");
	
	//copy tree1.hed content to tree.hed
	treeHed.write(tree1Hed.readAll());
	
	tree1Hed.close();
	
	QString command = "TB";
	int threshold = 350;
	QFile hmmlist(KStandardDirs::locateLocal("tmp", userName+"/monophones0"));
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
	
	treeHed.write(QString(" \nTR 1\n \nAU "+KStandardDirs::locateLocal("tmp", userName+"/fulllist")+" \nCO "+KStandardDirs::locateLocal("tmp", userName+"/tiedlist")+" \n \nST "+KStandardDirs::locateLocal("tmp", userName+"/trees")+" \n").toUtf8());
	
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
	if (!QFile::copy(KStandardDirs::locateLocal("tmp", userName+"/hmm15/hmmdefs"), hmmDefsPath))
		return false;

	if (QFile::exists(tiedListPath))
		if (!QFile::remove(tiedListPath)) return false;
	if (!QFile::copy(KStandardDirs::locateLocal("tmp", userName+"/tiedlist"), tiedListPath))
		return false;

	return true;
}

bool ModelCompilationManager::makeTriphones()
{
	emit status(i18n("Erstelle triphone..."),1380);
	proc->start('"'+ModelCompilationConfiguration::hLEd().path()+"\" -A -D -T 1 -n \""+KStandardDirs::locateLocal("tmp", userName+"/triphones1")+"\" -l * -i \""+KStandardDirs::locateLocal("tmp", userName+"/wintri.mlf")+"\" \""+KStandardDirs::locate("appdata", "scripts/mktri.led")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/aligned.mlf")+"\"");
	proc->waitForFinished(-1);
	if ((proc->exitCode() != 0))
	{
		emit error(i18n("Erstellen der Triphone files fehlgeschlagen. Bitte überprüfen Sie ob Sie den Pfad für die Datei mktri.led richtig angegeben haben (%1) und überprüfen Sie den Pfad zu HLEd (%2)", KStandardDirs::locate("appdata", "scripts/mktri.led"), ModelCompilationConfiguration::hLEd().path()));
		return false;
	}
	
	emit status(i18n("Erstelle mktri.hed..."),1400);
	if (!makeMkTriHed())
	{
		emit error(i18n("Fehler beim generieren der mktri.hed"));
		return false;
	}
	
	emit status(i18n("Erstelle hmm10..."),1470);
	if (!buildHMM10())
	{
		emit error(i18n("Fehler beim Generieren des HMM10. Bitte überprüfen Sie den Pfad zu HHEd (%1).", ModelCompilationConfiguration::hHEd().path()));
		return false;
	}
	
	emit status(i18n("Erstelle hmm11..."),1550);
	if (!buildHMM11())
	{
		emit error(i18n("Fehler beim Generieren des HMM11. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2)", ModelCompilationConfiguration::hERest().path(), KStandardDirs::locate("appdata", "scripts/config")));
		return false;
	}
	
	emit status(i18n("Erstelle hmm12..."),1620);
	if (!buildHMM12())
	{
		emit error(i18n("Fehler beim Generieren des HMM12. Bitte überprüfen Sie den Pfad zu HERest (%1), der config (%2), und die stats-Datei (%3)", ModelCompilationConfiguration::hERest().path(), KStandardDirs::locate("appdata", "scripts/config"), KStandardDirs::locateLocal("tmp", userName+"/stats")));
		return false;
	}
	
	return true;
}


bool ModelCompilationManager::buildHMM12()
{
	proc->start('"'+ModelCompilationConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", userName+"/wintri.mlf")+"\" -t 250.0 150.0 3000.0 -s \""+KStandardDirs::locateLocal("tmp", userName+"/stats")+"\" -S \""+KStandardDirs::locateLocal("tmp", userName+"/train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm11/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm11/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm12/")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/triphones1")+"\"");

	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelCompilationManager::buildHMM11()
{
	proc->start('"'+ModelCompilationConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", userName+"/wintri.mlf")+"\" -t 250.0 150.0 3000.0 -S \""+KStandardDirs::locateLocal("tmp", userName+"/train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm10/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm10/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm11/")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/triphones1")+"\"");
	
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelCompilationManager::buildHMM10()
{
	proc->start('"'+ModelCompilationConfiguration::hHEd().path()+"\" -A -D -T 1 -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm9/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm9/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm10/")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/mktri.hed")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/monophones1")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelCompilationManager::makeMkTriHed()
{
	QFile monophones1(KStandardDirs::locateLocal("tmp", userName+"/monophones1"));
	if (!monophones1.open(QIODevice::ReadOnly))
		return false;
		
	
	QFile mktriHed(KStandardDirs::locateLocal("tmp", userName+"/mktri.hed"));
	if (!mktriHed.open(QIODevice::WriteOnly)) return false;
	
	mktriHed.write("CL "+KStandardDirs::locateLocal("tmp", userName+"/triphones1").toUtf8()+"\n");
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
	proc->start('"'+ModelCompilationConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", userName+"/aligned.mlf")+"\" -t 250.0 150.0 3000.0 -S \""+KStandardDirs::locateLocal("tmp", userName+"/train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm8/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm8/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm9/")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/monophones1")+"\"");
	
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelCompilationManager::buildHMM8()
{
	proc->start('"'+ModelCompilationConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", userName+"/aligned.mlf")+"\" -t 250.0 150.0 3000.0 -S \""+KStandardDirs::locateLocal("tmp", userName+"/train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm7/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm7/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm8/")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/monophones1")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelCompilationManager::realignHMM7()
{
	proc->start('"'+ModelCompilationConfiguration::hVite().path()+"\" -A -D -T 1 -l *  -o SWT -b silence -C \""+KStandardDirs::locate("appdata", "scripts/config")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm7/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm7/hmmdefs")+"\" -i \""+KStandardDirs::locateLocal("tmp", userName+"/aligned.mlf")+"\" -m -t 250.0 150.0 1000.0 -y lab -a -I \""+KStandardDirs::locateLocal("tmp", userName+"/words.mlf")+"\" -S \""+KStandardDirs::locateLocal("tmp", userName+"/train.scp")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/dict1")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/monophones1")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelCompilationManager::makeDict1()
{
	QFile::remove(KStandardDirs::locateLocal("tmp", userName+"/dict1"));
	if (!QFile::copy(KStandardDirs::locateLocal("tmp", userName+"/dict"), KStandardDirs::locateLocal("tmp", userName+"/dict1"))) return false;
	QFile dict1(KStandardDirs::locateLocal("tmp", userName+"/dict1"));
	if (!dict1.open(QIODevice::WriteOnly|QIODevice::Append)) return false;

	dict1.write("silence  []  sil\n");
	dict1.close();
	return true;
}

bool ModelCompilationManager::buildHMM7()
{
	proc->start('"'+ModelCompilationConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", userName+"/phones1.mlf")+"\" -t 250.0 150.0 3000.0 -S \""+KStandardDirs::locateLocal("tmp", userName+"/train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm6/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm6/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm7/")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/monophones1")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelCompilationManager::buildHMM6()
{
	proc->start('"'+ModelCompilationConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", userName+"/phones1.mlf")+"\" -t 250.0 150.0 3000.0 -S \""+KStandardDirs::locateLocal("tmp", userName+"/train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm5/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm5/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm6/")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/monophones1")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelCompilationManager::buildHMM5()
{
	proc->start('"'+ModelCompilationConfiguration::hHEd().path()+"\" -A -D -T 1 -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm4/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm4/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm5/")+"\" \""+KStandardDirs::locate("appdata", "scripts/sil.hed")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/monophones1")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelCompilationManager::buildHMM4()
{
	QFile::copy(KStandardDirs::locateLocal("tmp", userName+"/hmm3/macros"), KStandardDirs::locateLocal("tmp", userName+"/hmm4/macros"));

	QStringList  tmp2;

	QFile hmmdefs3(KStandardDirs::locateLocal("tmp", userName+"/hmm3/hmmdefs"));
	if (!hmmdefs3.open(QIODevice::ReadOnly)) return false;
	QFile hmmdefs4(KStandardDirs::locateLocal("tmp", userName+"/hmm4/hmmdefs"));
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
	proc->start('"'+ModelCompilationConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", userName+"/phones0.mlf")+"\" -t 250.0 150.0 1000.0 -S \""+KStandardDirs::locateLocal("tmp", userName+"/train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm2/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm2/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm3/")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/monophones0")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelCompilationManager::buildHMM2()
{
	proc->start('"'+ModelCompilationConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", userName+"/phones0.mlf")+"\" -t 250.0 150.0 1000.0 -S \""+KStandardDirs::locateLocal("tmp", userName+"/train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm1/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm1/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm2/")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/monophones0")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelCompilationManager::buildHMM1()
{
	proc->start('"'+ModelCompilationConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", userName+"/phones0.mlf")+"\" -t 250.0 150.0 1000.0 -S \""+KStandardDirs::locateLocal("tmp", userName+"/train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm0/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", userName+"/hmm0/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm1/")+"\" \""+KStandardDirs::locateLocal("tmp", userName+"/monophones0")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelCompilationManager::buildHMM0()
{
	proc->start('"'+ModelCompilationConfiguration::hCompV().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "scripts/config")+"\" -f 0.01 -m -S \""+KStandardDirs::locateLocal("tmp", userName+"/train.scp")+"\" -M \""+KStandardDirs::locateLocal("tmp", userName+"/hmm0/")+"\" \""+KStandardDirs::locate("appdata", "scripts/proto")+'"');
	proc->waitForFinished(-1);
	if (proc->exitCode()!=0) return false;

	QString protoBody="";
	QString protoHead="";
	QString line;
	QFile protoFile(KStandardDirs::locateLocal("tmp", userName+"/hmm0/proto"));
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
	QFile monophones0(KStandardDirs::locateLocal("tmp", userName+"/monophones0"));
	if (!monophones0.open(QIODevice::ReadOnly)) return false;

	while (!monophones0.atEnd())
		monophones.append(monophones0.readLine(50).trimmed());
	monophones0.close();
	
	Logger::log(i18n("[INF] Verwendete Monophone des Modells: %1", monophones.join(", ")));

	QFile hmmdefs(KStandardDirs::locateLocal("tmp", userName+"/hmm0/hmmdefs"));
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
	QFile macros(KStandardDirs::locateLocal("tmp", userName+"/hmm0/macros"));
	if (!macros.open(QIODevice::WriteOnly)) return false;
	macros.write(protoPreamble.toUtf8());

	QFile vFloors(KStandardDirs::locateLocal("tmp", userName+"/hmm0/vFloors"));
	if (!vFloors.open(QIODevice::ReadOnly)) return false;
	while (!vFloors.atEnd()) macros.write(vFloors.readLine(1000));
	vFloors.close();

	macros.close();

	return true;
}

bool ModelCompilationManager::makeMonophones()
{
	//make monophones1
	proc->start('"'+ModelCompilationConfiguration::hDMan().path()+"\" -A -D -T 1 -m -w \""+KStandardDirs::locateLocal("tmp", userName+"/wlist")+"\" -g \""+KStandardDirs::locate("appdata", "scripts/global.ded")+"\" -n \""+KStandardDirs::locateLocal("tmp", userName+"/monophones1")+"\" -i \""+KStandardDirs::locateLocal("tmp", userName+"/dict")+"\" \""+lexiconPath+'"');
	proc->waitForFinished(-1);
	if (proc->exitStatus()!=0) return false;

	//make monophones0
	//ditch the "sp" phoneme

	QFile monophones1(KStandardDirs::locateLocal("tmp", userName+"/monophones1"));
	QFile monophones0(KStandardDirs::locateLocal("tmp", userName+"/monophones0"));
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

void ModelCompilationManager::logInfo()
{
	lastOutput = proc->readAllStandardOutput();
	Logger::log(i18n("[INF]")+' '+lastOutput);
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
		line = promptsFile.readLine(3000);
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
	QFile wlistFile(KStandardDirs::locateLocal("tmp", userName+"/wlist"));
	if (!wlistFile.open(QIODevice::WriteOnly))
		return false;
	for (int i=0; i < words.count(); i++)
	{
		wlistFile.write(words[i].toUtf8()+'\n');
	}
	wlistFile.close();
	return true;
}

bool ModelCompilationManager::generateMlf()
{
	QFile promptsFile(promptsPath);
	QFile mlf(KStandardDirs::locateLocal("tmp", userName+"/words.mlf"));

	if (!promptsFile.open(QIODevice::ReadOnly))
		return false;
	if (!mlf.open(QIODevice::WriteOnly))
		return false;
	
	mlf.write("#!MLF!#\n");
	QStringList lineWords;
	QString line;
	while (!promptsFile.atEnd())
	{
		line = promptsFile.readLine(3000);
		if (line.trimmed().isEmpty()) continue;
		lineWords = line.split(QRegExp("( |\n)"), QString::SkipEmptyParts);
		QString labFile = "\"*/"+lineWords.takeAt(0)+".lab\""; //ditch the file-id
		mlf.write(labFile.toUtf8()+"\n");
		for (int i=0; i < lineWords.count(); i++)
			mlf.write(lineWords[i].toUtf8()+"\n");
		mlf.write(".\n");
	}
	promptsFile.close();
	mlf.close();
	return true;
}

ModelCompilationManager::~ModelCompilationManager()
{
	proc->deleteLater();
}

