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
#include <QHash>
#include <QDebug>
#include <QMessageBox>
#include <QMutex>
#include "addwordview.h"
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
    WordListManager *wlistmgr;
	QString filename;
    QHash<QString, QString> *sampleHash;    //<! sampleHash("SampleName","Text");
	QMutex promptsLock;
    AddWordView *addWordView;
public:
	PromptsTable *promptsTable;
	TrainingManager();

    QHash<QString, QString>* getSampleHash(){return sampleHash;}
	int getProbability(QString name, PromptsTable *promptsTable);
	TrainingList* readTrainingTexts(QString pathToTexts="texts");

	int getProbability(QString name);
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
    
    void setupTrainingSession();
    bool allWordsExisting();
    void setWordListManager(WordListManager *wlistmgr);
    void addSamples(QHash<QString, QString> *hash);
    void setAddWordView(AddWordView *addWordView);
    
    void writePrompts(QString text);

	~TrainingManager();

};

#endif
