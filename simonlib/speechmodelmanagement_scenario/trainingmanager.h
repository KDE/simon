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

#include <speechmodelbase_scenario/word.h>
#include <speechmodelbase_scenario/trainingtext.h>
#include <QHash>
#include <QMutex>
#include <QList>
#include "simonmodelmanagement_export.h"


/**
 *	@class TrainingManager
 *	@brief Class to handle the process of training the language model
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */

typedef QHash<QString, QString> PromptsTable;
typedef  QList<TrainingText*> TrainingList;

class TrainingManager;

class MODELMANAGEMENT_EXPORT TrainingManager : public QObject
{
Q_OBJECT
	private:
		bool dirty;

		static TrainingManager *instance;
		TrainingList *trainingTexts;

		QHash<QString,int> wordRelevance; /// Stores the relevance of words (caching for getProbability)

		QMutex promptsLock;
		PromptsTable *promptsTable;

		bool deletePrompt ( QString key );

	signals:
		void addMissingWords(QStringList words);
		void trainingFinished();
		void trainingDataChanged();
		void trainingSettingsChanged();

	protected:
		TrainingManager(QObject *parent=0);

	public:
		QString htkify(const QString& in);
		bool saveTrainingsText(const QString& name, const QStringList pages);
		//TODO: Port me to friends for the modelmanager
		PromptsTable* getPrompts();
		bool writePromptsFile(PromptsTable* prompts, QString path);
		//end

		static TrainingManager* getInstance();

		bool init();

		int getProbability ( QString name, PromptsTable *promptsTable );
		TrainingList* readTrainingTexts ();

		int getProbability ( QString name );
		PromptsTable* readPrompts ( QString pathToPrompts );

		bool deleteWord ( Word *w );
		bool savePrompts();

		bool refreshTraining(int sampleRate, const QByteArray& prompts);

		bool addSample(const QString& fileBaseName, const QString& prompt);
		bool removeSample(const QString& fileBaseName);

		QStringList missingWords(const QStringList& prompts);

		QString getPage ( int i );

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


		bool deleteText ( int index );

		float calcRelevance ( TrainingText *text );
		QString getTrainingDir();

		void trainWords ( const WordList *words );

		bool trainText ( int i );
		TrainingText* getText ( int i );

		void trainingSettingsSaved();

		~TrainingManager();

};

#endif
