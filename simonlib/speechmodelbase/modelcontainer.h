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


#ifndef MODELCONTAINER_H
#define MODELCONTAINER_H

#include "speechmodelbase_export.h"

#include <QString>
#include <QHash>

class SPEECHMODELBASE_EXPORT ModelContainer
{
	private:
		int m_sampleRate, m_channels;
		QIODevice* m_wavConfig;
	
		QIODevice* m_grammarStructures;
		
		QIODevice* m_simpleVocab;
		QIODevice* m_activeVocab;
		QIODevice* m_activeLexicon;
		QIODevice* m_shadowVocab;
		QIODevice* m_shadowLexicon;
		
		QIODevice* m_treeHed;

		QHash<QString, QString> m_trainingsMap;

	public:
		ModelContainer(int sampleRate, int channels, QIODevice* wavConfig,
				QIODevice* grammarStructures, QIODevice *simpleVocab,
				QIODevice* activeVocab, QIODevice* activeLexicon, 
				QIODevice* shadowVocab, QIODevice* shadowLexicon, 
				QIODevice* treeHed,
				const QHash<QString,QString>& trainingsMap);
			       
		~ModelContainer();
		
		int sampleRate() { return m_sampleRate; }
		int channels() { return m_channels; }
		QIODevice* wavConfig() { return m_wavConfig; }
		QIODevice* grammarStructures() { return m_grammarStructures; }
		QIODevice* simpleVocab() { return m_simpleVocab; }
		QIODevice* activeVocab() { return m_activeVocab; }
		QIODevice* activeLexicon() { return m_activeLexicon; }
		QIODevice* shadowVocab() { return m_shadowVocab; }
		QIODevice* shadowLexicon() { return m_shadowLexicon; }
		QIODevice* treeHed() { return m_treeHed; }
		QHash<QString,QString> trainingsMap() { return m_trainingsMap; }
		
};

#endif
