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

/**
 * @brief Constructor
 *
 *	@author Peter Grasch
 */
TrainingManager::TrainingManager(QString pathToTexts)
{
	
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
 * \todo The relevance is 0 (hardcoded); We allways assume the first pronunciation
 */
void TrainingManager::trainWords(WordList *words)
{
	if (!words) return;
	
	QStringList pages;
	QStringList labels;
	
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
	QString label;
	for (int i=0; i< ceil((double)wordCount/wordsPerPage); i++)
	{
		page="";
		label="sil ";
		for (int j=0; (j<wordsPerPage) && (j+(i*wordsPerPage) < wordCount); j++)
		{
			page += words->at(j+(i*wordsPerPage))->getWord()+QString(" ");
			if (words->at(j+(i*wordsPerPage))->getPronunciation(0)) 
				label += *(words->at(j+(i*wordsPerPage))->getPronunciation(0))+QString(" sp ");
		}
		label += " sil";
		
		pages.append(page);
		labels.append(label);
	}
	
	labels.append("sil");
	
	TrainingText *newText = new TrainingText( "Spezialisiertes Training",
						"", pages, labels, 0);
	
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
	QFile text(trainingTexts->at(index)->getPath());
	text.remove();
}

/**
 * @brief Read the Training Texts and returns the list
 *
 * @return TrainingList*
 * The TrainingList (member)
 */
TrainingList* TrainingManager::readTrainingTexts(QString pathToTexts)
{
	QDir *textdir = new QDir(pathToTexts);
	if (!textdir || !textdir->exists()) return NULL;
	QStringList filter;
	textdir->setFilter(QDir::Files|QDir::Readable);
	
	QStringList textsrcs = textdir->entryList();
	
	trainingTexts = new TrainingList();
	for (int i=0; i < textsrcs.count(); i++)
	{
		XMLTrainingText *text = new XMLTrainingText( pathToTexts+textsrcs.at(i) );
		text->load();
		trainingTexts->append(new TrainingText(text->getTitle(), 
				      pathToTexts+textsrcs.at(i),
				      text->getAllPages(),text->getAllLabels(), 3.5));
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
	this->currentText = getText(i);
	return (currentText != NULL);
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
		


int TrainingManager::calcRelevance(TrainingText *text, WordList *wlist)
{
	
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingManager::~TrainingManager()
{
}


