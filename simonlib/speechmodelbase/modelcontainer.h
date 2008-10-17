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

#include <QString>
#include <QHash>
#include <QStringList>
#include "speechmodelbase_export.h"
#include "word.h"

class SPEECHMODELBASE_EXPORT ModelContainer
{
	private:
		int m_sampleRate, m_channels;
		QString m_wavConfig;
	
		QStringList m_grammarStructures;
		WordList *m_simpleVocab;
		QString m_treeHed;

		QHash<QString, QString> m_trainingsMap;

	public:
		ModelContainer(int sampleRate, int channels, const QString& wavConfig,
			       const QStringList& grammarStructures, WordList *simpleVocab,
			       const QString& treeHed,
			       const QHash<QString,QString>& trainingsMap);
			       
		~ModelContainer();
		
		int sampleRate() { return m_sampleRate; }
		int channels() { return m_channels; }
		QString wavConfig() { return m_wavConfig; }
		QStringList grammarStructures() { return m_grammarStructures; }
		WordList* simpleVocab() { return m_simpleVocab; }
		QString treeHed() { return m_treeHed; }
		QHash<QString,QString> trainingsMap() { return m_trainingsMap; }
		
};

#endif
