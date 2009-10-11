/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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


#include "scenariomanager.h"

#include <speechmodelmanagement/scenario.h>
#include <speechmodelmanagement/shadowvocabulary.h>
#include <simonscenariobase/scenariodisplay.h>

#include <QFileInfo>
#include <KDebug>
#include <KGlobal>
#include <KStandardDirs>

ScenarioManager* ScenarioManager::instance;

ScenarioManager::ScenarioManager(QObject *parent) : QObject(parent), currentScenario(0)
{
}

bool ScenarioManager::init()
{
	bool succ = true;
	if (!setupScenarios())
		succ = false;

	shadowVocab = new ShadowVocabulary();
	connect(shadowVocab, SIGNAL(changed()), this, SLOT(shadowVocabularyChanged()));
	return succ;
}


ShadowVocabulary* ScenarioManager::getShadowVocabulary()
{
	return shadowVocab;
}

// If force is true, every registered display will switch to this scenario
// if not, only displays that already display the scenario will be updated
void ScenarioManager::updateDisplays(Scenario* scenario, bool force)
{
	currentScenario = scenario;
	foreach (ScenarioDisplay* display, scenarioDisplays) {
		if (force || (display->currentScenario() ==  scenario)) {
			display->displayScenario(scenario);
		}
	}
}

Scenario* ScenarioManager::getScenario(const QString& id)
{
	foreach (Scenario *scenario, scenarios) {
		if (scenario->id() == id)
			return scenario;
	}
	return NULL;
}

bool ScenarioManager::setupScenarios()
{
	bool success = true;

	kDebug() << "Setting up scenarios...";
	//Currently loads all scenarios
	//Should be replaced with "only selected ones" through the manage scenario action
	QStringList scenarioSrcs = KGlobal::dirs()->findAllResources("appdata", "scenarios/");
	QStringList scenarioIds;

	foreach (const QString& src, scenarioSrcs) {
		QFileInfo f(src);
		QString idToBe = f.fileName();
		if (!scenarioIds.contains(idToBe)) 
			scenarioIds << idToBe;
	}

	kDebug() << "Found scenarios: " << scenarioIds;

	foreach (const QString& id, scenarioIds) {
		Scenario *s = new Scenario(id);
		kDebug() << "Initializing scenario" << id;
		if (!s->init()) {
//			KMessageBox::error(0, i18n("Failed to initialize scenario \"%1\"").arg(id));
			success = false;
		} else {
			connect(s, SIGNAL(changed(Scenario*)), this, SLOT(updateDisplays(Scenario*)));
			connect(s, SIGNAL(changed(Scenario*)), this, SLOT(scenariosChanged()));
			scenarios << s;
		}
	}
	return success;
}

QStringList ScenarioManager::getTerminals(SpeechModel::ModelElements elements)
{
	/*ShadowVocabulary=1,
	ScenarioVocabulary=2,
	ScenarioGrammar=4,
	ScenarioActions=8*/
	QStringList terminals;

	if ((elements & SpeechModel::ScenarioGrammar) || (elements & SpeechModel::ScenarioVocabulary))
		terminals << getCurrentScenario()->getTerminals(elements);

	if (elements & SpeechModel::ShadowVocabulary) {
		QStringList shadowTerminals = shadowVocab->getTerminals();
		foreach (const QString& terminal, shadowTerminals)
			if (!terminals.contains(terminal))
				terminals << terminal;
	}

	terminals.sort();

	return terminals;
}

bool ScenarioManager::renameTerminal(const QString& terminal, const QString& newName, SpeechModel::ModelElements affect)
{
	bool success = true;

	if (affect & SpeechModel::ShadowVocabulary) {
		if (!(shadowVocab->renameTerminal(terminal, newName)))
			success=false;
	}

	if (!getCurrentScenario()->renameTerminal(terminal, newName, affect))
		success = false;

	return success;
}

QList<Word*> ScenarioManager::findWords(const QString& name, SpeechModel::ModelElements elements)
{
	QList<Word*> words;
	if (elements & SpeechModel::ShadowVocabulary) {
			kDebug() << "Finding words from shadow vocabulary ";
		QList<Word*> newWords = shadowVocab->findWords(name);
		words.append(newWords);
	}

	if (elements & SpeechModel::AllScenariosVocabulary) {
		foreach (Scenario* s, scenarios) {
			kDebug() << "Finding words for scenario " << s;
			QList<Word*> newWords = s->findWords(name);
			words.append(newWords);
		}
	} else 
		if (elements & SpeechModel::ScenarioVocabulary) {
			QList<Word*> newWords = getCurrentScenario()->findWords(name);
			words.append(newWords);
		}

	return words;
}

ScenarioManager::~ScenarioManager()
{
}

