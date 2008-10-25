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

#include "modelcontainer.h"

ModelContainer::ModelContainer(int sampleRate, int channels, QIODevice* wavConfig,
				QIODevice* grammarStructures, 
				QIODevice* simpleVocab, QIODevice* activeVocab,
				QIODevice* activeLexicon, QIODevice* shadowVocab, 
				QIODevice* shadowLexicon, QIODevice* treeHed,
				const QHash<QString,QString>& trainingsMap) : 
	m_sampleRate(sampleRate), m_channels(channels), m_wavConfig(wavConfig), 
	m_grammarStructures(grammarStructures),m_simpleVocab(simpleVocab),
	m_activeVocab(activeVocab), m_activeLexicon(activeLexicon), 
	m_shadowVocab(shadowVocab), m_shadowLexicon(shadowLexicon), 
	m_treeHed(treeHed), m_trainingsMap(trainingsMap)
{
	
}


ModelContainer::~ModelContainer()
{
	delete m_wavConfig;

	delete m_grammarStructures;
	
	delete m_simpleVocab;
	delete m_activeVocab;
	delete m_shadowVocab;
	delete m_activeLexicon;
	delete m_shadowLexicon;
	
	delete m_treeHed;
}
