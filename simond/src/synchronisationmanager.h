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

#ifndef SYNCHRONISATIONMANAGER_H
#define SYNCHRONISATIONMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QStringList>
#include <QMap>

class Model;
class WordListContainer;
class GrammarContainer;
class LanguageDescriptionContainer;
class TrainingContainer;

class SynchronisationManager : public QObject
{
	Q_OBJECT

	private:
		QString username;
		QStringList missingFiles;
		//QString currentSrcContainerPath;
		//QDateTime currentSrcContainerDate;
		QString srcContainerTempPath;
		bool cleanTemp();

		QString getLatestPath(const QMap<QDateTime, QString>& models);

		QMap<QDateTime, QString> getWordLists(); //lexicon, vocab
		QMap<QDateTime, QString> getGrammars(); //grammars
		QMap<QDateTime, QString> getTrainingDatas(); //prompts, wavconfig
		QMap<QDateTime, QString> getLanguageDescriptions(); //treehed

		QString getLatestWordListPath();
		QString getLatestGrammarPath();
		QString getLatestLanguageDescriptionPath();
		QString getLatestTrainingPath();
	
	public:
		SynchronisationManager(const QString& username, QObject *parent=0);

		QMap<QDateTime, QString> getModels();

		Model* getActiveModel();
		bool hasActiveModel();
		QDateTime getActiveModelDate();
		void setActiveModelSampleRate(int activeModelSampleRate);
		bool storeActiveModel(const QDateTime& changedDate, qint32 sampleRate, const QByteArray& hmmDefs,
				const QByteArray& tiedList, const QByteArray& dict, const QByteArray& dfa);

				
		QDateTime getModelSrcDate();
		QDateTime getCompileModelSrcDate();
		bool hasModelSrc();

		void setCurrentSrcContainerTime(const QDateTime& time);
		
		QDateTime getWordListDate(QString path=QString());
		bool hasWordList(const QString& modelPath=QString());
		WordListContainer* getWordList();
		bool storeWordList(const QDateTime& changedDate, const QByteArray& simpleVocab,
					const QByteArray& activeVocab, const QByteArray& activeLexicon);
		
		QDateTime getGrammarDate(QString path=QString());
		bool hasGrammar(const QString& modelPath=QString());
		GrammarContainer* getGrammar();
		bool storeGrammar(const QDateTime& changedDate, const QByteArray& grammarStructures);
		
		QDateTime getLanguageDescriptionDate(QString path=QString());
		bool hasLanguageDescription(const QString& modelPath=QString());
		LanguageDescriptionContainer* getLanguageDescription();
		bool storeLanguageDescription(const QDateTime& changedDate, const QByteArray& shadowVocab, 
				        const QByteArray& treeHed);
		
		QDateTime getTrainingDate(QString path=QString());
		bool hasTraining(const QString& modelPath=QString());
		TrainingContainer* getTraining();
		bool storeTraining(const QDateTime& changedDate, qint32 sampleRate, const QByteArray& wavConfig,
					const QByteArray& prompts);
		
		void buildMissingSamples();
		QByteArray getSample(const QString& sampleName);
		QString missingSample();
		bool storeSample(const QByteArray& sample);

		QString getLexiconPath();
		QString getGrammarPath();
		QString getVocabPath();
		QString getSimpleVocabPath();
		QString getPromptsPath();
		QString getTreeHedPath();
		QString getWavConfigPath();

		bool startSynchronisation();
		bool abort();
		bool commit();
		bool removeExcessModelBackups();
		
		void modelCompiled();
		bool copyWordList(const QString& source, const QString& dest);
		bool copyGrammar(const QString& source, const QString& dest);
		bool copyTrainingData(const QString& source, const QString& dest);
		bool copyLanguageDescription(const QString& source, const QString& dest);
		bool switchToModel(const QDateTime& modelDate);

		~SynchronisationManager();
		
};

#endif
