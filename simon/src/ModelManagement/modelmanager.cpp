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

#include "modelmanager.h"
#include <QCoreApplication>
#include <KMessageBox>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QDebug>
#include <KLocalizedString>

#include <simonlogging/logger.h>

#include "WordList/wordlistmanager.h"
#include "Grammar/grammarmanager.h"
#include "coreconfiguration.h"


ModelManager* ModelManager::instance;


ModelManager::ModelManager(QWidget *parent) : QThread(parent)
{
	connect(this, SIGNAL(error(QString)), this, SLOT(processError(QString)));
}


bool ModelManager::compileGrammar()
{
	emit status(i18n("Generiere Umkehr-Grammatik..."));
	if (!generateReverseGrammar())
	{
		emit error(i18n("Konnte Umkehr-Grammatik nicht erstellen.\n\nIst eine Grammatik definiert?"));
		return false;
	}
	
	emit progress(2050);
	emit status(i18n("Entferne unverwendete Vokabeln..."));
	if (!makeSimpleVocab())
	{
		emit error(i18n("Konnte simple Vokabeln nicht erstellen.\n\nBitte überprüfen Sie ob der die Wortliste richtig geladen wurde."));
		return false;
	}
	emit progress(2090);
	

	emit status(i18n("Generiere temporäre Vokabeln..."));
	if (!makeTempVocab())
	{
		emit error(i18n("Konnte Temporäre Vokabeln nicht erstellen.\n\nBitte überprüfen Sie die Pfade zur Vokabulardatei (%1).", KStandardDirs::locate("appdata", "model/model.voca")));
		return false;
	}
	emit progress(2100);
	

	emit status(i18n("Generiere DFA..."));
	if (!makeDfa())
	{
		emit error(i18n("Konnte dfa nicht generieren.\n\nBitte überprüfen Sie die Pfade zur mkfa und dfa_minimize Datei (%1, %2).", CoreConfiguration::mkfa().path(), CoreConfiguration::dfa_minimize().path()));
		return false;
	}
	emit progress(2250);
	
	emit status(i18n("Generiere Grammatikalisches Wörterbuch..."));
	if (!generateDict())
	{
		emit error(i18n("Konnte das grammatikalische Wörterbuch nicht generieren. \nBitte überprüfen Sie die Pfade zur Ausgabedatei. (%1).", KStandardDirs::locate("appdata", "model/model.dict")));
		return false;
	}
	emit progress(2299);
	
	return true;
}

bool ModelManager::makeTempVocab()
{
	QString vocabPath = KStandardDirs::locateLocal("tmp", "simpleVocab");
	QFile vocab ( vocabPath );

	QString terminal;

	if ( !vocab.open ( QFile::ReadOnly ) ) return false;
	QFile tmpVocab ( KStandardDirs::locateLocal("tmp", "tempvoca") );
	if ( !tmpVocab.open ( QFile::WriteOnly ) ) return false;

	QFile term ( KStandardDirs::locateLocal("tmp", "term") );
	if ( !term.open ( QFile::WriteOnly ) ) return false;

	QString vocabEntry;

	int termid=0;
	while ( !vocab.atEnd() )
	{
		vocabEntry = vocab.readLine ( 1024 );
		vocabEntry.remove ( QRegExp ( "\r+$" ) );
		vocabEntry.remove ( QRegExp ( "#.*" ) );
		if ( vocabEntry.trimmed().isEmpty() ) continue;
		if ( vocabEntry.startsWith ( '%' ) )
		{
			terminal = vocabEntry.mid ( 1 ).trimmed();
			terminal = terminal.remove(':');
			tmpVocab.write ( '#'+terminal.toLatin1() +'\n' );

			term.write ( termid+"\t"+terminal.toLatin1() +"\n" );
			termid++;
		}

	}
	vocab.close();
	tmpVocab.close();
	term.close();
	return true;
}

bool ModelManager::makeSimpleVocab()
{
	WordListManager *wlistman = WordListManager::getInstance();
	QStringList usedTerminals = GrammarManager::getInstance()->getTerminals();

	WordList *simpleList = new WordList();
	for (int i=0; i < usedTerminals.count(); i++)
	{
		WordList *tempList = wlistman->getWordsByTerminal(usedTerminals[i]);
		for (int j=0; j < tempList->count(); j++)
		{
			Word currentW = tempList->at(j);
			currentW.setTerminal(currentW.getTerminal().remove(':'));
			simpleList->append(tempList->at(j));
		}
		delete tempList;
	
	}
	
	bool succ = wlistman->saveWordList(simpleList, KStandardDirs::locateLocal("tmp", "simpleLexicon"), KStandardDirs::locateLocal("tmp", "simpleVocab"));
	
	delete simpleList;
	return succ;
}

bool ModelManager::makeDfa()
{
	QString mkfa = CoreConfiguration::mkfa().path();
	QString dfaMinimize= CoreConfiguration::dfa_minimize().path();
	
	QString execStr = '"'+mkfa+"\" -e1 -fg \""+KStandardDirs::locateLocal("tmp", "reverseGrammar")+"\" -fv \""+KStandardDirs::locateLocal("tmp", "tempvoca")+"\" -fo \""+KStandardDirs::locateLocal("tmp", "dfaTemp.tmp")+"\" -fh \""+KStandardDirs::locateLocal("tmp", "dfaTemp.h")+"\"";
	proc->start(execStr);
	proc->waitForFinished(-1);
	if (proc->exitCode() != 0) 
		return false;

	proc->start('"'+dfaMinimize+'"'+" \""+KStandardDirs::locateLocal("tmp", "dfaTemp.tmp")+"\" -o \""+KStandardDirs::locate("appdata", "model/model.dfa")+'"');
	proc->waitForFinished(-1);
	if (proc->exitCode()!= 0) 
		return false;

	return true;	
}

bool ModelManager::generateReverseGrammar()
{
// 	QFile grammar(KStandardDirs::locate("appdata", "model/model.grammar"));
// 	if (!grammar.open(QFile::ReadOnly)) return false;

	QStringList structures = CoreConfiguration::grammarStructures();

	if (structures.count() == 0)
		return false;

	QFile reverseGrammar(KStandardDirs::locateLocal("tmp", "reverseGrammar"));
	if (!reverseGrammar.open(QFile::WriteOnly)) return false;

	QString reverseGrammarEntry;
	QString grammarEntry;
	QStringList terminals;
	QString identifier;
	
	for (int i=0; i < structures.count(); i++)
	{
		grammarEntry = structures[i];
		if (grammarEntry.trimmed().isEmpty()) continue;
		
// 		int splitter =grammarEntry.indexOf(QRegExp("\\:"));
// 		if (splitter == -1) continue;
// 		identifier = grammarEntry.left(splitter);
		
		reverseGrammarEntry = "S: NS_E ";
		terminals = grammarEntry.split(' ');
		for (int j=terminals.count()-1; j >= 0; j--)
			reverseGrammarEntry += terminals[j].remove(':').trimmed()+' ';
		reverseGrammarEntry += " NS_B";
		reverseGrammar.write(reverseGrammarEntry.toLatin1()+'\n');
	}
	reverseGrammar.close();
	return true;
}

bool ModelManager::generateDict()
{
	int nowId = -1;
	QFile vocab(KStandardDirs::locateLocal("tmp", "simpleVocab"));
	if (!vocab.open(QFile::ReadOnly)) return false;
	QFile dict(KStandardDirs::locate("appdata", "model/model.dict"));
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
			
			dict.write(QString(QString::number(nowId)+"\t["+vocabEntry.left(splitter)+"]\t"+vocabEntry.mid(splitter).trimmed()+'\n').toLatin1());
		}
	}
	
	vocab.close();
	dict.close();
	return true;
}

void ModelManager::displayError(QString error)
{
	KMessageBox::error(0, i18n("Beim Kompilieren des Modells ist ein Fehler aufgetreten:\n\n%1\n\nLetzter Output:\n...%2\n\nFehlermeldung:\n...%3", error, lastOutput.right(600), lastError.right(600)));
	emit status(i18n("Abgebrochen."));
	emit progress(100,100);
}

bool ModelManager::startCompilation()
{
	if (isRunning()) return false;
	lastError="";
	start();
	return true;
}

bool ModelManager::codeAudioData()
{
	emit status(i18n("Kodiere Audiodaten..."));
	
	//creating codetrain
	if (!generateCodetrainScp())
	{
		emit error(i18n("Konnte CodeTrain-Datei nicht erstellen."));
		return false;
	}

	emit progress(160);
	QString codetrainPath = KStandardDirs::locateLocal("tmp", "/codetrain.scp");

	//TODO: implement some sort of caching (maybe with an file/hash combination?)
	proc->start('"'+CoreConfiguration::hCopy().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "model/wav_config")+"\" -S \""+codetrainPath+'"');
	proc->waitForFinished(-1);
	if (!proc->exitCode()==0)
	{
		emit error(i18n("Fehler beim kodieren der samples! Bitte überprüfen Sie den Pfad zu HCopy (%1) und der wav config (%2)", CoreConfiguration::hCopy().path(), KStandardDirs::locate("appdata", "model/wav_config")));
		return false;
	}
	emit progress(550);

	return true;
}

bool ModelManager::generateCodetrainScp()
{
	QString samplePath = CoreConfiguration::modelTrainingsDataPath().path();
	QString codetrainPath = KStandardDirs::locateLocal("tmp", "codetrain.scp");
	QString trainPath = KStandardDirs::locateLocal("tmp", "train.scp");

	QDir wavDir(samplePath);
	QStringList wavs = wavDir.entryList(QStringList() << ("*.wav"));
	
	QString pathToMFCs =KStandardDirs::locateLocal("tmp", "mfcs");
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

		
		scpFile.write(QString('"'+samplePath+'/'+file + "\" \"" + mfcFile +"\"\n").toLatin1());
		trainScpFile.write(mfcFile.toLatin1()+'\n');
	}
	scpFile.close();
	trainScpFile.close();
	return true;
}

void ModelManager::run()
{
	proc = new QProcess();
	proc->setWorkingDirectory(QCoreApplication::applicationDirPath());
	connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(logInfo()));
	connect(this, SIGNAL(finished()), proc, SLOT(deleteLater()));
	
	Logger::log(i18n("[INF] Modell wird generiert..."));
	emit status(i18n("Vorbereitung"));
	emit progress(0,2300);
	

	if (!generateInputFiles()) return;
	if (!makeTranscriptions()) return;
	if (!codeAudioData()) return;
	if (!buildHMM()) return;
	if (!compileGrammar()) return;

	//sync model
	
	emit status(i18n("Fertig."));
	emit progress(2300, 2300);
}

bool ModelManager::generateInputFiles()
{	
	emit status(i18n("Generiere Wordliste..."));
	//wlist
	if (!generateWlist())
	{
		emit error(i18n("Erstellen der Wordliste fehlgeschlagen. Bitte überprüfen Sie die Berechtigungen für den Temporären Pfad."));
		return false;
	}
	emit progress(35);

	//monophones
	emit status(i18n("Erstelle Monophone..."));

	
	if (!makeMonophones())
	{
		emit error(i18n("Erstellen der Monophone fehlgeschlagen. Bitte überprüfen Sie ob das Programm HDMan richtig eingerichtet ist und das Lexicon alle verwendeten Wörter beinhaltet."));
		return false;
	}
	
	emit progress(40);

	return true;
}

bool ModelManager::makeTranscriptions()
{
	//mlf
	emit status(i18n("Erstelle Master Label File..."));
	if (!generateMlf())
	{
		emit error(i18n("Erstellen der Master Label File fehlgeschlagen. Bitte überprüfen Sie die prompts-Datei (%1)", KStandardDirs::locate("appdata", "Model/PathToPrompts")));
		return false;
	}
	emit progress(55);
	
	
	proc->start('"'+CoreConfiguration::hLEd().path()+"\" -A -D -T 1 -l \"*\" -d \""+KStandardDirs::locateLocal("tmp", "/dict")+"\" -i \""+KStandardDirs::locateLocal("tmp", "/phones0.mlf")+"\" \""+KStandardDirs::locate("appdata", "model/scripts/mkphones0.led")+"\" \""+KStandardDirs::locateLocal("tmp", "/words.mlf")+"\"");
	proc->waitForFinished(-1);
	if (proc->exitCode() ==0)
	{
		proc->start('"'+CoreConfiguration::hLEd().path()+"\" -A -D -T 1 -l \"*\" -d \""+KStandardDirs::locateLocal("tmp", "/dict")+"\" -i \""+KStandardDirs::locateLocal("tmp", "/phones1.mlf")+"\" \""+KStandardDirs::locate("appdata", "model/scripts/mkphones1.led")+"\" \""+KStandardDirs::locateLocal("tmp", "/words.mlf")+"\"");
		proc->waitForFinished(-1);
	}
	
	
	if (proc->exitCode() != 0)
	{
		emit error(i18n("Erstellen der Transcriptions files fehlgeschlagen. Bitte überprüfen Sie ob Sie den Pfad für die Dateien mkphones0.led und mkphones1.led richtig angegeben haben. (%1, %2)", KStandardDirs::locate("appdata", "model/scripts/mkphones0.led"), KStandardDirs::locate("appdata", "model/scripts/mkphones1.led")));
		return false;
	}
	emit progress(155);
	return true;
}

/**
 * \brief Processes an error (reacts on it some way)
 * \author Peter Grasch
 * \param userError A Human-Readable error-description
 * @return If this is true we knew what to do; if this is false you'd better throw an error message
 */
bool ModelManager::processError(QString userError)
{
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
		
		//this error ONLY occurs when there are samples for the word but the word itself is not recorded
		//so - RECORD THE WORD!
		emit missingWord(word);
	} else if (err.startsWith("ERROR [+2662]"))
	{
// 		"ERROR [+2662]  FindProtoModel: no proto for E in hSet
		QString phoneme = err.mid(44,err.indexOf(' ', 44)-44);
		displayError(i18n("Phonem %1 kommt in den Trainingsdaten nicht vor.\n\nBitte trainieren Sie ein Wort welches das Phonem %1 beinhaltet.\n\nSie können zum Beispiel in der Wortliste ein beliebiges Wort wählen welches diesen Phonem beinhaltet, ihn zu einem speziellen Training hinzufügen und dieses Training dann durchführen.", phoneme));
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
			QString filename = CoreConfiguration::modelTrainingsDataPath().path()+'/'+sampleName+".wav";
			
			emit sampleWithoutWord(filename);
		}
	}
	else if (err.contains("Error:       undefined class \""))
	{
		int startIndex = err.indexOf("Error:       undefined class \"") + 30;
		
		QString undefClass = err.mid(startIndex);
		undefClass = undefClass.left(undefClass.indexOf('"'));
		emit unknownGrammarClass(undefClass);
	} else
		displayError(userError);

	emit status(i18n("Abgebrochen"));
	emit progress(1,1);
	return false;
}

bool ModelManager::createMonophones()
{
	emit status(i18n("Erstelle hmm0..."));
	if (!buildHMM0())
	{
		emit error(i18n("Fehler beim Generieren des HMM0. \n\nBitte überprüfen Sie, ob ausreichend Trainingsmaterial vorhanden ist.\n\nSollten Sie sicher sein, das Modell wurde ausreichend trainiert, überprüfen Sie bitte den Pfad zu HCompV (%1), der config (%2) und des Prototypen (%3).", CoreConfiguration::hCompV().path(), KStandardDirs::locate("appdata", "model/scripts/config"), KStandardDirs::locate("appdata", "model/scripts/proto")));
		return false;
	}
	emit progress(550);
	emit status(i18n("Erstelle hmm1..."));
	if (!buildHMM1())
	{
		emit error(i18n("Fehler beim Generieren des HMM1. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2)", CoreConfiguration::hERest().path(), KStandardDirs::locate("appdata", "model/scripts/config")));
		return false;
	}
	emit progress(800);
	emit status(i18n("Erstelle hmm2..."));
	if (!buildHMM2())
	{
		emit error(i18n("Fehler beim Generieren des HMM2. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2)", CoreConfiguration::hERest().path(), KStandardDirs::locate("appdata", "model/scripts/config")));
		return false;
	}
	emit progress(850);
	emit status(i18n("Erstelle hmm3..."));
	if (!buildHMM3())
	{
		emit error(i18n("Fehler beim Generieren des HMM3. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2)", CoreConfiguration::hERest().path(), KStandardDirs::locate("appdata", "model/scripts/config")));
		return false;
	}
	emit progress(900);
	return true;
}

bool ModelManager::fixSilenceModel()
{
	emit status(i18n("Erstelle Pausenmodell (hmm4)..."));
	if (!buildHMM4())
	{
		emit error(i18n("Fehler beim Generieren des HMM4. Bitte überprüfen Sie das HMM3"));
		return false;
	}
	emit progress(950);
	emit status(i18n("Erstelle hmm5..."));
	if (!buildHMM5())
	{
		emit error(i18n("Fehler beim Generieren des HMM5. Bitte überprüfen Sie den Pfad zu HHEd (%1) und  des Silence-Modells (%2)", CoreConfiguration::hHEd().path(), KStandardDirs::locate("appdata", "model/scripts/sil.hed")));
		return false;
	}
	emit progress(1000);
	emit status(i18n("Erstelle hmm6..."));
	if (!buildHMM6())
	{
		emit error(i18n("Fehler beim Generieren des HMM6. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2", CoreConfiguration::hERest().path(), KStandardDirs::locate("appdata", "model/scripts/config")));
		return false;
	}
	emit progress(1080);
	emit status(i18n("Erstelle hmm7..."));
	if (!buildHMM7())
	{
		emit error(i18n("Fehler beim Generieren des HMM7. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2", CoreConfiguration::hERest().path(), KStandardDirs::locate("appdata", "model/scripts/config")));
		return false;
	}
	emit progress(1150);
	
	return true;
}

bool ModelManager::realign()
{
	emit status(i18n("Erstellte dict1..."));
	if (!makeDict1())
	{
		emit error(i18n("Fehler beim erstellen des dict1"));
		return false;
	}
	emit progress(1160);

	emit status(i18n("Hmm7 neu ausrichten..."));
	if (!realignHMM7())
	{
		emit error(i18n("Konnte HMM7 nicht neu ausrichten. Bitte überprüfen Sie den Pfad zu HVite (%1), der config (%2) und das HMM7.", CoreConfiguration::hVite().path(), KStandardDirs::locate("appdata", "model/scripts/config")));
		return false;
	}
	emit progress(1160);

	emit status(i18n("Erstelle hmm8..."));
	if (!buildHMM8())
	{
		emit error(i18n("Fehler beim Generieren des HMM8. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2", CoreConfiguration::hERest().path(), KStandardDirs::locate("appdata", "model/scripts/config")));
		return false;
	}
	emit progress(1230);

	emit status(i18n("Erstelle hmm9..."));
	if (!buildHMM9())
	{
		emit error(i18n("Fehler beim Generieren des HMM9. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2", CoreConfiguration::hERest().path(), KStandardDirs::locate("appdata", "model/scripts/config")));
		return false;
	}
	emit progress(1300);
	
	return true;
}

bool ModelManager::tieStates()
{
	emit status(i18n("Erstelle triphone..."));
	
	proc->start('"'+CoreConfiguration::hDMan().path()+"\" -A -D -T 1 -b sp -n \""+KStandardDirs::locateLocal("tmp", "fulllist")+"\" -g \""+KStandardDirs::locate("appdata", "model/scripts/global.ded")+"\" \""+KStandardDirs::locateLocal("tmp", "dict-tri")+"\" \""+KStandardDirs::locate("appdata", "model/lexicon")+'"');
	proc->waitForFinished(-1);
	if ((proc->exitCode() != 0))
	{
		emit error(i18n("Konnte Triphone nicht binden. Bitte überprüfen Sie den Pfad zu HDMan (%1), global.ded (%2) und dem Lexikon (%3).", CoreConfiguration::hDMan().path(), KStandardDirs::locate("appdata", "model/scripts/global.ded"), KStandardDirs::locate("appdata", "model/lexicon")));
		return false;
	}
	emit progress(1700);

	emit status(i18n("Erstelle Liste der Triphone..."));
	if (!makeFulllist())
	{
		emit error(i18n("Konnte Liste der Triphone nicht erstellen."));
		return false;
	}
	emit progress(1705);
	emit status(i18n("Erstelle tree.hed..."));
	if (!makeTreeHed())
	{
		emit error(i18n("Konnte tree.hed nicht erstellen."));
		return false;
	}
	emit progress(1750);
	
	emit status(i18n("Erstelle hmm13..."));
	if (!buildHMM13())
	{
		emit error(i18n("Fehler beim Generieren des HMM13. Bitte überprüfen Sie den Pfad zu HHEd (%1).", CoreConfiguration::hHEd().path()));
		return false;
	}
	emit progress(1830);
	
	
	emit status(i18n("Erstelle hmm14..."));
	if (!buildHMM14())
	{
		emit error(i18n("Fehler beim Generieren des HMM14. Bitte überprüfen Sie den Pfad zu HERest (%1), der config (%2), und die stats-Datei (%3)", CoreConfiguration::hERest().path(), KStandardDirs::locate("appdata", "model/scripts/config"), KStandardDirs::locateLocal("tmp", "stats")));
		return false;
	}
	emit progress(1900);
	
	emit status(i18n("Erstelle hmm15..."));
	if (!buildHMM15())
	{
		emit error(i18n("Fehler beim Generieren des HMM15. Bitte überprüfen Sie den Pfad zu HERest (%1), der config (%2), und die stats-Datei (%3)", CoreConfiguration::hERest().path(), KStandardDirs::locate("appdata", "model/scripts/config"), KStandardDirs::locateLocal("tmp", "stats")));
		return false;
	}
	emit progress(1990);

	
	return true;
}

bool ModelManager::buildHMM13()
{
	proc->start('"'+CoreConfiguration::hHEd().path()+"\" -A -D -T 1 -H \""+KStandardDirs::locateLocal("tmp", "hmm12/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm12/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm13/")+"\" \""+KStandardDirs::locateLocal("tmp", "tree.hed")+"\" \""+KStandardDirs::locateLocal("tmp", "triphones1")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelManager::buildHMM14()
{
	proc->start('"'+CoreConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "model/scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", "wintri.mlf")+"\" -t 250.0 150.0 3000.0 -s \""+KStandardDirs::locateLocal("tmp", "stats")+"\" -S \""+KStandardDirs::locateLocal("tmp", "train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm13/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm13/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm14/")+"\" \""+KStandardDirs::locateLocal("tmp", "tiedlist")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelManager::buildHMM15()
{
	proc->start('"'+CoreConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "model/scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", "wintri.mlf")+"\" -t 250.0 150.0 3000.0 -s \""+KStandardDirs::locateLocal("tmp", "stats")+"\" -S \""+KStandardDirs::locateLocal("tmp", "train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm14/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm14/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm15/")+"\" \""+KStandardDirs::locateLocal("tmp", "tiedlist")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::makeFulllist()
{	
	QFile::remove(KStandardDirs::locateLocal("tmp", "fulllist-original"));
	if (!QFile::copy(KStandardDirs::locateLocal("tmp", "fulllist"), KStandardDirs::locateLocal("tmp", "fulllist-original"))) return false;
	
	QFile triphones1(KStandardDirs::locateLocal("tmp", "triphones1"));
	QFile fulllist(KStandardDirs::locateLocal("tmp", "fulllist"));
	
	
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

bool ModelManager::makeTreeHed()
{	
	QFile::remove(KStandardDirs::locateLocal("tmp", "tree.hed"));

// 	if (!QFile::copy(KStandardDirs::locate("appdata", "model/tree1.hed"), KStandardDirs::locateLocal("tmp", "tree.hed"))) return false;
	
	QFile tree1Hed(KStandardDirs::locate("appdata", "model/tree1.hed"));
	QFile treeHed(KStandardDirs::locateLocal("tmp", "tree.hed"));
	
	if ((!treeHed.open(QIODevice::WriteOnly)) || 
		(!tree1Hed.open(QIODevice::ReadOnly))) return false;
	
	//HTK uses Latin1 instead of UTF-8 :/
	treeHed.write("RO 100 \""+KStandardDirs::locateLocal("tmp", "stats").toLatin1()+"\"\n");
	
	//copy tree1.hed content to tree.hed
	treeHed.write(tree1Hed.readAll());
	
	tree1Hed.close();
	
	QString command = "TB";
	int threshold = 350;
	QFile hmmlist(KStandardDirs::locateLocal("tmp", "monophones0"));
	if (!hmmlist.open(QIODevice::ReadOnly)) return false;
	
	QStringList phonemeList;
	while (!hmmlist.atEnd())
	{ phonemeList << hmmlist.readLine().trimmed(); }
	hmmlist.close();
	
	for (int i=0; i < phonemeList.count(); i++)
		treeHed.write(QString("%1 %2 \"ST_%3_2_\" {(\"%3\",\"*-%3+*\",\"%3+*\",\"*-%3\").state[2]}\n").arg(command).arg(threshold).arg(phonemeList[i]).toLatin1());
	
	for (int i=0; i < phonemeList.count(); i++)
		treeHed.write(QString("%1 %2 \"ST_%3_3_\" {(\"%3\",\"*-%3+*\",\"%3+*\",\"*-%3\").state[3]}\n").arg(command).arg(threshold).arg(phonemeList[i]).toLatin1());
	
	for (int i=0; i < phonemeList.count(); i++)
		treeHed.write(QString("%1 %2 \"ST_%3_4_\" {(\"%3\",\"*-%3+*\",\"%3+*\",\"*-%3\").state[4]}\n").arg(command).arg(threshold).arg(phonemeList[i]).toLatin1());	
	
	treeHed.write(QString(" \nTR 1\n \nAU "+KStandardDirs::locateLocal("tmp", "fulllist")+" \nCO "+KStandardDirs::locateLocal("tmp", "tiedlist")+" \n \nST "+KStandardDirs::locateLocal("tmp", "trees")+" \n").toLatin1());
	
	treeHed.close();
	
	return true;
}

bool ModelManager::buildHMM()
{
	if (!createMonophones()) return false;
	if (!fixSilenceModel()) return false;
	if (!realign()) return false;
	if (!makeTriphones()) return false;
	if (!tieStates()) return false;
	

	//TODO: Make a Backup!
	emit status(i18n("Übernehme Modell..."));

	if (QFile::exists(KStandardDirs::locateLocal("appdata", "model/hmmdefs")))
		if (!QFile::remove(KStandardDirs::locateLocal("appdata", "model/hmmdefs"))) return false;
	if (!QFile::copy(KStandardDirs::locateLocal("tmp", "hmm15/hmmdefs"), KStandardDirs::locateLocal("appdata", "model/hmmdefs")))
		return false;

	if (QFile::exists(KStandardDirs::locateLocal("appdata", "model/tiedlist")))
		if (!QFile::remove(KStandardDirs::locateLocal("appdata", "model/tiedlist"))) return false;
	if (!QFile::copy(KStandardDirs::locateLocal("tmp", "tiedlist"), KStandardDirs::locateLocal("appdata", "model/tiedlist")))
		return false;

	return true;
}

bool ModelManager::makeTriphones()
{
	emit status(i18n("Erstelle triphone..."));
	proc->start('"'+CoreConfiguration::hLEd().path()+"\" -A -D -T 1 -n \""+KStandardDirs::locateLocal("tmp", "triphones1")+"\" -l * -i \""+KStandardDirs::locateLocal("tmp", "/wintri.mlf")+"\" \""+KStandardDirs::locate("appdata", "model/scripts/mktri.led")+"\" \""+KStandardDirs::locateLocal("tmp", "aligned.mlf")+"\"");
	proc->waitForFinished(-1);
	if ((proc->exitCode() != 0))
	{
		emit error(i18n("Erstellen der Triphone files fehlgeschlagen. Bitte überprüfen Sie ob Sie den Pfad für die Datei mktri.led richtig angegeben haben (%1) und überprüfen Sie den Pfad zu HLEd (%2)", KStandardDirs::locate("appdata", "model/scripts/mktri.led"), CoreConfiguration::hLEd().path()));
		return false;
	}
	emit progress(1380);
	
	emit status(i18n("Erstelle mktri.hed..."));
	if (!makeMkTriHed())
	{
		emit error(i18n("Fehler beim generieren der mktri.hed"));
		return false;
	}
	emit progress(1400);
	
	emit status(i18n("Erstelle hmm10..."));
	if (!buildHMM10())
	{
		emit error(i18n("Fehler beim Generieren des HMM10. Bitte überprüfen Sie den Pfad zu HHEd (%1).", CoreConfiguration::hHEd().path()));
		return false;
	}
	emit progress(1470);
	
	emit status(i18n("Erstelle hmm11..."));
	if (!buildHMM11())
	{
		emit error(i18n("Fehler beim Generieren des HMM11. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2)", CoreConfiguration::hERest().path(), KStandardDirs::locate("appdata", "model/scripts/config")));
		return false;
	}
	emit progress(1550);
	
	emit status(i18n("Erstelle hmm12..."));
	if (!buildHMM12())
	{
		emit error(i18n("Fehler beim Generieren des HMM12. Bitte überprüfen Sie den Pfad zu HERest (%1), der config (%2), und die stats-Datei (%3)", CoreConfiguration::hERest().path(), KStandardDirs::locate("appdata", "model/scripts/config"), KStandardDirs::locateLocal("tmp", "stats")));
		return false;
	}
	emit progress(1620);
	
	return true;
}


bool ModelManager::buildHMM12()
{
	proc->start('"'+CoreConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "model/scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", "wintri.mlf")+"\" -t 250.0 150.0 3000.0 -s \""+KStandardDirs::locateLocal("tmp", "stats")+"\" -S \""+KStandardDirs::locateLocal("tmp", "train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm11/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm11/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm12/")+"\" \""+KStandardDirs::locateLocal("tmp", "triphones1")+"\"");

	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelManager::buildHMM11()
{
	proc->start('"'+CoreConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "model/scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", "wintri.mlf")+"\" -t 250.0 150.0 3000.0 -S \""+KStandardDirs::locateLocal("tmp", "train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm10/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm10/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm11/")+"\" \""+KStandardDirs::locateLocal("tmp", "triphones1")+"\"");
	
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelManager::buildHMM10()
{
	proc->start('"'+CoreConfiguration::hHEd().path()+"\" -A -D -T 1 -H \""+KStandardDirs::locateLocal("tmp", "hmm9/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm9/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm10/")+"\" \""+KStandardDirs::locateLocal("tmp", "mktri.hed")+"\" \""+KStandardDirs::locateLocal("tmp", "monophones1")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelManager::makeMkTriHed()
{
	QFile monophones1(KStandardDirs::locateLocal("tmp", "monophones1"));
	if (!monophones1.open(QIODevice::ReadOnly))
		return false;
		
	
	QFile mktriHed(KStandardDirs::locateLocal("tmp", "mktri.hed"));
	if (!mktriHed.open(QIODevice::WriteOnly)) return false;
	
	mktriHed.write("CL "+KStandardDirs::locateLocal("tmp", "triphones1").toLatin1()+"\n");
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


bool ModelManager::buildHMM9()
{
	proc->start('"'+CoreConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "model/scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", "aligned.mlf")+"\" -t 250.0 150.0 3000.0 -S \""+KStandardDirs::locateLocal("tmp", "train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm8/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm8/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm9/")+"\" \""+KStandardDirs::locateLocal("tmp", "monophones1")+"\"");
	
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelManager::buildHMM8()
{
	proc->start('"'+CoreConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "model/scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", "aligned.mlf")+"\" -t 250.0 150.0 3000.0 -S \""+KStandardDirs::locateLocal("tmp", "train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm7/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm7/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm8/")+"\" \""+KStandardDirs::locateLocal("tmp", "monophones1")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::realignHMM7()
{
	proc->start('"'+CoreConfiguration::hVite().path()+"\" -A -D -T 1 -l *  -o SWT -b silence -C \""+KStandardDirs::locate("appdata", "model/scripts/config")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm7/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm7/hmmdefs")+"\" -i \""+KStandardDirs::locateLocal("tmp", "aligned.mlf")+"\" -m -t 250.0 150.0 1000.0 -y lab -a -I \""+KStandardDirs::locateLocal("tmp", "words.mlf")+"\" -S \""+KStandardDirs::locateLocal("tmp", "train.scp")+"\" \""+KStandardDirs::locateLocal("tmp", "dict1")+"\" \""+KStandardDirs::locateLocal("tmp", "monophones1")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::makeDict1()
{
	QFile::remove(KStandardDirs::locateLocal("tmp", "dict1"));
	if (!QFile::copy(KStandardDirs::locateLocal("tmp", "dict"), KStandardDirs::locateLocal("tmp", "dict1"))) return false;
	QFile dict1(KStandardDirs::locateLocal("tmp", "dict1"));
	if (!dict1.open(QIODevice::WriteOnly|QIODevice::Append)) return false;

	dict1.write("silence  []  sil\n");
	dict1.close();
	return true;
}

bool ModelManager::buildHMM7()
{
	proc->start('"'+CoreConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "model/scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", "phones1.mlf")+"\" -t 250.0 150.0 3000.0 -S \""+KStandardDirs::locateLocal("tmp", "train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm6/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm6/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm7/")+"\" \""+KStandardDirs::locateLocal("tmp", "monophones1")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::buildHMM6()
{
	proc->start('"'+CoreConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "model/scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", "phones1.mlf")+"\" -t 250.0 150.0 3000.0 -S \""+KStandardDirs::locateLocal("tmp", "train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm5/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm5/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm6/")+"\" \""+KStandardDirs::locateLocal("tmp", "monophones1")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::buildHMM5()
{
	proc->start('"'+CoreConfiguration::hHEd().path()+"\" -A -D -T 1 -H \""+KStandardDirs::locateLocal("tmp", "hmm4/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm4/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm5/")+"\" \""+KStandardDirs::locate("appdata", "model/scripts/sil.hed")+"\" \""+KStandardDirs::locateLocal("tmp", "monophones1")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::buildHMM4()
{
	QFile::copy(KStandardDirs::locateLocal("tmp", "hmm3/macros"), KStandardDirs::locateLocal("tmp", "hmm4/macros"));

	QStringList  tmp2;

	QFile hmmdefs3(KStandardDirs::locateLocal("tmp", "hmm3/hmmdefs"));
	if (!hmmdefs3.open(QIODevice::ReadOnly)) return false;
	QFile hmmdefs4(KStandardDirs::locateLocal("tmp", "hmm4/hmmdefs"));
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
			hmmdefs4.write(tmp2[0].replace("~h \"sil\"", "~h \"sp\"").toLatin1());
			hmmdefs4.write(tmp2[1].toLatin1());
			hmmdefs4.write(tmp2[2].replace('5', '3').toLatin1());
			hmmdefs4.write(tmp2[9].replace('3', '2').toLatin1());
			hmmdefs4.write(tmp2[10].toLatin1());
			hmmdefs4.write(tmp2[11].toLatin1());
			hmmdefs4.write(tmp2[12].toLatin1());
			hmmdefs4.write(tmp2[13].toLatin1());
			hmmdefs4.write(tmp2[14].toLatin1());
			hmmdefs4.write(tmp2[21].replace('5', '3').toLatin1());
			hmmdefs4.write("0.000000e+000 1.000000e+000 0.000000e+000\n");
			hmmdefs4.write("0.000000e+000 0.900000e+000 0.100000e+000\n");
			hmmdefs4.write("0.000000e+000 0.000000e+000 0.000000e+000\n");
		}
		hmmdefs4.write(line);
	}
	return true;
}

bool ModelManager::buildHMM3()
{
	proc->start('"'+CoreConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "model/scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", "phones0.mlf")+"\" -t 250.0 150.0 1000.0 -S \""+KStandardDirs::locateLocal("tmp", "train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm2/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm2/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm3/")+"\" \""+KStandardDirs::locateLocal("tmp", "monophones0")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::buildHMM2()
{
	proc->start('"'+CoreConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "model/scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", "phones0.mlf")+"\" -t 250.0 150.0 1000.0 -S \""+KStandardDirs::locateLocal("tmp", "train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm1/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm1/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm2/")+"\" \""+KStandardDirs::locateLocal("tmp", "monophones0")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::buildHMM1()
{
	proc->start('"'+CoreConfiguration::hERest().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "model/scripts/config")+"\" -I \""+KStandardDirs::locateLocal("tmp", "phones0.mlf")+"\" -t 250.0 150.0 1000.0 -S \""+KStandardDirs::locateLocal("tmp", "train.scp")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm0/macros")+"\" -H \""+KStandardDirs::locateLocal("tmp", "hmm0/hmmdefs")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm1/")+"\" \""+KStandardDirs::locateLocal("tmp", "monophones0")+"\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::buildHMM0()
{
	proc->start('"'+CoreConfiguration::hCompV().path()+"\" -A -D -T 1 -C \""+KStandardDirs::locate("appdata", "model/scripts/config")+"\" -f 0.01 -m -S \""+KStandardDirs::locateLocal("tmp", "train.scp")+"\" -M \""+KStandardDirs::locateLocal("tmp", "hmm0/")+"\" \""+KStandardDirs::locate("appdata", "model/scripts/proto")+'"');
	proc->waitForFinished(-1);
	if (proc->exitCode()!=0) return false;

	QString protoBody="";
	QString protoHead="";
	QString line;
	QFile protoFile(KStandardDirs::locateLocal("tmp", "hmm0/proto"));
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
	QFile monophones0(KStandardDirs::locateLocal("tmp", "monophones0"));
	if (!monophones0.open(QIODevice::ReadOnly)) return false;

	while (!monophones0.atEnd())
		monophones.append(monophones0.readLine(50).trimmed());
	monophones0.close();
	
	Logger::log(i18n("[INF] Verwendete Monophone des Modells: %1", monophones.join(", ")));

	QFile hmmdefs(KStandardDirs::locateLocal("tmp", "hmm0/hmmdefs"));
	if (!hmmdefs.open(QIODevice::WriteOnly)) return false;
	QString phoneHead;
	QString currentHead="";
	for (int i=0; i < monophones.count(); i++)
	{
		currentHead = protoHead;
		hmmdefs.write(currentHead.replace("proto", monophones[i]).toLatin1());
		hmmdefs.write(protoBody.toLatin1());
	}
	hmmdefs.close();

	//generating macros
	QFile macros(KStandardDirs::locateLocal("tmp", "hmm0/macros"));
	if (!macros.open(QIODevice::WriteOnly)) return false;
	macros.write(protoPreamble.toLatin1());

	QFile vFloors(KStandardDirs::locateLocal("tmp", "hmm0/vFloors"));
	if (!vFloors.open(QIODevice::ReadOnly)) return false;
	while (!vFloors.atEnd()) macros.write(vFloors.readLine(1000));
	vFloors.close();

	macros.close();

	return true;
}

bool ModelManager::makeMonophones()
{
	//make monophones1
	proc->start('"'+CoreConfiguration::hDMan().path()+"\" -A -D -T 1 -m -w \""+KStandardDirs::locateLocal("tmp", "/wlist")+"\" -g \""+KStandardDirs::locate("appdata", "model/scripts/global.ded")+"\" -n \""+KStandardDirs::locateLocal("tmp", "monophones1")+"\" -i \""+KStandardDirs::locateLocal("tmp", "dict")+"\" \""+KStandardDirs::locate("appdata", "model/lexicon")+'"');
	proc->waitForFinished(-1);
	if (proc->exitStatus()!=0) return false;

	//make monophones0
	//ditch the "sp" phoneme

	QFile monophones1(KStandardDirs::locateLocal("tmp", "/monophones1"));
	QFile monophones0(KStandardDirs::locateLocal("tmp", "/monophones0"));
	if (!monophones1.open(QIODevice::ReadOnly))
		return false;
	if (!monophones0.open(QIODevice::WriteOnly|QIODevice::Truncate))
		return false;

	QString phoneme;
	while (!monophones1.atEnd())
	{
		phoneme = monophones1.readLine(50);
		if ((phoneme.trimmed() != "sp") && (!phoneme.trimmed().isEmpty()))
			monophones0.write(phoneme.toLatin1());
	}
	monophones1.close();
	monophones0.close();
	return true;
}

void ModelManager::logInfo()
{
	lastOutput = proc->readAllStandardOutput();
	Logger::log(i18n("[INF]")+' '+lastOutput);
}

bool ModelManager::generateWlist()
{
	QFile promptsFile(KStandardDirs::locateLocal("appdata", "model/prompts"));
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
	
	QFile wlistFile(KStandardDirs::locateLocal("tmp", "wlist"));
	if (!wlistFile.open(QIODevice::WriteOnly))
		return false;
	for (int i=0; i < words.count(); i++)
	{
		wlistFile.write(words[i].toLatin1()+'\n');
	}
	wlistFile.close();
	return true;
}

bool ModelManager::generateMlf()
{
	QFile promptsFile(KStandardDirs::locateLocal("appdata", "model/prompts"));
	QFile mlf(KStandardDirs::locateLocal("tmp", "words.mlf"));

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
		mlf.write(labFile.toLatin1()+"\n");
		for (int i=0; i < lineWords.count(); i++)
			mlf.write(lineWords[i].toLatin1()+"\n");
		mlf.write(".\n");
	}
	promptsFile.close();
	mlf.close();
	return true;
}

ModelManager::~ModelManager()
{
	proc->deleteLater();
}

