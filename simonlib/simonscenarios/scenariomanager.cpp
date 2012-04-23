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
#include "scenariodisplay.h"
#include "voiceinterfacecommand.h"

#include <simonscenarios/scenario.h>
#include <simonscenarios/shadowvocabulary.h>
#include <simongraphemetophoneme/graphemetophoneme.h>

#include <QFileInfo>
#include <QCoreApplication>

#include <KDebug>
#include <KGlobal>
#include <KStandardDirs>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KDateTime>

ScenarioManager *ScenarioManager::instance;

ScenarioManager *ScenarioManager::getInstance()
{
  if (!instance) {
    instance = new ScenarioManager();
    connect(qApp, SIGNAL(aboutToQuit()), instance, SLOT(deleteLater()));
  }
  return instance;
}

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

  shadowVocab = new ShadowVocabulary(this);
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

QHash<QString,QString> ScenarioManager::transcribe(QStringList words)
{
  QHash<QString,QString> out;
  QStringList toTranscribe;
  foreach (const QString& word, words) {
    QList<Word*> similar = ScenarioManager::getInstance()->findWords(word,
        (SpeechModel::ModelElements) (SpeechModel::ShadowVocabulary|
        SpeechModel::AllScenariosVocabulary), Vocabulary::ExactMatch);
    if (!similar.isEmpty()) {
      out.insert(word.toUpper(), similar.first()->getPronunciation());
      continue;
    }
    toTranscribe << word;
  }
  if (!toTranscribe.isEmpty()) {
    //sequitur
    QString transcription;
    QHash<QString, TranscriptionResult> sequiturResults = GraphemeToPhoneme::transcribe(toTranscribe, KStandardDirs::locate("appdata", "model/languageProfile"));
    if (sequiturResults.isEmpty()) {
      kWarning() << "Sequitur transcription failed. Is sequitur installed and do you have a valid model?";
      return out;
    }
    
    for (QHash<QString,TranscriptionResult>::const_iterator i = sequiturResults.constBegin();
         i != sequiturResults.constEnd(); ++i) {
      if (i.value().getSuccess())
        out.insert(i.key(), i.value().getData());
      else
        kWarning() << i.key() << "could not be transcribed";
    }
  }
  kDebug() << out;
  return out;
}

QString ScenarioManager::transcribe(QString word)
{
  return transcribe(QStringList() << word).value(word.toUpper());
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

QStringList ScenarioManager::getAllDeactivatedScenarioIds()
{
    QStringList deactivatedScenarios;

    kDebug() << "Preparing a list of deactivated scenarios...";

    foreach (Scenario* scenario, scenarios)
    {
        if (!scenario->isActive())
        {
            deactivatedScenarios.push_back(scenario->id());
            kDebug() << scenario->id() + " is deactivated";
        }
        else
        {
            kDebug() << scenario->id() + " is activated";
        }
    }

    return deactivatedScenarios;
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
    if (scenarios.at(i)->id() == id) {
      kDebug() << "Found scenario in the old list; replacing it with new version";
      Scenario *s = scenarios.takeAt(i);
      s->deleteLater();
      scenarios.insert(i, newScenario);
      break;
    }
  }

  //Forcing the display update if needed
  if (currentId == id) {
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

void ScenarioManager::deRegisterScenarioDisplay(ScenarioDisplay* display)
{
  scenarioDisplays.removeAll(display);
}

Scenario* ScenarioManager::getScenario(const QString& id)
{
  foreach (Scenario *scenario, scenarios) {
    if (scenario->id() == id)
      return scenario;
  }
  return 0;
}


bool ScenarioManager::setupScenarios(bool forceChange)
{
    bool success = true;

    kDebug() << "Setting up scenarios...";

    QStringList defaultScenarioIds;
    defaultScenarioIds << "general";

    QStringList scenarioIds;

    qDeleteAll(scenarios);
    scenarios.clear();

    KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
    KConfigGroup cg(config, "");

    if (cg.hasKey("SelectedScenarios")) {
        scenarioIds = cg.readEntry("SelectedScenarios", defaultScenarioIds);
    } else {
        scenarioIds = defaultScenarioIds;
        cg.writeEntry("SelectedScenarios", defaultScenarioIds);
        cg.writeEntry("LastModified", KDateTime::currentUtcDateTime().dateTime());
        cg.sync();
    }

    kDebug() << "Loading scenario: " << scenarioIds;

    foreach (const QString& id, scenarioIds) {
        Scenario *s = new Scenario(id, QString(), this);
        kDebug() << "Initializing scenario" << id;

        if (setupScenario(s))
            scenarios << s;
        else {
            success = false;
            kDebug() << "Could not initialize scenario: " << id;
        }
    }

    setupAllChildScenarios();

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
    updateDisplays(scenarios.at(0), true);

    return success;
}

void ScenarioManager::setupAllChildScenarios()
{
    foreach(Scenario* loadedScenario, scenarios)
    {
        loadedScenario->setupChildScenarios();
    }
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
    kDebug() << "Could not init scenario";
    return false;
  }
  //	connect(s, SIGNAL(changed(Scenario*)), this, SLOT(updateDisplays(Scenario*)));
  connect(s, SIGNAL(changed(Scenario*)), this, SIGNAL(scenariosChanged()));
  connect(s, SIGNAL(activationChanged()), this, SLOT(scenarioActivationChanged()));
  return true;
}

void ScenarioManager::scenarioActivationChanged()
{
    kDebug() << "ScenarioManager is preparing the list of deactivated scenarios!";

    emit deactivatedScenarioListChanged();
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


bool ScenarioManager::triggerCommand(const QString& type, const QString& trigger, bool silent)
{
  kDebug() << "Should execute command " << type << trigger;
  foreach (Scenario *s, scenarios) {
    if (s->triggerCommand(type, trigger, silent))
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


CommandList ScenarioManager::getCommandList()
{
  CommandList commandList;
  foreach (Scenario *s, scenarios) {
      commandList.append(s->getCommandList());
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
    blockSignals(true);                           //do not tell anyone

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
  cGroup.writeEntry("BaseModelDate", KDateTime::currentUtcDateTime().dateTime());
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


QString ScenarioManager::baseModelName()
{
  return SpeechModelManagementConfiguration::baseModelName();
}

QDateTime ScenarioManager::baseModelCreationDate()
{
  return SpeechModelManagementConfiguration::baseModelDate();
}

void ScenarioManager::setBaseModelType(int type)
{
  SpeechModelManagementConfiguration::setModelType(type);
  SpeechModelManagementConfiguration::self()->writeConfig();
  touchBaseModelAccessTime();
}


void ScenarioManager::setBaseModel(const QString& name, const QDateTime& creationDate)
{
  SpeechModelManagementConfiguration::setBaseModelName(name);
  SpeechModelManagementConfiguration::setBaseModelDate(creationDate);
}

QString ScenarioManager::languageProfileName()
{
  return SpeechModelManagementConfiguration::languageProfileName();
}
void ScenarioManager::setLanguageProfileName(const QString& name)
{
  SpeechModelManagementConfiguration::setLanguageProfileName(name);
  SpeechModelManagementConfiguration::self()->writeConfig();
}


void ScenarioManager::setListBaseConfiguration(QHash<CommandListElements::Element, VoiceInterfaceCommand*> listInterfaceCommands)
{
  kDebug() << "Setting list interface commands";
  this->listInterfaceCommands = listInterfaceCommands;
}

QHash<CommandListElements::Element, VoiceInterfaceCommand*> ScenarioManager::getListBaseConfiguration()
{
  return listInterfaceCommands;
}


ScenarioManager::~ScenarioManager()
{
  foreach (Scenario *s, scenarios)
    s->blockSignals(true);
  blockSignals(true);

  qDeleteAll(listInterfaceCommands);
}
