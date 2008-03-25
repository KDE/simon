//
// C++ Implementation: modelmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "modelmanager.h"
#include "settings.h"
#include "logger.h"
#include <QCoreApplication>

#include <QMessageBox>
#include <QDebug>
#include <QProgressDialog>
#include <QDir>
#include <QFile>
#include <QProcess>
#include "wordlistmanager.h"
#include "grammarmanager.h"


ModelManager* ModelManager::instance;


ModelManager::ModelManager(QWidget *parent) : QThread(parent)
{
	processDialog = new QProgressDialog();
	connect(processDialog, SIGNAL(canceled()), this, SLOT(terminate()));
	connect(this, SIGNAL(status(QString)), this, SLOT(setStatus(QString)));
	connect(this, SIGNAL(progress(int,int)), this, SLOT(setProgress(int,int)));

	connect(this, SIGNAL(error(QString)), this, SLOT(processError(QString)));
	
	connect(processDialog, SIGNAL(canceled()), this, SLOT(terminate()));

	connect(this, SIGNAL(unknownGrammarClass(QString)), processDialog, SLOT(close()));
	connect(this, SIGNAL(missingWord(QString)), processDialog, SLOT(close()));
	connect(this, SIGNAL(sampleWithoutWord(QString)), processDialog, SLOT(close()));

	processDialog->setWindowTitle(tr("Generiere Sprachmodell..."));
}


bool ModelManager::compileGrammar()
{
	emit status(tr("Generiere Umkehr-Grammatik..."));
	if (!generateReverseGrammar())
	{
		emit error(tr("Konnte Umkehr-Grammatik nicht erstellen.\n\nIst eine Grammatik definiert?\n\nWenn ja, überprüfen Sie bitte auch die Pfade zur Grammatikdatei (%1).").arg(Settings::getS("Model/PathToGrammar")));
		return false;
	}
	
	emit progress(2050);
	emit status(tr("Entferne unverwendete Vokabeln..."));
	if (!makeSimpleVocab())
	{
		emit error(tr("Konnte simple Vokabeln nicht erstellen.\n\nBitte überprüfen Sie ob der die Wortliste richtig geladen wurde."));
		return false;
	}
	emit progress(2090);
	

	emit status(tr("Generiere temporäre Vokabeln..."));
	if (!makeTempVocab())
	{
		emit error(tr("Konnte Temporäre Vokabeln nicht erstellen.\n\nBitte überprüfen Sie die Pfade zur Vokabulardatei (%1).").arg(Settings::getS("Model/PathToVocab")));
		return false;
	}
	emit progress(2100);
	

	emit status(tr("Generiere DFA..."));
	if (!makeDfa())
	{
		emit error(tr("Konnte dfa nicht generieren.\n\nBitte überprüfen Sie die Pfade zur dfa und dfa_minimize Datei (%1, %2).").arg(Settings::getS("Programs/Julius/mkfa")).arg(Settings::getS("Programs/Julius/dfa_minimize")));
		return false;
	}
	emit progress(2250);
	
	emit status(tr("Generiere Grammatikalisches Wörterbuch..."));
	if (!generateDict())
	{
		emit error(tr("Konnte das grammatikalische Wörterbuch nicht generieren. \nBitte überprüfen Sie die Pfade zur Ausgabedatei. (%1).").arg(Settings::getS("Model/PathToDict")));
		return false;
	}
	emit progress(2299);
	
	return true;
}

bool ModelManager::makeTempVocab()
{
	QString vocabPath = realTmpDir+"simpleVocab";
	QFile vocab ( vocabPath );

	QString terminal;

	if ( !vocab.open ( QFile::ReadOnly ) ) return false;
	QFile tmpVocab ( realTmpDir+"tempvoca" );
	if ( !tmpVocab.open ( QFile::WriteOnly ) ) return false;

	QFile term ( realTmpDir+"term" );
	if ( !term.open ( QFile::WriteOnly ) ) return false;

	QString vocabEntry;

	int termid=0;
	while ( !vocab.atEnd() )
	{
		vocabEntry = vocab.readLine ( 1024 );
		vocabEntry.remove ( QRegExp ( "\r+$" ) );
		vocabEntry.remove ( QRegExp ( "#.*" ) );
		if ( vocabEntry.trimmed().isEmpty() ) continue;
		if ( vocabEntry.startsWith ( "%" ) )
		{
			terminal = vocabEntry.mid ( 1 ).trimmed();
			terminal = terminal.remove(":");
			tmpVocab.write ( "#"+terminal.toLatin1() +"\n" );

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
			currentW.setTerminal(currentW.getTerminal().remove(":"));
			simpleList->append(tempList->at(j));
		}
		delete tempList;
	
	}
	
	bool succ = wlistman->saveWordList(simpleList, realTmpDir+"simpleLexicon", realTmpDir+"simpleVocab");
	
	delete simpleList;
	return succ;
}

bool ModelManager::makeDfa()
{
	QString mkfa = Settings::getS("Programs/Julius/mkfa");
	QString dfaMinimize= Settings::getS("Programs/Julius/dfa_minimize");
	
	QString execStr = "\""+mkfa+"\" -e1 -fg \""+tmpDir+"reverseGrammar\" -fv \""+tmpDir+"tempvoca\" -fo \""+tmpDir+"dfaTemp.tmp\" -fh \""+tmpDir+"dfaTemp.h\"";
	proc->start(execStr);
	proc->waitForFinished(-1);
	if (proc->exitCode() != 0) 
		return false;

	proc->start("\""+dfaMinimize+"\""+" \""+tmpDir+"dfaTemp.tmp\" -o \""+Settings::getS("Model/PathToDfa")+"\"");
	proc->waitForFinished(-1);
	if (proc->exitCode()!= 0) 
		return false;

	return true;	
}

bool ModelManager::generateReverseGrammar()
{
	QFile grammar(Settings::getS("Model/PathToGrammar"));
	if (!grammar.open(QFile::ReadOnly)) return false;

	QFile reverseGrammar(realTmpDir+"reverseGrammar");
	if (!reverseGrammar.open(QFile::WriteOnly)) return false;

	QString reverseGrammarEntry;
	QString grammarEntry;
	QStringList terminals;
	QString identifier;
	
	int structureCount=0;
	
	while (!grammar.atEnd())
	{
		grammarEntry = grammar.readLine(1024);
		grammarEntry.remove(QRegExp("\r+$"));
		grammarEntry.remove(QRegExp("#.*"));
		if (grammarEntry.trimmed().isEmpty()) continue;
		
		int splitter =grammarEntry.indexOf(QRegExp("\\:"));
		
		if (splitter == -1) continue;
		
		identifier = grammarEntry.left(splitter);
		
		reverseGrammarEntry = identifier+": ";
		terminals = grammarEntry.mid(splitter+1).split(" ");
		for (int i=terminals.count()-1; i >= 0; i--)
			reverseGrammarEntry += terminals[i].remove(":").trimmed()+" ";
		
		structureCount++;
		
		reverseGrammar.write(reverseGrammarEntry.toLatin1()+"\n");
	}
	reverseGrammar.close();
	grammar.close();
	if (structureCount==0) {
		return false;
	}
	return true;
}

bool ModelManager::generateDict()
{
	int nowId = -1;
	QFile vocab(realTmpDir+"simpleVocab");
	if (!vocab.open(QFile::ReadOnly)) return false;
	QFile dict(Settings::getS("Model/PathToDict"));
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
		
		if (vocabEntry.startsWith("%"))
		{
			nowId++;
			continue;
		} else
		{
			int splitter = vocabEntry.indexOf("\t");
			if (splitter == -1) continue;
			
			dict.write(QString(QString::number(nowId)+"\t"+"["+vocabEntry.left(splitter)+"]\t"+vocabEntry.mid(splitter).trimmed()+"\n").toLatin1());
		}
	}
	
	vocab.close();
	dict.close();
	return true;
}

void ModelManager::setStatus(QString status)
{
	processDialog->setLabelText(status);
}

void ModelManager::setProgress(int now, int max)
{
	processDialog->setMaximum(max);
	processDialog->setValue(now);
}

void ModelManager::displayError(QString error)
{
	QMessageBox::critical(0, tr("Fehler"), tr("Beim Kompilieren des Modells ist ein Fehler aufgetreten:\n\n%1\n\nLetzter Output:\n...%2\n\nFehlermeldung:\n...%3").arg(error).arg(lastOutput.right(600)).arg(lastError.right(600)));
	processDialog->close();
}

bool ModelManager::startCompilation()
{
	processDialog->show();
	if (isRunning()) return false;
	lastError="";
	start();
	return true;
}

bool ModelManager::generateDirectoryStructure()
{
	bool success=true;
	QDir dir(Settings::getS("TempDir"));
	//remove if it exists
	
	if (!dir.exists("modeltmp"))
		success = dir.mkdir("modeltmp");

	dir.cd ("modeltmp");
	
	for (int i=0; i < 16; i++)
		dir.mkdir("hmm"+QString::number(i));

	return success;
}

bool ModelManager::codeAudioData()
{
	emit status(tr("Kodiere Audiodaten..."));
	
	//creating codetrain
	if (!generateCodetrainScp())
	{
		emit error(tr("Konnte CodeTrain-Datei nicht erstellen."));
		return false;
	}

	emit progress(160);
	QString codetrainPath = tmpDir+"/codetrain.scp";

	//TODO: implement some sort of caching (maybe with an file/hash combination?)
	proc->start("\""+Settings::getS("Programs/HTK/HCopy")+"\" -A -D -T 1 -C \""+Settings::getS("Model/PathToWavConfig")+"\" -S \""+codetrainPath+"\"");
	proc->waitForFinished(-1);
	if (!proc->exitCode()==0)
	{
		emit error(tr("Fehler beim kodieren der samples! Bitte überprüfen Sie den Pfad zu HCopy (%1) und der wav config (%2)").arg(Settings::getS("Programs/HTK/HCopy")).arg(Settings::getS("Model/PathToWavConfig")));
		return false;
	}
	emit progress(550);

	return true;
}

bool ModelManager::generateCodetrainScp()
{
	QString samplePath = Settings::getS("Model/PathToSamples");
	QString codetrainPath = realTmpDir+"codetrain.scp";
	QString trainPath = realTmpDir+"train.scp";

	QDir wavDir(samplePath);
	QStringList wavs = wavDir.entryList(QStringList() << ("*.wav"));
	
	QString pathToMFCs =realTmpDir+"mfcs";
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
		fileBase = file.left(file.lastIndexOf("."));
		mfcFile = pathToMFCs+"/"+fileBase+".mfc";

		
		scpFile.write(QString("\""+samplePath+"/"+file + "\" \"" + mfcFile +"\"\n").toLatin1());
		trainScpFile.write(mfcFile.toLatin1()+"\n");
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
	connect(processDialog, SIGNAL(canceled()), proc, SLOT(terminate()));
	connect(this, SIGNAL(finished()), proc, SLOT(deleteLater()));
	
	Logger::log(tr("[INF] Modell wird generiert..."));
	emit status(tr("Vorbereitung"));
	emit progress(0,2300);
	
	if (!generateDirectoryStructure())
	{
		emit error(tr("Fehler beim Erstellen der Ordnerstruktur. Bitte überprüfen Sie die Berechtigungen für den Temporären Ordner (%1)").arg(Settings::getS("TempDir")));
		return;
	}
	emit progress(2);

	this->tmpDir = realTmpDir = Settings::getS("TempDir")+"/modeltmp/";
// 	proc->setWorkingDirectory(tmpDir);
	
// 	tmpDir = ".";
	if (tmpDir.startsWith(proc->workingDirectory()))
	{
		tmpDir = "./"+tmpDir.mid(proc->workingDirectory().size());
	}

	if (!generateInputFiles()) return;
	if (!makeTranscriptions()) return;
	if (!codeAudioData()) return;
	if (!buildHMM()) return;
	if (!compileGrammar()) return;
	
	emit status(tr("Abgeschlossen"));
	emit progress(2300, 2300);
}

bool ModelManager::generateInputFiles()
{	
	emit status(tr("Generiere Wordliste..."));
	//wlist
	if (!generateWlist())
	{
		emit error(tr("Erstellen der Wordliste fehlgeschlagen. Bitte überprüfen Sie die Berechtigungen für den Temporären Pfad (%1) und ob die Prompts-File richtig gesetzt ist (%2)")
				.arg(Settings::getS("TempDir")).arg(Settings::getS("Model/PathToPrompts")));
		return false;
	}
	emit progress(35);

	//monophones
	emit status(tr("Erstelle Monophone..."));

	
	if (!makeMonophones())
	{
		emit error(tr("Erstellen der Monophone fehlgeschlagen. Bitte überprüfen Sie ob das Programm HDMan richtig eingerichtet ist und der Pfad zum Lexikon richtig konfiguriert wurde (%1), und diese alle verwendeten Wörter beinhaltet.").arg(Settings::getS("Model/PathToLexicon")));
		return false;
	}
	
	emit progress(40);

	return true;
}

bool ModelManager::makeTranscriptions()
{
	//mlf
	emit status(tr("Erstelle Master Label File..."));
	if (!generateMlf())
	{
		emit error(tr("Erstellen der Master Label File fehlgeschlagen. Bitte überprüfen Sie, ob die prompts-Datei (%1)").arg(Settings::getS("Model/PathToPrompts")));
		return false;
	}
	emit progress(55);
	
	
	proc->start("\""+Settings::getS("Programs/HTK/HLEd")+"\" -A -D -T 1 -l \"*\" -d \""+tmpDir+"/dict\" -i \""+tmpDir+"/phones0.mlf\" \""+Settings::getS("Model/PathToMkPhones0")+"\" \""+tmpDir+"/words.mlf\"");
	proc->waitForFinished(-1);
	if (proc->exitCode() ==0)
	{
		proc->start("\""+Settings::getS("Programs/HTK/HLEd")+"\" -A -D -T 1 -l \"*\" -d \""+tmpDir+"/dict\" -i \""+tmpDir+"/phones1.mlf\" \""+Settings::getS("Model/PathToMkPhones1")+"\" \""+tmpDir+"/words.mlf\"");
		proc->waitForFinished(-1);
	}
	
	
	if (proc->exitCode() != 0)
	{
		emit error(tr("Erstellen der Transcriptions files fehlgeschlagen. Bitte überprüfen Sie ob Sie den Pfad für die Dateien mkphones0.led und mkphones1.led richtig angegeben haben. (%1, %2)").arg(Settings::getS("Model/PathToMkPhones0")).arg(Settings::getS("Model/PathToMkPhones1")));
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
	lastError = QString(lastError+"\n"+proc->readAllStandardError()).right(400);
	
	Logger::log(tr("[ERR]")+" "+lastError);
	
	QString err = lastError.trimmed();

	qDebug() << err;
	if (err.startsWith(("ERROR [+1232]"))) //word missing
	{
		//ERROR [+1232]  NumParts: Cannot find word DARAUFFOLGEND in dictionary
		int wordstart = 45;
		QString word = lastError.mid(wordstart, lastError.indexOf(" ", wordstart)-wordstart);
		
		//this error ONLY occurs when there are samples for the word but the word itself is not recorded
		//so - RECORD THE WORD!
		emit missingWord(word);
	} else if (err.startsWith("ERROR [+2662]"))
	{
// 		"ERROR [+2662]  FindProtoModel: no proto for E in hSet
		QString phoneme = err.mid(44,err.indexOf(" ", 44)-44);
		displayError(tr("Phonem %1 kommt in den Trainingsdaten nicht vor.\n\nBitte trainieren Sie ein Wort welches das Phonem %1 beinhaltet.\n\nSie können zum Beispiel in der Wortliste ein beliebiges Wort wählen welches diesen Phonem beinhaltet, ihn zu einem speziellen Training hinzufügen und dieses Training dann durchführen.").arg(phoneme));
// 		emit missingPhoneme(phoneme);
	} else
	if (err.startsWith("ERROR [+6510]"))  //sample without prompts-entry
	{ //LOpen: Unable to open label file /path/to/missing-sample.lab
// 		err = err.mid(48); //err.left(err.indexOf("\n"));
// ERROR [+6510]  LOpen: Unable to open label file ./tmp//modeltmp/mfcs/Test_1_2008-03-19_13-45-08_2008-03-24_22-23-22.lab

		err = err.left(err.indexOf("\n"));
		QString label = err.mid(48);
		label = label.mid(label.lastIndexOf("/"));
		QString sampleName = label.left(label.count()-4);
		if (!sampleName.isEmpty())
		{
			QString filename = Settings::getS("Model/PathToSamples")+"/"+sampleName+".wav";
			
			emit sampleWithoutWord(filename);
		}
	}
	else if (err.contains("Error:       undefined class \""))
	{
		int startIndex = err.indexOf("Error:       undefined class \"");
		QString undefClass = err.mid(30+startIndex, err.lastIndexOf("\"", startIndex+1)-1);
		emit unknownGrammarClass(undefClass);
	} else
		displayError(userError);

	return false;
}

bool ModelManager::createMonophones()
{
	emit status(tr("Erstelle hmm0..."));
	if (!buildHMM0())
	{
		emit error(tr("Fehler beim Generieren des HMM0. \n\nBitte überprüfen Sie, ob ausreichend Trainingsmaterial vorhanden ist.\n\nSollten Sie sicher sein, das Modell wurde ausreichend trainiert, überprüfen Sie bitte den Pfad zu HCompV (%1), der config (%2) und des Prototypen (%3).").arg(Settings::getS("Programs/HTK/HCompV")).arg(Settings::getS("Model/PathToConfig")).arg(Settings::getS("Model/PathToProto")));
		return false;
	}
	emit progress(550);
	emit status(tr("Erstelle hmm1..."));
	if (!buildHMM1())
	{
		emit error(tr("Fehler beim Generieren des HMM1. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2)").arg(Settings::getS("Programs/HTK/HERest")).arg(Settings::getS("Model/PathToConfig")));
		return false;
	}
	emit progress(800);
	emit status(tr("Erstelle hmm2..."));
	if (!buildHMM2())
	{
		emit error(tr("Fehler beim Generieren des HMM2. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2)").arg(Settings::getS("Programs/HTK/HERest")).arg(Settings::getS("Model/PathToConfig")));
		return false;
	}
	emit progress(850);
	emit status(tr("Erstelle hmm3..."));
	if (!buildHMM3())
	{
		emit error(tr("Fehler beim Generieren des HMM3. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2)").arg(Settings::getS("Programs/HTK/HERest")).arg(Settings::getS("Model/PathToConfig")));
		return false;
	}
	emit progress(900);
	return true;
}

bool ModelManager::fixSilenceModel()
{
	emit status(tr("Erstelle Pausenmodell (hmm4)..."));
	if (!buildHMM4())
	{
		emit error(tr("Fehler beim Generieren des HMM4. Bitte überprüfen Sie das HMM3"));
		return false;
	}
	emit progress(950);
	emit status(tr("Erstelle hmm5..."));
	if (!buildHMM5())
	{
		emit error(tr("Fehler beim Generieren des HMM5. Bitte überprüfen Sie den Pfad zu HHEd (%1) und  des Silence-Modells (%2)").arg(Settings::getS("Programs/HTK/HHEd")).arg(Settings::getS("Model/PathToSilHed")));
		return false;
	}
	emit progress(1000);
	emit status(tr("Erstelle hmm6..."));
	if (!buildHMM6())
	{
		emit error(tr("Fehler beim Generieren des HMM6. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2").arg(Settings::getS("Programs/HTK/HERest")).arg(Settings::getS("Model/PathToConfig")));
		return false;
	}
	emit progress(1080);
	emit status(tr("Erstelle hmm7..."));
	if (!buildHMM7())
	{
		emit error(tr("Fehler beim Generieren des HMM7. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2").arg(Settings::getS("Programs/HTK/HERest")).arg(Settings::getS("Model/PathToConfig")));
		return false;
	}
	emit progress(1150);
	
	return true;
}

bool ModelManager::realign()
{
	emit status(tr("Erstellte dict1..."));
	if (!makeDict1())
	{
		emit error(tr("Fehler beim erstellen des dict1"));
		return false;
	}
	emit progress(1160);

	emit status(tr("Hmm7 neu ausrichten..."));
	if (!realignHMM7())
	{
		emit error(tr("Konnte HMM7 nicht neu ausrichten. Bitte überprüfen Sie den Pfad zu HVite (%1), der config (%2) und das HMM7.").arg(Settings::getS("Programs/HTK/HVite")).arg(Settings::getS("Model/PathToConfig")));
		return false;
	}
	emit progress(1160);

	emit status(tr("Erstelle hmm8..."));
	if (!buildHMM8())
	{
		emit error(tr("Fehler beim Generieren des HMM8. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2").arg(Settings::getS("Programs/HTK/HERest")).arg(Settings::getS("Model/PathToConfig")));
		return false;
	}
	emit progress(1230);

	emit status(tr("Erstelle hmm9..."));
	if (!buildHMM9())
	{
		emit error(tr("Fehler beim Generieren des HMM9. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2").arg(Settings::getS("Programs/HTK/HERest")).arg(Settings::getS("Model/PathToConfig")));
		return false;
	}
	emit progress(1300);
	
	return true;
}

bool ModelManager::tieStates()
{
	emit status(tr("Erstelle triphone..."));
	
	proc->start("\""+Settings::getS("Programs/HTK/HDMan")+"\" -A -D -T 1 -b sp -n \""+tmpDir+"fulllist\" -g \""+Settings::getS("Model/PathToGlobalDed")+"\" \""+tmpDir+"dict-tri\" \""+Settings::getS("Model/PathToLexicon")+"\"");
	proc->waitForFinished(-1);
	if ((proc->exitCode() != 0))
	{
		emit error(tr("Konnte Triphone nicht binden. Bitte überprüfen Sie den Pfad zu HDMan (%1), global.ded (%2) und dem Lexikon (%3).").arg(Settings::getS("Programs/HTK/HDMan")).arg(Settings::getS("Model/PathToGlobalDed")).arg(Settings::getS("Model/PahtToLexicon")));
		return false;
	}
	emit progress(1700);

	emit status(tr("Erstelle Liste der Triphone..."));
	if (!makeFulllist())
	{
		emit error(tr("Konnte Liste der Triphone nicht erstellen."));
		return false;
	}
	emit progress(1705);
	emit status(tr("Erstelle tree.hed..."));
	if (!makeTreeHed())
	{
		emit error(tr("Konnte tree.hed nicht erstellen."));
		return false;
	}
	emit progress(1750);
	
	emit status(tr("Erstelle hmm13..."));
	if (!buildHMM13())
	{
		emit error(tr("Fehler beim Generieren des HMM13. Bitte überprüfen Sie den Pfad zu HHEd (%1).").arg(Settings::getS("Programs/HTK/HHEd")));
		return false;
	}
	emit progress(1830);
	
	
	emit status(tr("Erstelle hmm14..."));
	if (!buildHMM14())
	{
		emit error(tr("Fehler beim Generieren des HMM14. Bitte überprüfen Sie den Pfad zu HERest (%1), der config (%2), und die stats-Datei (%3)").arg(Settings::getS("Programs/HTK/HERest")).arg(Settings::getS("Model/PathToConfig")).arg(tmpDir+"stats"));
		return false;
	}
	emit progress(1900);
	
	emit status(tr("Erstelle hmm15..."));
	if (!buildHMM15())
	{
		emit error(tr("Fehler beim Generieren des HMM15. Bitte überprüfen Sie den Pfad zu HERest (%1), der config (%2), und die stats-Datei (%3)").arg(Settings::getS("Programs/HTK/HERest")).arg(Settings::getS("Model/PathToConfig")).arg(tmpDir+"stats"));
		return false;
	}
	emit progress(1990);

	
	return true;
}

bool ModelManager::buildHMM13()
{
	QString wd = proc->workingDirectory();
	proc->setWorkingDirectory(tmpDir);
	proc->start("\""+Settings::getS("Programs/HTK/HHEd")+"\" -A -D -T 1 -H ./hmm12/macros -H ./hmm12/hmmdefs -M ./hmm13 ./tree.hed ./triphones1");
// 	proc->start("\""+Settings::getS("Programs/HTK/HHEd")+"\" -A -D -T 1 -H \""+tmpDir+"hmm12/macros\" -H \""+tmpDir+"hmm12/hmmdefs\" -M \""+tmpDir+"hmm13\" \""+tmpDir+"tree.hed\" \""+tmpDir+"triphones1\"");
	proc->waitForFinished(-1);
	proc->setWorkingDirectory(wd);
	return (proc->exitCode()==0);
}


bool ModelManager::buildHMM14()
{
	proc->start("\""+Settings::getS("Programs/HTK/HERest")+"\" -A -D -T 1 -C \""+Settings::getS("Model/PathToConfig")+"\" -I \""+tmpDir+"wintri.mlf\" -t 250.0 150.0 3000.0 -s \""+tmpDir+"stats\" -S \""+tmpDir+"train.scp\" -H \""+tmpDir+"hmm13/macros\" -H \""+tmpDir+"hmm13/hmmdefs\" -M \""+tmpDir+"hmm14\" \""+tmpDir+"tiedlist\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelManager::buildHMM15()
{
	proc->start("\""+Settings::getS("Programs/HTK/HERest")+"\" -A -D -T 1 -C \""+Settings::getS("Model/PathToConfig")+"\" -I \""+tmpDir+"wintri.mlf\" -t 250.0 150.0 3000.0 -s \""+tmpDir+"stats\" -S \""+tmpDir+"train.scp\" -H \""+tmpDir+"hmm14/macros\" -H \""+tmpDir+"hmm14/hmmdefs\" -M \""+tmpDir+"hmm15\" \""+tmpDir+"tiedlist\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::makeFulllist()
{	
	QFile::remove(realTmpDir+"fulllist-original");
	if (!QFile::copy(realTmpDir+"fulllist", realTmpDir+"fulllist-original")) return false;
	
	QFile triphones1(realTmpDir+"triphones1");
	QFile fulllist(realTmpDir+"fulllist");
	
	
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
	QFile::remove(realTmpDir+"tree.hed");
	if (!QFile::copy(Settings::getS("Model/PathToTreeHed"), realTmpDir+"tree.hed")) return false;
	
	QFile treeHed(realTmpDir+"tree.hed");
	if (!treeHed.open(QIODevice::WriteOnly|QIODevice::Append)) return false;
	
	QString command = "TB";
	int threshold = 350;
	QFile hmmlist(realTmpDir+"monophones0");
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
	
// 	treeHed.write(QString(" \nTR 1\n \nAU \""+tmpDir+"fulllist\" \nCO \""+tmpDir+"tiedlist\" \n \nST "+tmpDir+"trees \n").toLatin1());
	treeHed.write(QString(" \nTR 1\n \nAU ./fulllist \nCO ./tiedlist \n \nST ./trees \n").toLatin1());
	
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
	emit status(tr("Übernehme Modell..."));

	if (QFile::exists(Settings::getS("Model/PathToHmm")))
		if (!QFile::remove(Settings::getS("Model/PathToHmm"))) return false;
	if (!QFile::copy(realTmpDir+"hmm15/hmmdefs", Settings::getS("Model/PathToHmm")))
		return false;

	if (QFile::exists(Settings::getS("Model/PathToTiedlist")))
		if (!QFile::remove(Settings::getS("Model/PathToTiedlist"))) return false;
	if (!QFile::copy(realTmpDir+"tiedlist", Settings::getS("Model/PathToTiedlist")))
		return false;

	return true;
}

bool ModelManager::makeTriphones()
{
	emit status(tr("Erstelle triphone..."));
	proc->start("\""+Settings::getS("Programs/HTK/HLEd")+"\" -A -D -T 1 -n \""+tmpDir+"/triphones1\" -l * -i \""+tmpDir+"/wintri.mlf\" \""+Settings::getS("Model/PathToMktriLed")+"\" \""+tmpDir+"/aligned.mlf\"");
	proc->waitForFinished(-1);
	if ((proc->exitCode() != 0))
	{
		emit error(tr("Erstellen der Triphone files fehlgeschlagen. Bitte überprüfen Sie ob Sie den Pfad für die Datei mktri.led richtig angegeben haben (%1) und überprüfen Sie den Pfad zu HLEd (%2)").arg(Settings::getS("Model/PathToMktriLed")).arg(Settings::getS("Programs/HTK/HLEd")));
		return false;
	}
	emit progress(1380);
	
	emit status(tr("Erstelle mktri.hed..."));
	if (!makeMkTriHed())
	{
		emit error(tr("Fehler beim generieren der mktri.hed"));
		return false;
	}
	emit progress(1400);
	
	emit status(tr("Erstelle hmm10..."));
	if (!buildHMM10())
	{
		emit error(tr("Fehler beim Generieren des HMM10. Bitte überprüfen Sie den Pfad zu HHEd (%1).").arg(Settings::getS("Programs/HTK/HHEd")));
		return false;
	}
	emit progress(1470);
	
	emit status(tr("Erstelle hmm11..."));
	if (!buildHMM11())
	{
		emit error(tr("Fehler beim Generieren des HMM11. Bitte überprüfen Sie den Pfad zu HERest (%1) und der config (%2)").arg(Settings::getS("Programs/HTK/HERest")).arg(Settings::getS("Model/PathToConfig")));
		return false;
	}
	emit progress(1550);
	
	emit status(tr("Erstelle hmm12..."));
	if (!buildHMM12())
	{
		emit error(tr("Fehler beim Generieren des HMM12. Bitte überprüfen Sie den Pfad zu HERest (%1), der config (%2), und die stats-Datei (%3)").arg(Settings::getS("Programs/HTK/HERest")).arg(Settings::getS("Model/PathToConfig")).arg(realTmpDir+"stats"));
		return false;
	}
	emit progress(1620);
	
	return true;
}


bool ModelManager::buildHMM12()
{
	proc->start("\""+Settings::getS("Programs/HTK/HERest")+"\" -A -D -T 1 -C \""+Settings::getS("Model/PathToConfig")+"\" -I \""+tmpDir+"wintri.mlf\" -t 250.0 150.0 3000.0 -s \""+tmpDir+"stats\" -S \""+tmpDir+"train.scp\" -H \""+tmpDir+"hmm11/macros\" -H \""+tmpDir+"hmm11/hmmdefs\" -M \""+tmpDir+"hmm12\" \""+tmpDir+"triphones1\"");

	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelManager::buildHMM11()
{
	proc->start("\""+Settings::getS("Programs/HTK/HERest")+"\" -A -D -T 1 -C \""+Settings::getS("Model/PathToConfig")+"\" -I \""+tmpDir+"wintri.mlf\" -t 250.0 150.0 3000.0 -S \""+tmpDir+"train.scp\" -H \""+tmpDir+"hmm10/macros\" -H \""+tmpDir+"hmm10/hmmdefs\" -M \""+tmpDir+"hmm11\" \""+tmpDir+"triphones1\"");
	
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelManager::buildHMM10()
{
	proc->start("\""+Settings::getS("Programs/HTK/HHEd")+"\" -A -D -T 1 -H \""+tmpDir+"hmm9/macros\" -H \""+tmpDir+"hmm9/hmmdefs\" -M \""+tmpDir+"hmm10\" \""+tmpDir+"mktri.hed\" \""+tmpDir+"monophones1\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelManager::makeMkTriHed()
{
	QFile monophones1(realTmpDir+"monophones1");
	if (!monophones1.open(QIODevice::ReadOnly))
		return false;
		
	
	QFile mktriHed(realTmpDir+"mktri.hed");
	if (!mktriHed.open(QIODevice::WriteOnly)) return false;
	
	mktriHed.write("CL "+tmpDir.toLatin1()+"triphones1\n");
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
	proc->start("\""+Settings::getS("Programs/HTK/HERest")+"\" -A -D -T 1 -C \""+Settings::getS("Model/PathToConfig")+"\" -I \""+tmpDir+"aligned.mlf\" -t 250.0 150.0 3000.0 -S \""+tmpDir+"train.scp\" -H \""+tmpDir+"hmm8/macros\" -H \""+tmpDir+"hmm8/hmmdefs\" -M \""+tmpDir+"hmm9\" \""+tmpDir+"monophones1\"");
	
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}


bool ModelManager::buildHMM8()
{
	proc->start("\""+Settings::getS("Programs/HTK/HERest")+"\" -A -D -T 1 -C \""+Settings::getS("Model/PathToConfig")+"\" -I \""+tmpDir+"aligned.mlf\" -t 250.0 150.0 3000.0 -S \""+tmpDir+"train.scp\" -H \""+tmpDir+"hmm7/macros\" -H \""+tmpDir+"hmm7/hmmdefs\" -M \""+tmpDir+"hmm8\" \""+tmpDir+"monophones1\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::realignHMM7()
{
	proc->start("\""+Settings::getS("Programs/HTK/HVite")+"\" -A -D -T 1 -l *  -o SWT -b silence -C \""+Settings::getS("Model/PathToConfig")+"\" -H \""+tmpDir+"hmm7/macros\" -H \""+tmpDir+"hmm7/hmmdefs\" -i \""+tmpDir+"aligned.mlf\" -m -t 250.0 150.0 1000.0 -y lab -a -I \""+tmpDir+"words.mlf\" -S \""+tmpDir+"train.scp\" \""+tmpDir+"dict1\" \""+tmpDir+"monophones1\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::makeDict1()
{
	QFile::remove(tmpDir+"dict1");
	if (!QFile::copy(tmpDir+"dict", tmpDir+"dict1")) return false;
	QFile dict1(tmpDir+"dict1");
	if (!dict1.open(QIODevice::WriteOnly|QIODevice::Append)) return false;

	dict1.write("silence  []  sil\n");
	dict1.close();
	return true;
}

bool ModelManager::buildHMM7()
{
	proc->start("\""+Settings::getS("Programs/HTK/HERest")+"\" -A -D -T 1 -C \""+Settings::getS("Model/PathToConfig")+"\" -I \""+tmpDir+"phones1.mlf\" -t 250.0 150.0 3000.0 -S \""+tmpDir+"train.scp\" -H \""+tmpDir+"hmm6/macros\" -H \""+tmpDir+"hmm6/hmmdefs\" -M \""+tmpDir+"hmm7\" \""+tmpDir+"monophones1\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::buildHMM6()
{
	proc->start("\""+Settings::getS("Programs/HTK/HERest")+"\" -A -D -T 1 -C \""+Settings::getS("Model/PathToConfig")+"\" -I \""+tmpDir+"phones1.mlf\" -t 250.0 150.0 3000.0 -S \""+tmpDir+"train.scp\" -H \""+tmpDir+"hmm5/macros\" -H \""+tmpDir+"hmm5/hmmdefs\" -M \""+tmpDir+"hmm6\" \""+tmpDir+"monophones1\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::buildHMM5()
{
	proc->start("\""+Settings::getS("Programs/HTK/HHEd")+"\" -A -D -T 1 -H \""+tmpDir+"hmm4/macros\" -H \""+tmpDir+"hmm4/hmmdefs\" -M \""+tmpDir+"hmm5\" \""+Settings::getS("Model/PathToSilHed")+"\" \""+tmpDir+"monophones1\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::buildHMM4()
{
	QFile::copy(realTmpDir+"hmm3/macros", realTmpDir+"hmm4/macros");

	QStringList  tmp2;

	QFile hmmdefs3(realTmpDir+"hmm3/hmmdefs");
	if (!hmmdefs3.open(QIODevice::ReadOnly)) return false;
	QFile hmmdefs4(realTmpDir+"hmm4/hmmdefs");
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
			hmmdefs4.write(tmp2[2].replace("5", "3").toLatin1());
			hmmdefs4.write(tmp2[9].replace("3", "2").toLatin1());
			hmmdefs4.write(tmp2[10].toLatin1());
			hmmdefs4.write(tmp2[11].toLatin1());
			hmmdefs4.write(tmp2[12].toLatin1());
			hmmdefs4.write(tmp2[13].toLatin1());
			hmmdefs4.write(tmp2[14].toLatin1());
			hmmdefs4.write(tmp2[21].replace("5", "3").toLatin1());
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
	proc->start("\""+Settings::getS("Programs/HTK/HERest")+"\" -A -D -T 1 -C \""+Settings::getS("Model/PathToConfig")+"\" -I \""+tmpDir+"phones0.mlf\" -t 250.0 150.0 1000.0 -S \""+tmpDir+"train.scp\" -H \""+tmpDir+"hmm2/macros\" -H \""+tmpDir+"hmm2/hmmdefs\" -M \""+tmpDir+"hmm3\" \""+tmpDir+"monophones0\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::buildHMM2()
{
	proc->start("\""+Settings::getS("Programs/HTK/HERest")+"\" -A -D -T 1 -C \""+Settings::getS("Model/PathToConfig")+"\" -I \""+tmpDir+"phones0.mlf\" -t 250.0 150.0 1000.0 -S \""+tmpDir+"train.scp\" -H \""+tmpDir+"hmm1/macros\" -H \""+tmpDir+"hmm1/hmmdefs\" -M \""+tmpDir+"hmm2\" \""+tmpDir+"monophones0\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::buildHMM1()
{
	proc->start("\""+Settings::getS("Programs/HTK/HERest")+"\" -A -D -T 1 -C \""+Settings::getS("Model/PathToConfig")+"\" -I \""+tmpDir+"phones0.mlf\" -t 250.0 150.0 1000.0 -S \""+tmpDir+"train.scp\" -H \""+tmpDir+"hmm0/macros\" -H \""+tmpDir+"hmm0/hmmdefs\" -M \""+tmpDir+"hmm1\" \""+tmpDir+"monophones0\"");
	proc->waitForFinished(-1);
	return (proc->exitCode()==0);
}

bool ModelManager::buildHMM0()
{
	proc->start("\""+Settings::getS("Programs/HTK/HCompV")+"\" -A -D -T 1 -C \""+Settings::getS("Model/PathToConfig")+"\" -f 0.01 -m -S \""+tmpDir+"train.scp\" -M \""+tmpDir+"hmm0\" \""+Settings::getS("Model/PathToProto")+"\"");
	proc->waitForFinished(-1);
	if (proc->exitCode()!=0) return false;

	QString protoBody="";
	QString protoHead="";
	QString line;
	QFile protoFile(realTmpDir+"hmm0/proto");
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
	QFile monophones0(tmpDir+"monophones0");
	if (!monophones0.open(QIODevice::ReadOnly)) return false;

	while (!monophones0.atEnd())
		monophones.append(monophones0.readLine(50).trimmed());
	monophones0.close();
	
	Logger::log(tr("[INF] Verwendete Monophone des Modells: %1").arg(monophones.join(", ")));

	QFile hmmdefs(realTmpDir+"hmm0/hmmdefs");
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
	QFile macros(realTmpDir+"hmm0/macros");
	if (!macros.open(QIODevice::WriteOnly)) return false;
	macros.write(protoPreamble.toLatin1());

	QFile vFloors(realTmpDir+"hmm0/vFloors");
	if (!vFloors.open(QIODevice::ReadOnly)) return false;
	while (!vFloors.atEnd()) macros.write(vFloors.readLine(1000));
	vFloors.close();

	macros.close();

	return true;
}

bool ModelManager::makeMonophones()
{
	//make monophones1
	proc->start("\""+Settings::getS("Programs/HTK/HDMan")+"\" -A -D -T 1 -m -w \""+tmpDir+"/wlist\" -g \""+Settings::getS("Model/PathToGlobalDed")+"\" -n \""+tmpDir+"/monophones1\" -i \""+tmpDir+"/dict\" \""+Settings::getS("Model/PathToLexicon")+"\"");
	proc->waitForFinished(-1);
	if (proc->exitStatus()!=0) return false;

	//make monophones0
	//ditch the "sp" phoneme

	QFile monophones1(realTmpDir+"/monophones1");
	QFile monophones0(realTmpDir+"/monophones0");
	if (!monophones1.open(QIODevice::ReadOnly))
		return false;
	if (!monophones0.open(QIODevice::WriteOnly|QIODevice::Truncate))
		return false;

	QString phoneme;
	while (!monophones1.atEnd())
	{
		phoneme = monophones1.readLine(50);
		if ((phoneme.trimmed() != "sp") && (phoneme.trimmed() != ""))
			monophones0.write(phoneme.toLatin1());
	}
	monophones1.close();
	monophones0.close();
	return true;
}

void ModelManager::logInfo()
{
	lastOutput = proc->readAllStandardOutput();
	Logger::log(tr("[INF]")+" "+lastOutput);
}

bool ModelManager::generateWlist()
{
	QFile promptsFile(Settings::getS("Model/PathToPrompts"));
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
	
	QFile wlistFile(Settings::getS("TempDir")+"/modeltmp/wlist");
	if (!wlistFile.open(QIODevice::WriteOnly))
		return false;
	for (int i=0; i < words.count(); i++)
	{
		wlistFile.write(words[i].toLatin1()+"\n");
	}
	wlistFile.close();
	return true;
}

bool ModelManager::generateMlf()
{
	QFile promptsFile(Settings::getS("Model/PathToPrompts"));
	QFile mlf(realTmpDir+"words.mlf");

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
    processDialog->deleteLater();
    proc->deleteLater();
}

