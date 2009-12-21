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
	return succ;
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
	QStringList  scenarioIds = cg.readEntry("SelectedScenarios", QStringList());

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

bool ScenarioManager::setupScenario(Scenario *s)
{
	if (!s->init()) {
		kDebug() << "Couldn't init scenario";
		return false;
	}
	connect(s, SIGNAL(changed(Scenario*)), this, SLOT(updateDisplays(Scenario*)));
	connect(s, SIGNAL(changed(Scenario*)), this, SIGNAL(scenariosChanged()));
	return true;
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


bool ScenarioManager::triggerCommand(const QString& type, const QString& trigger)
{
	//TODO
	kDebug() << "Should execute command " << type << trigger;

	return false;
}

bool ScenarioManager::processResult(RecognitionResult recognitionResult)
{
	//TODO
	kDebug() << "Processing result " <<  recognitionResult.sentence().toUtf8().data();

	foreach (Scenario *s, scenarios) {
		if (s->processResult(recognitionResult))
			return true;
	}
	
	kDebug() << "Nobody accepted recognition result. Discarding.";
	return false;
}









/*
bool ScenarioManager::triggerCommand(const QString& type, const QString& trigger)
{
	if (type == "simonrecognitionresult" && currentlyPromptedListOfResults) {
		//result from a did-you-mean popup
		QString selectedSentence = trigger;
		selectedSentence.remove(QRegExp("^[0-9][0-9]?[0-9]?%: "));
		for (int i=0; i< currentlyPromptedListOfResults->count(); i++)
		{
			QString sentence = currentlyPromptedListOfResults->at(i).sentence();
			if (sentence == selectedSentence) {
				fprintf(stderr, "Found the result!\n");
				RecognitionResultList *list = new RecognitionResultList();
				list->append(RecognitionResult(currentlyPromptedListOfResults->at(i)));
				delete currentlyPromptedListOfResults;
				currentlyPromptedListOfResults = 0;
				processRawResults(list);
				return true;
			}
		}
		return true;
	}

	Command *com = getCommand(type, trigger);
	if (com)
		return com->trigger();
	else return false;
}

Command* ActionManager::getCommand(const QString& category, const QString& trigger)
{
	foreach (Action::Ptr action, actions)
	{
		if (action->manager()->name() == category)
		{
			CommandList *list = action->manager()->getCommands();
			foreach (Command* com, *list)
			{
				if (com->getTrigger() == trigger)
					return com;
			}
			break;
		}
	}
	return NULL;
}

void ScenarioManager::resultSelectionDone()
{
	fprintf(stderr, "resultSelectionDone()\n");
	delete currentlyPromptedListOfResults;
	currentlyPromptedListOfResults = 0;
}

void ScenarioManager::retrieveRecognitionResultFilteringParameters()
{
	useDYM = commandSettings->useDYM();
	minimumConfidenceThreshold = commandSettings->getMinimumConfidence();
}

void ScenarioManager::processRawResults(RecognitionResultList* recognitionResults)
{
	if (recognitionResults->isEmpty())
		return;

	RecognitionResultList *selectedRecognitionResults = new RecognitionResultList();

	if (!currentlyPromptedListOfResults || currentlyPromptedListOfResults->isEmpty()) {
		for (int i=0; i < recognitionResults->count(); i++) {
		//foreach (const RecognitionResult& result, recognitionResults) {
			//if the recognition result has:
			//	* One word that has a score of 0
			//	* An average score of below the minimum confidence
			//it will be not be included in the list of results
			
			QList<float> confidenceScores = recognitionResults->at(i).confidenceScores();

			//calc average
			float avg= recognitionResults->at(i).averageConfidenceScore();

			if (!confidenceScores.contains(0) && (avg > minimumConfidenceThreshold))
				selectedRecognitionResults->append(recognitionResults->at(i));
		}

		fprintf(stderr, "Viable recognition results: %d\n", selectedRecognitionResults->count());

		if (selectedRecognitionResults->count() == 0) return;
	} else {
		//we are already asking...
		selectedRecognitionResults->append(recognitionResults->at(0));
	}

	fprintf(stderr, "Greedy Recievers: %d\n", greedyReceivers->count());

	if (!greedyReceivers->isEmpty()) {
		for (int i=0; i < greedyReceivers->count(); i++) {
			if (greedyReceivers->at(i)->greedyTriggerRawList(selectedRecognitionResults))
				break;
		}
		delete selectedRecognitionResults;
		return;
	}

	if (selectedRecognitionResults->count() == 1) {
		processResult(selectedRecognitionResults->at(0));
	} else {
		presentUserWithResults(selectedRecognitionResults);
	}
	delete selectedRecognitionResults;
}

void ScenarioManager::presentUserWithResults(RecognitionResultList* recognitionResults)
{
	fprintf(stderr, "More than one possible recognition result ... should display list!\n");
	if (!useDYM || (currentlyPromptedListOfResults && 
				!currentlyPromptedListOfResults->isEmpty()))
	{
		//no double did-you-means...
		processResult(recognitionResults->at(0));
		return;
	}

	delete currentlyPromptedListOfResults;
	currentlyPromptedListOfResults = new RecognitionResultList();

	QStringList sentences;
	QStringList trigger;
	QStringList iconSrcs;
	for (int i=0; i<recognitionResults->count(); i++) {

		QList<float> confidenceScores = recognitionResults->at(i).confidenceScores();

		float avg = recognitionResults->at(i).averageConfidenceScore();
		avg *= 100;


		sentences << QString("%1%: %2").arg(qRound(avg)).arg(recognitionResults->at(i).sentence());
		iconSrcs << "";
		trigger << "simonrecognitionresult";
		currentlyPromptedListOfResults->append(recognitionResults->at(i));

	}

	ListCommand *list = new ListCommand(i18n("Did you mean ...?"), "help-hint", sentences, iconSrcs, trigger);
	connect(list, SIGNAL(canceled()), list, SLOT(deleteLater()));
	connect(list, SIGNAL(canceled()), this, SLOT(resultSelectionDone()));
	connect(list, SIGNAL(entrySelected()), list, SLOT(deleteLater()));
	list->trigger();
}



void ScenarioManager::processResult(RecognitionResult recognitionResult)
{
	fprintf(stderr, "processResult()\n");
	fprintf(stderr, "Verarbeite Ergebnis: %s\n", recognitionResult.sentence().toUtf8().data());
	int i=0;
	bool commandFound=false;
	QString currentTrigger;
	QString realCommand;
	fprintf(stderr, "recognitionResult: %s\n", recognitionResult.sentence().toUtf8().data());
	while ((i<actions.count()) && (!commandFound))
	{
		currentTrigger = actions[i]->trigger();
		fprintf(stderr, "CurrentTrigger: %s\n", currentTrigger.toUtf8().data());
		RecognitionResult tempResult = recognitionResult;
		if (tempResult.matchesTrigger(currentTrigger)) {
			tempResult.removeTrigger(currentTrigger);

			if(actions.at(i)->manager()->processResult(tempResult))
				commandFound=true;
		}
		i++;
	}
}

void ScenarioManager::registerGreedyReceiver(GreedyReceiver *receiver)
{
	greedyReceivers->insert(0, receiver);
}


void ScenarioManager::deRegisterGreedyReceiver(GreedyReceiver *receiver)
{
	greedyReceivers->removeAll(receiver);
}
*/



ScenarioManager::~ScenarioManager()
{
}

