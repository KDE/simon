//
// C++ Interface: trainingmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRAININGMANAGER_H
#define TRAININGMANAGER_H

#include <QDir>
#include "math.h"
#include "trainingtext.h"
#include "xmltrainingtext.h"
#include "word.h"

/**
 *	@class TrainingManager
 *	@brief Class to handle the process of training the language model
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 *	@todo Implementing
 */
class TrainingManager{
private:
	TrainingList *trainingTexts;
	TrainingText *currentText;
public:
	TrainingManager(QString pathToTexts="texts/");

	TrainingList* readTrainingTexts(QString pathToTexts="texts/");
	
	/**
	 * @brief Getter method for the QList of training texts
	 *
	 * @return TrainingList*
	 * The TrainingList (member)
	 */
	TrainingList* getList()
	{
		return trainingTexts;
	}
	
	
	QString getLabel(int i)
	{
		return ""; //just to supress these silly compiler
		//warnings that make me mad
	}
	
	bool deleteText(int index);
	
	int calcRelevance(TrainingText *text, WordList *wlist);
	
	void trainWords(WordList *words);
	
	void abortTraining();
	void pauseTraining();
	void resumeTraining();
	
	bool trainText(int i);
	int getPageCount();
	QString getPage(int i);
	QString getTextName();
	TrainingText* getText(int i);

	~TrainingManager();

};

#endif
