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

#include "word.h"
#include <QHash>
#include <QMutex>
#include "trainingtext.h"

class AddWordView;

/**
 *	@class TrainingManager
 *	@brief Class to handle the process of training the language model
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
typedef QHash<QString, QString> PromptsTable;
class TrainingManager
{
	private:
		static TrainingManager *instance;
		TrainingList *trainingTexts;
		TrainingText *currentText;
		QHash<QString, QString> *sampleHash;    //<! sampleHash("SampleName","Text");
		QMutex promptsLock;
	public:
		static TrainingManager* getInstance();
		PromptsTable *promptsTable;
		TrainingManager();

		QHash<QString, QString> *getSampleHash() {return sampleHash;}
		int getProbability ( QString name, PromptsTable *promptsTable );
		TrainingList* readTrainingTexts ();

		int getProbability ( QString name );
		PromptsTable* readPrompts ( QString pathToPrompts );
		PromptsTable* getPrompts();

		bool deleteWord ( Word *w, bool recompiledLater=false );
		bool deletePrompt ( QString key );
		bool savePrompts(bool recompiledLater=false);

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


		QString getLabel ( int i )
		{
			return QString::number ( i ); //just to supress these silly compiler
			//warnings that make me mad
		}

		bool deleteText ( int index );

		float calcRelevance ( TrainingText *text );

		void trainWords ( WordList *words );

		bool trainText ( int i );
		int getPageCount();
		QString getPage ( int i );
		QString getTextName();
		TrainingText* getText ( int i );

		void finishTrainingSession();
		bool allWordsExisting();
		void addSamples ( QHash<QString, QString> *hash );

		void writePrompts ( QString text );

		~TrainingManager();

};

#endif
