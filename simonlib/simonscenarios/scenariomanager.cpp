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

#include <simonscenarios/scenario.h>
#include <simonscenarios/shadowvocabulary.h>
#include "scenariodisplay.h"

#include <QFileInfo>
#include <KDebug>
#include <KGlobal>
#include <KStandardDirs>
#include <KSharedConfig>
#include <KConfigGroup>

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

	qDeleteAll(scenarios);
	scenarios.clear();

	kDebug() << "Setting up scenarios...";

	KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
	KConfigGroup cg(config, "");
	QStringList  scenarioIds = cg.readEntry("SelectedScenarios", QStringList());

	foreach (const QString& id, scenarioIds) {
		Scenario *s = new Scenario(id);
		kDebug() << "Initializing scenario" << id;
		if (!s->init()) {
			success = false;
		} else {
			connect(s, SIGNAL(changed(Scenario*)), this, SLOT(updateDisplays(Scenario*)));
			connect(s, SIGNAL(changed(Scenario*)), this, SIGNAL(scenariosChanged()));
			scenarios << s;
		}
	}

	return success;
}

QStringList ScenarioManager::getTerminals(SpeechModel::ModelElements elements)
{
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
	kDebug() << "Start renaming terminal...";
	bool success = true;

	kDebug() << "Renaming in shadow...";
	if (affect & SpeechModel::ShadowVocabulary) {
		if (!(shadowVocab->renameTerminal(terminal, newName)))
			success=false;
	}

	kDebug() << "Renaming in current...";
	if (!getCurrentScenario()->renameTerminal(terminal, newName, affect))
		success = false;

	kDebug() << "Renaming done...";
	return success;
}

QList<Word*> ScenarioManager::findWords(const QString& name, SpeechModel::ModelElements elements, Vocabulary::MatchType type)
{
	QList<Word*> words;
	if (elements & SpeechModel::ShadowVocabulary) {
		QList<Word*> newWords = shadowVocab->findWords(name, type);
		words.append(newWords);
	}

	if (elements & SpeechModel::AllScenariosVocabulary) {
		foreach (Scenario* s, scenarios) {
			QList<Word*> newWords = s->findWords(name, type);
			words.append(newWords);
		}
	} else 
		if (elements & SpeechModel::ScenarioVocabulary) {
			QList<Word*> newWords = getCurrentScenario()->findWords(name, type);
			words.append(newWords);
		}

	return words;
}

QStringList ScenarioManager::getExampleSentences(const QString& name, const QString& terminal, int count, SpeechModel::ModelElements elements)
{
	QStringList outSentences;

	if (elements == SpeechModel::AllScenariosGrammar) {
		foreach (Scenario* s, scenarios) {
			outSentences.append(s->getExampleSentences(name, terminal, count));
		}
	} 

	if (elements == SpeechModel::ScenarioGrammar) {
		outSentences.append(getCurrentScenario()->getExampleSentences(name, terminal, count));
	}


	return outSentences;
}


ScenarioManager::~ScenarioManager()
{
}

