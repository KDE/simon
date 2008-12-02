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


#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "simonmodelmanagement_export.h"
#include <QObject>
#include <QDateTime>
#include <QStringList>

class Model;
class WordListContainer;
class GrammarContainer;
class LanguageDescriptionContainer;
class TrainingContainer;

class MODELMANAGEMENT_EXPORT ModelManager : public QObject
{
	Q_OBJECT
	
	signals:
		void modelChanged();
		
	private:
		bool inGroup;
		bool modelChangedFlag;

		QStringList missingFiles;
		bool hasWordList();
		bool hasTraining();
		bool hasGrammar();
		bool hasLanguageDescription();
		bool hasActiveContainer();
	
	private slots:
		void slotModelChanged();

	protected:
		static ModelManager* instance;
	
	public:
		static ModelManager* getInstance(QObject *parent=0)
		{
			if (!instance) instance = new ModelManager(parent);
			return instance;
		}
	
		ModelManager(QObject *parent=0);
		void startGroup();
		void commitGroup(bool silent=false);

		QDateTime getSrcContainerModifiedTime();
		
		WordListContainer* getWordListContainer();
		QDateTime getWordListModifiedTime();
		bool storeWordList(const QDateTime& changedTime, const QByteArray& simpleVocab,
					const QByteArray& activeVocab, const QByteArray& activeLexicon);
		
		GrammarContainer* getGrammarContainer();
		QDateTime getGrammarModifiedTime();
		bool storeGrammar(const QDateTime& changedTime, const QByteArray& grammarStructures);
		
		LanguageDescriptionContainer* getLanguageDescriptionContainer();
		QDateTime getLanguageDescriptionModifiedTime();
		bool storeLanguageDescription(const QDateTime& changedTime, const QByteArray& shadowVocab, 
				        const QByteArray& treeHed);
		
		TrainingContainer* getTrainingContainer();
		QDateTime getTrainingModifiedTime();
		bool storeTraining(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& wavConfig,
					const QByteArray& prompts);

		Model* createActiveContainer();
		qint32 getActiveModelSampleRate();
		QDateTime getActiveContainerModifiedTime();
		bool storeActiveModel(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& hmmDefs,
				const QByteArray& tiedList, const QByteArray& dict, const QByteArray& dfa);
				
		void buildMissingSamplesList();
		QByteArray getSample(const QString& sampleName);
		QString missingSample();
		bool storeSample(const QByteArray& sample);

		virtual ~ModelManager() {}
		
};

#endif
