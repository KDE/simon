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

#include "trainingtext.h"
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
public:
	TrainingManager();

	TrainingList* readTrainingTexts();
	TrainingList* getList();
	int calcRelevance(TrainingText *text, WordList *wlist);

	~TrainingManager();

};

#endif
