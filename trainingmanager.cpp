//
// C++ Implementation: trainingmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "trainingmanager.h"
#include "settings.h"
#include "logger.h"
#include "settings.h"
#include "wordlistmanager.h"
#include "math.h"
#include <QObject>
#include "addwordview.h"

/**
 * @brief Constructor
 *
 *	@author Peter Grasch
 */
TrainingManager::TrainingManager(AddWordView *addWordView)
{

	filename = Settings::getS("PathToTexts");
	promptsLock.lock();
	this->promptsTable = readPrompts(Settings::getS("Model/PathToPrompts"));
	promptsLock.unlock();
    sampleHash = new QHash<QString, QString>();
    addWordView = addWordView;
  
}

void TrainingManager::setWordListManager(WordListManager *wlistmgr)
{
    this->wlistmgr = wlistmgr;
}

bool TrainingManager::deleteWord(Word *w)
{
	QString wordToDelete = w->getWord().toUpper();
	
	promptsLock.lock();
	//TODO: For now we delete every word with the same name
	//For the future we should implement a lookup which tries to resolve the pronunciation using the samples
	//and looking up if this is the selected word
	QStringList sampleFileNames = promptsTable->keys();
// 	QStringList samplesToDelete;
	for (int i=0; i < this->promptsTable->count(); i++)
	{
		QString filename = sampleFileNames[i];
		QStringList promptWords = promptsTable->value(filename).split(" ");
		for (int j=0; j < promptWords.count(); j++)
		{
			if (promptWords[j].toUpper() == wordToDelete)
			{
// 				samplesToDelete << filename;
// 				promptsTable->remove(filename);
				if (!deletePrompt(filename)) return false;
			}
		}
	}
	promptsLock.unlock();
	
// 	for (int i=0; i < samplesToDelete.count(); i++) qDebug() <<samplesToDelete[i];
	return true;
}

bool TrainingManager::deletePrompt(QString key)
{
// 	int index = promptsTable->keys().indexOf(key);
	promptsLock.lock();
	promptsTable->remove(key);
	promptsLock.unlock();
	
	//removes the sample
	QFile::remove(Settings::getS("Model/PathToSamples")+"/"+key+".wav");
	return savePrompts();
}

bool TrainingManager::savePrompts()
{
	QFile prompts(Settings::getS("Model/PathToPrompts"));
	if (!prompts.open(QIODevice::WriteOnly)) return false;
	
	promptsLock.lock();
	QStringList samples = this->promptsTable->keys();
	
	for (int i=0; i <samples.count(); i++)
		prompts.write(samples[i].toLatin1()+" "+promptsTable->value(samples[i]).toLatin1()+"\n");
    
    
    
    
	promptsLock.unlock();
	
	prompts.close();
	return true;
}

PromptsTable* TrainingManager::getPrompts()
{
	return this->promptsTable;
}


/**
 * \brief Builds and returns the promptstable by parsing the file at the given path
 * \author Peter Grasch
 * \param QString promptspath
 * The path to the prompts file
 * \return PromptsTable*
 * The table
 */
PromptsTable* TrainingManager::readPrompts(QString promptspath)
{
	Logger::log(QObject::tr("[INF] Parse Promptsdatei von %1").arg(promptspath));
	PromptsTable *promptsTable = new PromptsTable();
	
	QFile *prompts = new QFile ( promptspath );
	prompts->open ( QFile::ReadOnly );
	if ( !prompts->isReadable() ) return new PromptsTable();
	
	QString label;
	QString prompt;
	QString line;
	int labelend;
	while ( !prompts->atEnd() ) //for each line that was successfully read
	{
		line = prompts->readLine(1024);
		labelend = line.indexOf(" ");
		label = line.left(labelend);
		prompt = line.mid(labelend).trimmed();

		promptsTable->insert( label, prompt );
	}
	Logger::log(QCoreApplication::tr("[INF] %1 Prompts gelesen").arg(promptsTable->count()));
	return promptsTable;
}

/**
 * \brief Aborts the rebuilding of the language model and cleans up
 * \author Peter Grasch
 */
void TrainingManager::abortTraining()
{
	
}

/**
 * \brief Creates a training text and sets it to be the current text
 * \author Peter Grasch
 */
void TrainingManager::trainWords(WordList *words)
{
	if (!words) return;
	
	Logger::log(QObject::tr("[INF] Starten eines  on-the-fly Trainings mit %1 Wörter").arg(words->count()));
	
	QStringList pages;
	
	//we try to guess the perfect amount of words/page
	//We first go through the possible words/page word counts from 5 to 12
	//If we find a perfect match (means we have x pages with the /same/ amount of words
	//on them, we found a perfect words/page value
	//If not, we try to find the w/p count that leaves the last page with the closest
	//value of w/p than the others.
	//for example if we have given 37 words, we would end up with the following:
	//
	//	+---------+-----------------+
	//	| w/p     |    w/p last page|
	//	+---------------------------+
	//	|  5      |         2	    |
	//	|  6      |         1	    |
	//	|  7      |         2	    |
	//	|  8      |         5	    |
	//	|  9      |         1	    |
	//	|  10     |         7	    |
	//	|  11     |         4	    |
	//	|  12     |         1	    |
	//	+---------+-----------------+
	//
	//In this case the perfect amount of w/p would be 10 because even the last page
	//would have enough words for perfect accuracy
	
	short wordCount = words->count();
	short wordsPerPage=5;
	
	short maxLeftOver=0;
	short leftOverWordsPerPage=5;
	
	while((wordCount%wordsPerPage != 0) && (wordsPerPage <=12))
	{
		if (wordCount%wordsPerPage > maxLeftOver) 
		{
			maxLeftOver = wordCount%wordsPerPage;
			leftOverWordsPerPage = wordsPerPage;
		}
		
		wordsPerPage++;
	}
	if (wordsPerPage==13) wordsPerPage=leftOverWordsPerPage;
	
	QString page;
	for (int i=0; i< ceil((double)wordCount/wordsPerPage); i++)
	{
		page="";
		for (int j=0; (j<wordsPerPage) && (j+(i*wordsPerPage) < wordCount); j++)
		{
			page += words->at(j+(i*wordsPerPage)).getWord()+QString(" ");
		}
		
		pages.append(page);
	}
	
	TrainingText *newText = new TrainingText( QObject::tr("Spezialisiertes Training"),
						"", pages);
	
	currentText=newText;
}

/**
 * \brief Pauses the training progress
 * Holds the process and waits for the signal to resume - or not;
 * This is useful if the user aborts the process and we want to ask him if he really
 * meant to;
 * \see resumeTraining
 * \author Peter Grasch
 */
void TrainingManager::pauseTraining()
{
	
}

/**
 * \brief Resumes the training progress
 * Resumes the training after a pause using pauseTraining()
 * \see pauseTraining
 * \author Peter Grasch
 */
void TrainingManager::resumeTraining()
{
	
}

/**
 * \brief Deletes the given file from the harddrive
 * \author Peter Grasch
 * \param int index
 * The index of the text to delete
 */
bool TrainingManager::deleteText(int index)
{
	Logger::log(QObject::tr("[INF] Löschen von \"%1\" von \"%2\"").arg(trainingTexts->at(index)->getName()).arg(trainingTexts->at(index)->getPath()));
	QFile text(trainingTexts->at(index)->getPath());
	return text.remove();
}

/**
 * @brief Read the Training Texts and returns the list
 *
 * @return TrainingList*
 * The TrainingList (member)
 */
TrainingList* TrainingManager::readTrainingTexts(QString pathToTexts)
{
	if (pathToTexts.isEmpty()) pathToTexts=this->filename;

	if (pathToTexts.isEmpty())
	{
		QMessageBox::critical(0, QCoreApplication::tr("Fehler beim Auslesen der Trainingstexte"), QCoreApplication::tr("Der Pfad zu den Trainingstexten ist nicht richtig konfiguriert. (Er ist leer)\n\nBitte setzen Sie einen korrekten Pfad in den Einstellungen."));
		return new TrainingList();
	}
	Logger::log(QObject::tr("[INF] Lesen der Trainingtexte von \"")+pathToTexts+"\"");
	QDir *textdir = new QDir(pathToTexts);
	if (!textdir || !textdir->exists()) return NULL;
	QStringList filter;
	textdir->setFilter(QDir::Files|QDir::Readable);
	
	QStringList textsrcs = textdir->entryList();
	
	trainingTexts = new TrainingList();
	for (int i=0; i < textsrcs.count(); i++)
	{
		XMLTrainingText *text = new XMLTrainingText( pathToTexts+"/"+textsrcs.at(i) );
		text->load(pathToTexts+"/"+textsrcs.at(i));
		TrainingText *newText = new TrainingText(text->getTitle(), 
				      pathToTexts+"/"+textsrcs.at(i),
				      text->getAllPages());
		newText->setRelevance(calcRelevance(newText));
		trainingTexts->append(newText);
	}
	
	return trainingTexts;
}

/**
 * \brief Marks text at the given index as the one we are training now
 * Stores a pointer of the text in the member currentText
 * \author Peter Grasch
 * \param int i
 * The index
 * \return bool
 * Success?
 */
bool TrainingManager::trainText(int i)
{
	Logger::log(QObject::tr("[INF] Training Text: \"")+getText(i)->getName()+"\"");
	this->currentText = getText(i);
    bool allWordsInDict = allWordsExisting();
    if(!allWordsInDict)
        return false;
    QString textName = getTextName();
    textName.replace(QString(" "), QString("_"));
    QString time = qvariant_cast<QString>(QTime::currentTime());
    time.replace(QString(":"), QString("-"));
    
    for(int i=0; i<getPageCount(); i++)
    {
        QString hashKey = QString::number(i+1);
        sampleHash->insert(hashKey, (textName+"_S"+QString::number(i+1)+"_"+QDate::currentDate().toString("yyyy-MM-dd")+"_"+time));
    }
	return (currentText != NULL);
}

/**
 * \brief chechs if all words in the dict
 * \author Susanne Tschernegg
 */
bool TrainingManager::allWordsExisting()
{

    for(int x=0; x<getPageCount(); x++)
    {
        QStringList strList = getPage(x).split(" ");
        for(int y=0; y<strList.size(); y++)
        {
            QString word = strList.at(y);
            word.trimmed();
            word.remove(".");
            word.remove(",");
            word.remove("(");
            word.remove(")");
            word.remove("?");
            word.remove("!");
            word.remove("\"");
            word.remove("/");
            word.remove("\\");
            word.remove("[");
            word.remove("]");
            WordList* words = wlistmgr->getWords(word, false);
            if(words==NULL)
            {
                //AddWordView *addWordView = new AddWordView(0, wlistmgr, );
                //addWordView->createWord(word);
                //addwordview, von simonview übergeben
                addWordView->show();
                addWordView->createWord(word);
                return false;
            }
        }
    }
    return true;
}

/**
 * \brief This is used to get the page <i> of the currently training text (stored in the currentText member)
 * \author Peter Grasch
 * \param int i
 * The index
 * \return QString
 * The text of the page <i>
 */
QString TrainingManager::getPage(int i)
{
	if (!currentText) return "";
	return currentText->getPage(i);
}

/**
 * \brief Returns the pagecount of the currently training text
 * \author Peter Grasch
 * \return int
 * count of pages
 */
int TrainingManager::getPageCount()
{
	if (!currentText) return 0;
	return currentText->getPageCount();
}

/**
 * \brief Returns the name of the currently trained text
 * \author Peter Grasch
 * \return QString
 * Name of the text
 */
QString TrainingManager::getTextName()
{
	if (!currentText) return "";
	return currentText->getName();
}

/**
 * \brief Returns the Text <i>
 * \author Peter Grasch
 * \param int i
 * The index of the text
 * \return TraininText*
 * Pointer to the TrainingText
 */
TrainingText* TrainingManager::getText(int i)
{
	if (this->trainingTexts)
		return this->trainingTexts->at(i);
	else return NULL;
}
		


/**
 * \brief Calculates the relevance of the given text with the given wordlist
 * \author Peter Grasch, Susanne Tschernegg
 * @param text The text to check
 * @param wlist The wordlist as reference
 * @return An index of how well simon would recognize it - the lower the worse
 */
float TrainingManager::calcRelevance(TrainingText *text)
{
	Logger::log(QObject::tr("[INF] Berechne Nutzen des Textes ")+"\""+text->getName()+"\" ("+
		text->getPath()+")");
	QString currPage;
	QStringList words;
	
	int wordCount=0;
	int probability=0;
	for (int i=0; i<text->getPageCount();i++)
	{
		currPage = text->getPage(i);
		currPage.remove(".");
		currPage.remove(",");
		currPage.remove("?");
		currPage.remove("!");
		currPage.remove("\"");
		currPage.remove("/");
		currPage.remove("[");
		currPage.remove("]");
		
		words = currPage.split(" ");
        //wlistmgr->addWords(words);
 		
		for (int j=0; j<words.count(); j++)
		{
			wordCount++;
			promptsLock.lock();
			probability += getProbability(words.at(j));
			promptsLock.unlock();
		}
	}
	if (wordCount > 0)
		return round(probability/wordCount);
	else return 0;
}

/**
 * @brief compiles the model
 *
 *	@author Susanne Tschernegg
 */
void TrainingManager::setupTrainingSession()
{
    //for (int i=1; i < getPageCount()+1; i++)
    //{
        //QList<QFile> files = f.findChildren(
        
        //QDir dir(Settings::getS("Model/PathToSamples")); 
        /*QDir dir("C:/Dokumente und Einstellungen/Susi/Eigene Dateien/schule/diplomarbeit/Diplomarbeit/trunk"); 
        QStringList list = dir.entryList(QDir::Files);
        QString textName = getTextName();
        textName.replace(QString(" "), QString("_"));
        
        QStringList filteredList = list.filter(QRegExp(QString(textName+"_S"+QString::number(i)+"_"+QDate::currentDate().toString("yyyy-MM-dd")+"_.wav")));*/
        //QStringList filteredList = list.filter(QRegExp(".wav"));
        
        /*for ( int j=0; j<filteredList.size(); j++)
        {
            QString fileName = filteredList.at(j);
            fileName.remove(QString(".wav"), Qt::CaseInsensitive);
            QString line =  fileName + " " + getPage(i);    //filename + text
            wlistmgr->writePrompts(line);
        }*/
    //}
    addSamples(sampleHash);
    bool success = wlistmgr->compileModel();
}


/**
 * \brief Returns the probability of the name (it is pulled out of the promptsTable)
 * \author Peter Grasch
 * \param QString wordname
 * Name of the word
 * \return int
 * Probability to recognize; The higher the more likly simon will recognize this word correctly
 */
int TrainingManager::getProbability(QString wordname)
{
	QStringList prompts = promptsTable->values();
	int prob=0;
	int i=0;
	QString line;
	while (i<prompts.count())
	{
		prob += prompts.at(i).count(wordname);
		i++;
	}
	return prob;
}

/**
 * @brief Adds the Samples to the prompts-file.
 *
 *	@author Susanne Tschernegg
 */
void TrainingManager::addSamples(QHash<QString, QString> *hash)
{
    QHashIterator<QString, QString> hIterator(*hash);
    hIterator.toFront();
    while (hIterator.hasNext())
    {
        hIterator.next();
        wlistmgr->writePrompts(hIterator.value() + " " + getPage(hIterator.key().toInt()-1));
    }
    hash->clear();
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingManager::~TrainingManager()
{
}


