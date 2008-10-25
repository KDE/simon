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


#ifndef TRAININGMANAGER_H
#define TRAININGMANAGER_H

#include <speechmodelbase/word.h>
#include <QHash>
#include <QMutex>
#include <QList>
#include "xmltrainingtext.h"
#include "simonacousticmodelmanagement_export.h"


/**
 *	@class TrainingManager
 *	@brief Class to handle the process of training the language model
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */

typedef QHash<QString, QString> PromptsTable;
typedef  QList<XMLTrainingText*> TrainingList;

class ACOUSTICMODELMANAGEMENT_EXPORT TrainingManager : public QObject
{
Q_OBJECT
	private:
		static TrainingManager *instance;
		TrainingList *trainingTexts;
		TrainingText *currentText;
		QHash<QString, QString> sampleHash;    //<! sampleHash("SampleName","Text")
		QHash<QString,int> wordRelevance; /// Stores the relevance of words (caching for getProbability)
		QMutex promptsLock;
		PromptsTable *promptsTable;

	private slots:
		void askDeleteLonelySample(QString);
		bool initPrompts();

	signals:
		void addMissingWords(QStringList words);
		void trainingFinished();
		void trainingDataChanged();
		void trainingSettingsChanged();
// 		void promptsChanged();

	protected:
		TrainingManager(QObject *parent=0);

	public:
		static TrainingManager* getInstance();

		bool init();

		QHash<QString, QString> getSampleHash() {return sampleHash;}
		int getProbability ( QString name, PromptsTable *promptsTable );
		TrainingList* readTrainingTexts ();

		int getProbability ( QString name );
		PromptsTable* readPrompts ( QString pathToPrompts );
		PromptsTable* getPrompts();

		bool deleteWord ( Word *w, bool recompiledLater=false );
		bool deletePrompt ( QString key );
		bool savePrompts(bool recompiledLater=false);
		bool writePromptsFile(PromptsTable* prompts, QString path);

// 		QHash<QString, QString> getTransferTrainingMap();
// 		QHash<QString, QString> getTrainingsDataHashes();

		bool refreshTraining(int soundChannels, int sampleRate, const QByteArray& prompts);

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
			return QString::number ( i ); //just to surpress these silly compiler
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
		void addSamples ( const QHash<QString, QString>& trainingsMap, bool recompiledLater=false );

		void clearSampleHash() { sampleHash.clear(); }

		void trainingSettingsSaved();

		~TrainingManager();

};

#endif
