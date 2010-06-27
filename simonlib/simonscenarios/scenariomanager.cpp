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
#include "speechmodelmanagementconfiguration.h"

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

ScenarioManager::ScenarioManager(QObject *parent) : QObject(parent),
	m_inGroup(false),
	m_baseModelDirty(false),
	m_scenariosDirty(false),
	m_shadowVocabularyDirty(false), 
	currentScenario(0)
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


void ScenarioManager::slotBaseModelChanged()
{
	if (m_inGroup)
		m_baseModelDirty = true;
	else
		emit baseModelChanged();
}

QStringList ScenarioManager::getAllAvailableScenarioIds(const QString& dataPrefix)
{
	QStringList scenarioSrcs = KGlobal::dirs()->findAllResources("data", dataPrefix+"scenarios/");
	QStringList scenarioIds;

	foreach (const QString& src, scenarioSrcs) {
		QFileInfo f(src);
		QString idToBe = f.fileName();
		if (!scenarioIds.contains(idToBe)) 
			scenarioIds << idToBe;
	}
	return scenarioIds;
}

QStringList ScenarioManager::getAllAvailableScenarioIds()
{
	return getAllAvailableScenarioIds("simon/");
}

bool ScenarioManager::storeScenario(const QString& id, const QByteArray& data)
{
	QString currentId = currentScenario->id();

	QFile f(KGlobal::dirs()->locateLocal("appdata", "scenarios/"+id));
	if (!f.open(QIODevice::WriteOnly)) return false;
	f.write(data);
	f.close();

	Scenario *newScenario = new Scenario(id);
	kDebug() << "Setting new scenario " << id;
	if (!setupScenario(newScenario))
		return false;


	for (int i=0; i < scenarios.count(); i++) {
		if (scenarios[i]->id() == id) {
			kDebug() << "Found scenario in the old list; replacing it with new version";
			Scenario *s = scenarios.takeAt(i);
			s->deleteLater();
			scenarios.insert(i, newScenario);
			break;
		}
	}

	//Forcing the display update if needed
	if (currentId == id)
	{
		kDebug() << "Updating scenario displays";
		updateDisplays(newScenario, true);
	}

	if (m_inGroup)
		m_scenariosDirty = true;
	else
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


void ScenarioManager::registerScenarioDisplay(ScenarioDisplay *display)
{
	scenarioDisplays.append(display);

	if (currentScenario)
		display->displayScenario(currentScenario);
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

	QStringList defaultScenarioIds;
	defaultScenarioIds << "general";

	QStringList scenarioIds;

	if (cg.hasKey("SelectedScenarios")) {
		scenarioIds = cg.readEntry("SelectedScenarios", defaultScenarioIds);
	} else {
		scenarioIds = defaultScenarioIds;
		cg.writeEntry("SelectedScenarios", defaultScenarioIds);
		cg.writeEntry("LastModified", QDateTime::currentDateTime());
		cg.sync();
	}

	kDebug() << "Loading scenario: " << scenarioIds;

	foreach (const QString& id, scenarioIds) {
		Scenario *s = new Scenario(id);
		kDebug() << "Initializing scenario" << id;
		
		if (setupScenario(s))
			scenarios << s;
		else {
			success = false;
			kDebug() << "Couldn't initialize scenario: " << id;
		}
	}

	if (forceChange) {
		if (m_inGroup)
			m_scenariosDirty = true;
		else
			emit scenariosChanged();
	}

	emit scenarioSelectionChanged();

	//we have to have at least one scenario loaded anyways; If not this
	//crash here is the least of our worries...
	kDebug() << "Updating displays here";
	updateDisplays(scenarios[0], true);

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
	bool success = true;

	if (affect & SpeechModel::ShadowVocabulary) {
		if (!(shadowVocab->renameTerminal(terminal, newName)))
			success=false;
	}

	if (!getCurrentScenario()->renameTerminal(terminal, newName, affect))
		success = false;

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

QList<Word*> ScenarioManager::findWordsByTerminal(const QString& name, SpeechModel::ModelElements elements)
{
	QList<Word*> words;
	if (elements & SpeechModel::ShadowVocabulary) {
		QList<Word*> newWords = shadowVocab->findWordsByTerminal(name);
		words.append(newWords);
	}

	if (elements & SpeechModel::AllScenariosVocabulary) {
		foreach (Scenario* s, scenarios) {
			QList<Word*> newWords = s->findWordsByTerminal(name);
			kDebug() << "Got " << newWords.count() << " words from " << s->id();
			words.append(newWords);
		}
	} else 
		if (elements & SpeechModel::ScenarioVocabulary) {
			QList<Word*> newWords = getCurrentScenario()->findWordsByTerminal(name);
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
	kDebug() << "Out sentences: " << outSentences;

	if (elements == SpeechModel::ScenarioGrammar) {
		outSentences.append(getCurrentScenario()->getExampleSentences(name, terminal, count));
	}
	kDebug() << "Out sentences: " << outSentences;


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
	m_inGroup = true;
}

bool ScenarioManager::commitGroup(bool silent)
{
	if (silent)
		blockSignals(true); //don't tell anyone

	m_inGroup = false;

	bool success = true;
	foreach (Scenario *s, scenarios) 
		success = s->commitGroup() && success;

	if (m_scenariosDirty) {
		m_scenariosDirty = false;
		emit scenariosChanged();
	}

	if (m_shadowVocabularyDirty) {
		m_shadowVocabularyDirty = false;
		emit shadowVocabularyChanged();
	}

	if (m_baseModelDirty) {
		m_baseModelDirty = false;
		emit baseModelChanged();
	}

	if (silent)
		blockSignals(false);
	return success;
}

void ScenarioManager::touchBaseModelAccessTime()
{
	KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("BaseModelDate", QDateTime::currentDateTime());
	config.sync();

	if (m_inGroup)
		m_baseModelDirty = true;
	else
		emit baseModelChanged();
}

int ScenarioManager::baseModelType()
{
	return SpeechModelManagementConfiguration::modelType();
}

QString ScenarioManager::baseModelHMMName()
{
	return SpeechModelManagementConfiguration::baseModelHMMName();
}

QString ScenarioManager::baseModelTiedlistName()
{
	return SpeechModelManagementConfiguration::baseModelTiedlistName();
}

QString ScenarioManager::baseModelMacros()
{
	return SpeechModelManagementConfiguration::baseModelMacrosName();
}

QString ScenarioManager::baseModelStats()
{
	return SpeechModelManagementConfiguration::baseModelStatsName();
}

void ScenarioManager::setBaseModelType(int type)
{
	SpeechModelManagementConfiguration::setModelType(type);
	SpeechModelManagementConfiguration::self()->writeConfig();
	touchBaseModelAccessTime();
}

void ScenarioManager::setBaseModel(int modelType, const QString& hmmName, const QString& tiedlistName, 
			const QString& macrosName, const QString& statsName)
{
	SpeechModelManagementConfiguration::setModelType(modelType);
	SpeechModelManagementConfiguration::setBaseModelHMMName(hmmName);
	SpeechModelManagementConfiguration::setBaseModelTiedlistName(tiedlistName);
	SpeechModelManagementConfiguration::setBaseModelMacrosName(macrosName);
	SpeechModelManagementConfiguration::setBaseModelStatsName(statsName);
	SpeechModelManagementConfiguration::self()->writeConfig();
	touchBaseModelAccessTime();
}

ScenarioManager::~ScenarioManager()
{
}

