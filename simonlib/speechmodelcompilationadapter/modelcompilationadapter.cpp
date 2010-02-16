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

#include "modelcompilationadapter.h"

#include <KLocale>
#include <KDebug>
#include <KGlobal>
#include <QString>



ModelCompilationAdapter::ModelCompilationAdapter(const QString& userName, QObject *parent) : QThread(parent), m_userName(userName)
{
	KLocale::setMainCatalog("simonlib");
}

void ModelCompilationAdapter::run()
{
	emit  status(i18n("Adapting model..."), 0);
	if (!adaptModel(m_adaptionType, m_scenarioPathsIn, m_promptsPathIn, m_lexiconPathOut,
			m_grammarPathOut, m_simpleVocabPathOut, m_promptsPathOut)) {
	//	emit error(i18n("Failed to adapt model"));
		return;
	}
	emit  status(i18n("Model adaption complete"), 100);
	emit adaptionComplete();
}

int ModelCompilationAdapter::maxProgress()
{
	return 100;
}

bool ModelCompilationAdapter::startAdaption(ModelCompilationAdapter::AdaptionType adaptionType,
		const QString& lexiconPathOut, const QString& grammarPathOut, const QString& simpleVocabPathOut, 
		const QString& promptsPathOut, const QStringList& scenarioPathsIn, const QString& promptsIn)
{
	if (isRunning()) {
		keepGoing=false;
		//terminate();
		wait();
		emit adaptionAborted();
	}

	m_adaptionType = adaptionType;
	m_lexiconPathOut = lexiconPathOut;
	m_grammarPathOut = grammarPathOut;
	m_simpleVocabPathOut = simpleVocabPathOut;
	m_promptsPathOut = promptsPathOut;
	m_scenarioPathsIn = scenarioPathsIn;
	m_promptsPathIn = promptsIn;

	keepGoing=true;

	start();
	return true;
}


ModelCompilationAdapter::~ModelCompilationAdapter()
{
}

