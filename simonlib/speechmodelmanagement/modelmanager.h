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

#include "simonacousticmodelmanagement_export.h"
#include <QObject>
#include <QDateTime>

class Model;
class WordListContainer;
class GrammarContainer;
class LanguageDescriptionContainer;
class TrainingContainer;

class ACOUSTICMODELMANAGEMENT_EXPORT ModelManager : public QObject
{
	Q_OBJECT
	
	signals:
		void modelChanged();
	
	public:
		ModelManager();

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
		bool storeLanguageDescription(const QDateTime& changedTime, const QByteArray& shadowVocab, const QByteArray& shadowLexicon, 
				        const QByteArray& treeHed);
		
		TrainingContainer* getTrainingContainer();
		QDateTime getTrainingModifiedTime();
		bool storeTraining(const QDateTime& changedTime, int soundChannels, int sampleRate, const QByteArray& wavConfig,
					const QByteArray& prompts);

		Model* createActiveContainer();
		QDateTime getActiveContainerModifiedTime();
		bool storeActiveModel(const QDateTime& changedTime, int sampleRate, int channels, const QByteArray& hmmDefs,
				const QByteArray& tiedList, const QByteArray& dict, const QByteArray& term);

		~ModelManager() {}
		
};

#endif
