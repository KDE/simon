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


#include "mergeterminals.h"
#include <simonscenarios/scenariomanager.h>
#include "modelmanageruiproxy.h"
#include <KLocalizedString>

MergeTerminals::MergeTerminals(QObject* parent): QThread(parent)
{
}

void MergeTerminals::run()
{
	ModelManagerUiProxy::getInstance()->startGroup();

	emit status(i18n("Processing Words of Terminal %1", this->terminalA));
	emit progress(0,100);

	SpeechModel::ModelElements elements = SpeechModel::ScenarioVocabulary;
	if (includeGrammar)
		elements = (SpeechModel::ModelElements) (SpeechModel::ScenarioGrammar|elements);

	if (includeShadow)
		elements = (SpeechModel::ModelElements) (SpeechModel::ShadowVocabulary|elements);

	ScenarioManager::getInstance()->renameTerminal(terminalA, newName, elements);

	emit status(i18n("Processing Words of Terminal %1", this->terminalB));
	emit progress(50,100);

	ScenarioManager::getInstance()->renameTerminal(terminalB, newName, elements);

	ModelManagerUiProxy::getInstance()->commitGroup();

	emit status(i18n("Finished"));
	emit progress(100,100);
	emit done();
}


MergeTerminals::~MergeTerminals()
{
}


