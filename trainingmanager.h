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
#include <QHash>
#include "trainingtext.h"
#include "xmltrainingtext.h"
#include "word.h"
#include <QDebug>
#include <QMessageBox>
#include <QMutex>
class WordListManager;

/**
 *	@class TrainingManager
 *	@brief Class to handle the process of training the language model
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
typedef QHash<QString, QString> PromptsTable;
class TrainingManager{
private:
	TrainingList *trainingTexts;
	TrainingText *currentText;
	QString filename;
	QMutex promptsLock;
public:
	PromptsTable *promptsTable;
	TrainingManager();

	int getProbability(QString name);
	TrainingList* readTrainingTexts(QString pathToTexts="");
	PromptsTable* readPrompts(QString pathToPrompts);
	PromptsTable* getPrompts();

	bool deleteWord(Word *w);
	bool deletePrompt(QString key);
	bool savePrompts();
	
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
		return QString::number(i); //just to supress these silly compiler
		//warnings that make me mad
	}
	
	bool deleteText(int index);
	
	float calcRelevance(TrainingText *text);
	
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
