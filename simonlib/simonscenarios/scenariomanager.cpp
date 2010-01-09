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
	connect(shadowVocab, SIGNAL(changed()), this, SIGNAL(shadowVocabularyChanged()));
	return succ && !shadowVocab->isNull();
}



QStringList ScenarioManager::getAllAvailableScenarioIds()
{
	QStringList scenarioSrcs = KGlobal::dirs()->findAllResources("appdata", "scenarios/");
	QStringList scenarioIds;

	foreach (const QString& src, scenarioSrcs) {
		QFileInfo f(src);
		QString idToBe = f.fileName();
		if (!scenarioIds.contains(idToBe)) 
			scenarioIds << idToBe;
	}
	return scenarioIds;
}

bool ScenarioManager::storeScenario(const QString& id, const QByteArray& data)
{
	QFile f(KGlobal::dirs()->locateLocal("appdata", "scenarios/"+id));
	if (!f.open(QIODevice::WriteOnly)) return false;
	f.write(data);
	f.close();

	Scenario *newScenario = new Scenario(id);
	if (!setupScenario(newScenario))
		return false;


	for (int i=0; i < scenarios.count(); i++) {
		if (scenarios[i]->id() == id) {
			Scenario *s = scenarios.takeAt(i);
			delete s;
			scenarios.insert(i, newScenario);
			break;
		}
	}

	updateDisplays(newScenario, true);
	emit scenariosChanged();

	return true;
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

bool ScenarioManager::setupScenarios(bool forceChange)
{
	bool success = true;

	qDeleteAll(scenarios);
	scenarios.clear();

	kDebug() << "Setting up scenarios...";

	KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
	KConfigGroup cg(config, "");
	QStringList  scenarioIds = cg.readEntry("SelectedScenarios", QStringList() << "general");
	kDebug() << "Loading scenario: " << scenarioIds;

	foreach (const QString& id, scenarioIds) {
		Scenario *s = new Scenario(id);
		kDebug() << "Initializing scenario" << id;
		
		if (setupScenario(s))
			scenarios << s;
		else success = false;
	}

	if (forceChange)
		emit scenariosChanged();

	return success;
}

void ScenarioManager::setPluginFont(const QFont& font)
{
	kDebug() << "Setting plugin font...";
	foreach (Scenario *s, scenarios)
		s->setPluginFont(font);
}

bool ScenarioManager::setupScenario(Scenario *s)
{
	if (!s->init()) {
		kDebug() << "Couldn't init scenario";
		return false;
	}
//	connect(s, SIGNAL(changed(Scenario*)), this, SLOT(updateDisplays(Scenario*)));
	connect(s, SIGNAL(changed(Scenario*)), this, SIGNAL(scenariosChanged()));
	return true;
}

QStringList ScenarioManager::getTerminals(SpeechModel::ModelElements elements)
{
	QStringList terminals;


	if ((elements & SpeechModel::ScenarioGrammar) || (elements & SpeechModel::ScenarioVocabulary))
		terminals << getCurrentScenario()->getTerminals(elements);

	SpeechModel::ModelElements foreignElements;
	if (elements & SpeechModel::AllScenariosGrammar)
		foreignElements = SpeechModel::ScenarioGrammar;
	if (elements & SpeechModel::AllScenariosVocabulary)
		foreignElements = (SpeechModel::ModelElements) (((int)foreignElements)|((int)SpeechModel::ScenarioVocabulary));

	foreach (Scenario *s, scenarios) {
		QStringList foreignTerminals = s->getTerminals(foreignElements);
		foreach (const QString& terminal, foreignTerminals)
			if (!terminals.contains(terminal))
				terminals << terminal;
	}

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


bool ScenarioManager::triggerCommand(const QString& type, const QString& trigger)
{
	kDebug() << "Should execute command " << type << trigger;
	foreach (Scenario *s, scenarios) {
		if (s->triggerCommand(type, trigger))
			return true;
	}

	return false;
}

bool ScenarioManager::processResult(RecognitionResult recognitionResult)
{
	kDebug() << "Processing result " <<  recognitionResult.sentence().toUtf8().data();

	foreach (Scenario *s, scenarios) {
		if (s->processResult(recognitionResult))
			return true;
	}
	
	kDebug() << "Nobody accepted recognition result. Discarding.";
	return false;
}

CommandList* ScenarioManager::getCommandList()
{
	CommandList* commandList = new CommandList();
	foreach (Scenario *s, scenarios)  {
		CommandList *list = s->getCommandList();
		if (list) {
			commandList->append(*list);
			delete list;
		}
	}
	
	return commandList;
}


void ScenarioManager::startGroup()
{
	foreach (Scenario *s, scenarios) 
		s->startGroup();
}

bool ScenarioManager::commitGroup()
{
	bool success = true;
	foreach (Scenario *s, scenarios) 
		success = s->commitGroup() && success;
	return success;
}

QList<CommandLauncher*> ScenarioManager::getLauncherList()
{
	return currentScenario->getLauncherList();
}


ScenarioManager::~ScenarioManager()
{
}

