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
		XMLDocument *text = new XMLDocument( pathToTexts+textsrcs.at(i) );
		text->load();
		trainingTexts->append(new TrainingText(text->getTitle(), text->getAllPages(),3.5));
	}
	
	return trainingTexts;
}

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


