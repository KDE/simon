/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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
#include "version.h"
#include "scenariodisplay.h"
#include "voiceinterfacecommand.h"
#include "scenariomanageradaptor.h"
#include "scenarioofferui.h"

#include <simonscenarios/scenario.h>
#include <simonscenarios/shadowvocabulary.h>
#include "author.h"
#include <simongraphemetophoneme/graphemetophoneme.h>
#include <versionnumber.h>

#include <QFileInfo>
#include <QCoreApplication>
#include <QDBusConnection>

#include <QDebug>

#include <KSharedConfig>
#include <KDateTime>
#include <QStandardPaths>
#include <KConfigCore/KConfigGroup>

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
  currentScenario(0),
  m_scenarioOfferUi(0)
{
  new ScenarioManagerAdaptor(this);
  QDBusConnection dbus = QDBusConnection::sessionBus();
  dbus.registerObject("/ScenarioManager", this);
  dbus.registerService("org.simon-listens.ScenarioManager");
}

bool ScenarioManager::init()
{
  shadowVocab = new ShadowVocabulary(this);
  connect(shadowVocab, SIGNAL(changed()), this, SIGNAL(shadowVocabularyChanged()));

  bool succ = true;
  if (!setupScenarios())
    succ = false;

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
    QHash<QString, TranscriptionResult> sequiturResults = GraphemeToPhoneme::transcribe(toTranscribe, QStandardPaths::locate(QStandardPaths::DataLocation, "model/languageProfile"));
    if (sequiturResults.isEmpty()) {
      qWarning() << "Sequitur transcription failed. Is sequitur installed and do you have a valid model?";
      return out;
    }

    for (QHash<QString,TranscriptionResult>::const_iterator i = sequiturResults.constBegin();
         i != sequiturResults.constEnd(); ++i) {
      if (i.value().getSuccess())
        out.insert(i.key(), i.value().getData());
      else
        qWarning() << i.key() << "could not be transcribed";
    }
  }
  qDebug() << out;
  return out;
}

QString ScenarioManager::transcribe(QString word)
{
  return transcribe(QStringList() << word).value(word.toUpper());
}

QStringList ScenarioManager::getAllAvailableScenarioIds(const QString& dataPrefix)
{
  QStringList scenarioSrcs = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, dataPrefix+"scenarios", QStandardPaths::LocateDirectory);
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

  qDebug() << "Preparing a list of deactivated scenarios...";

  foreach (Scenario* scenario, scenarios)
  {
    if (!scenario->isActive())
    {
      deactivatedScenarios.push_back(scenario->id());
      qDebug() << scenario->id() + " is deactivated";
    }
    else
    {
      qDebug() << scenario->id() + " is activated";
    }
  }

  return deactivatedScenarios;
}


bool ScenarioManager::storeScenario(const QString& id, const QByteArray& data)
{
  QString currentId = currentScenario->id();

  QFile f(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "scenarios/"+id);
  if (!f.open(QIODevice::WriteOnly)) return false;
  f.write(data);
  f.close();

  for (int i=0; i < scenarios.count(); i++) {
    if (scenarios.at(i)->id() == id) {
      qDebug() << "Found scenario in the old list; replacing it with new version";

      Scenario *newScenario = new Scenario(id, QString(), this);
      qDebug() << "Setting new scenario" << id;
      if (!setupScenario(newScenario))
        return false;

      Scenario *s = scenarios.at(i);
      scenarios.insert(i, newScenario);
      scenarios.removeAt(i+1);
      if (s == currentScenario) {
        updateDisplays(newScenario, true);
        emit scenarioSelectionChanged();
      }
      qDebug() << "Deleted scenario:" << s;
      s->deleteLater();
      break;
    }
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


  QStringList scenarioIds;

  qDeleteAll(scenarios);
  scenarios.clear();

  KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
  KConfigGroup cg(config, "");

  if (cg.hasKey("SelectedScenarios")) {
    scenarioIds = cg.readEntry("SelectedScenarios", QStringList());
  } else {
    QString generalScenarioId = Scenario::createId(QLatin1String("general"));
    QStringList defaultScenarioIds;
    defaultScenarioIds << generalScenarioId;
    scenarioIds = defaultScenarioIds;
    cg.writeEntry("SelectedScenarios", defaultScenarioIds);

    //this is not supposed to be the current time but the date of the stock
    //configuration.
    //We set it to the beginning of the epoch to ensure, that any user configuration
    //that might get synced from the server is preferred to this one.
    QDateTime stockTime;
    stockTime.setTime_t(0);
    cg.writeEntry("LastModified", stockTime);
    cg.sync();

    // first run or corrupted configuration; Start off with new standard scenario
    Scenario *s = new Scenario(generalScenarioId);
    VersionNumber *minVersion = new VersionNumber(s, simon_version);
    QList<Author*> authors;
    authors << new Author(s, i18nc("Standard author of a scenario", "Anonymous"), i18nc("Standard \"email address\" of a scenario", "No mail provided"));

    if (!s->create(i18nc("Default name of an (empty) standard scenario", "Standard"), "simon", 1, minVersion, 0 /*maxVersion*/, "BSD", authors) 
        || !s->save(QString(), false, false)) {
      qWarning() << "Standard scenario could not be created";
      s->deleteLater();
      return false;
    }
    s->deleteLater();
  }

  qDebug() << "Loading scenarios:" << scenarioIds;

  foreach (const QString& id, scenarioIds) {
    Scenario *s = new Scenario(id, QString(), this);
    qDebug() << "Initializing scenario" << id;

    if (setupScenario(s))
      scenarios << s;
    else {
      success = false;
      qWarning() << "Could not initialize scenario:" << id;
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
  qDebug() << "Updating displays here";
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
  qDebug() << "Setting plugin font...";
  foreach (Scenario *s, scenarios)
    s->setPluginFont(font);
}


bool ScenarioManager::setupScenario(Scenario *s)
{
  if (!s->init()) {
    qDebug() << "Could not init scenario";
    return false;
  }
  //	connect(s, SIGNAL(changed(Scenario*)), this, SLOT(updateDisplays(Scenario*)));
  connect(s, SIGNAL(changed(Scenario*)), this, SIGNAL(scenariosChanged()));
  connect(s, SIGNAL(activationChanged()), this, SLOT(scenarioActivationChanged()));
  return true;
}

void ScenarioManager::scenarioActivationChanged()
{
  qDebug() << "ScenarioManager is preparing the list of deactivated scenarios!";

  emit deactivatedScenarioListChanged();
}


QStringList ScenarioManager::getCategories(SpeechModel::ModelElements elements)
{
  QStringList categories;

  if ((elements & SpeechModel::ScenarioGrammar) || (elements & SpeechModel::ScenarioVocabulary))
    categories << getCurrentScenario()->getCategories(elements);

  SpeechModel::ModelElements foreignElements;
  if (elements & SpeechModel::AllScenariosGrammar)
    foreignElements = SpeechModel::ScenarioGrammar;
  if (elements & SpeechModel::AllScenariosVocabulary)
    foreignElements = (SpeechModel::ModelElements) (((int)foreignElements)|((int)SpeechModel::ScenarioVocabulary));

  foreach (Scenario *s, scenarios) {
    QStringList foreignCategories = s->getCategories(foreignElements);
    foreach (const QString& category, foreignCategories)
      if (!categories.contains(category))
      categories << category;
  }

  if (elements & SpeechModel::ShadowVocabulary) {
    QStringList shadowCategories = shadowVocab->getCategories();
    foreach (const QString& category, shadowCategories)
      if (!categories.contains(category))
      categories << category;
  }

  categories.sort();

  return categories;
}


bool ScenarioManager::renameCategory(const QString& category, const QString& newName, SpeechModel::ModelElements affect)
{
  bool success = true;

  if (affect & SpeechModel::ShadowVocabulary) {
    if (!(shadowVocab->renameCategory(category, newName)))
      success=false;
  }

  if (!getCurrentScenario()->renameCategory(category, newName, affect))
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


QList<Word*> ScenarioManager::findWordsByCategory(const QString& name, SpeechModel::ModelElements elements)
{
  QList<Word*> words;
  if (elements & SpeechModel::ShadowVocabulary) {
    QList<Word*> newWords = shadowVocab->findWordsByCategory(name);
    words.append(newWords);
  }

  if (elements & SpeechModel::AllScenariosVocabulary) {
    foreach (Scenario* s, scenarios) {
      QList<Word*> newWords = s->findWordsByCategory(name);
      qDebug() << "Got " << newWords.count() << " words from " << s->id();
      words.append(newWords);
    }
  } else
  if (elements & SpeechModel::ScenarioVocabulary) {
    QList<Word*> newWords = getCurrentScenario()->findWordsByCategory(name);
    words.append(newWords);
  }

  return words;
}


QStringList ScenarioManager::getExampleSentences(const QString& name, const QString& category, int count, SpeechModel::ModelElements elements)
{
  QStringList outSentences;

  if (elements == SpeechModel::AllScenariosGrammar) {
    foreach (Scenario* s, scenarios) {
      outSentences.append(s->getExampleSentences(name, category, count));
    }
  }
  qDebug() << "Out sentences: " << outSentences;

  if (elements == SpeechModel::ScenarioGrammar) {
    outSentences.append(getCurrentScenario()->getExampleSentences(name, category, count));
  }
  qDebug() << "Out sentences: " << outSentences;

  return outSentences;
}


bool ScenarioManager::triggerCommand(const QString& type, const QString& trigger, bool silent)
{
  qDebug() << "Should execute command " << type << trigger;
  foreach (Scenario *s, scenarios) {
    if (!s->isActive())
      continue;
    if (s->triggerCommand(type, trigger, silent))
      return true;
  }

  return false;
}


bool ScenarioManager::processResult(RecognitionResult recognitionResult)
{
  qDebug() << "Processing result " <<  recognitionResult.sentence().toUtf8().data();

  foreach (Scenario *s, scenarios) {
    if (!s->isActive())
      continue;
    if (s->processResult(recognitionResult))
      return true;
  }

  qDebug() << "Nobody accepted recognition result. Discarding.";
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
    KConfig config( QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "model/modelsrcrc",
                    KConfig::SimpleConfig);
  KConfigGroup cGroup(&config, "");
  cGroup.writeEntry("BaseModelDate", KDateTime::currentUtcDateTime().dateTime());
  config.sync();

  if (m_inGroup)
    m_baseModelDirty = true;
  else
    emit baseModelChanged();
}

void ScenarioManager::setListBaseConfiguration(QHash<CommandListElements::Element, VoiceInterfaceCommand*> listInterfaceCommands)
{
  this->listInterfaceCommands = listInterfaceCommands;
}

QHash<CommandListElements::Element, VoiceInterfaceCommand*> ScenarioManager::getListBaseConfiguration()
{
  return listInterfaceCommands;
}

void ScenarioManager::installScenarioOfferUi(const ScenarioOfferUi* offerUi)
{
  m_scenarioOfferUi = offerUi;
}

ScenarioManager::ScenarioOfferReply ScenarioManager::installScenario(const QString& requester, const QString& path)
{
  Scenario* scenario = new Scenario(QLatin1String("scenarioOffer"));
  QString scenarioId = Scenario::idFromPath(path);

  // copy to temporary location
  QString tempPath = QDir::tempPath() + QLatin1Char('/') +  QLatin1String("simon/") + scenarioId;
  if (QFile::exists(tempPath)) QFile::remove(tempPath);
  QFile::copy(path, tempPath);

  if (!scenario->skim(tempPath)) {
    qDebug() << "Failed to skim from " << tempPath;
    scenario->deleteLater();
    return ScenarioManager::Incompatible;
  }

  //open scenario configuration
  KSharedConfigPtr scenariosConfig = KSharedConfig::openConfig("simonscenariosrc");
  KConfigGroup scenariosConfigGroup(scenariosConfig, "");
  //string list of imported scenarios of the form:
  // <id of the scenario to import>/<id of the (first) imported scenario>


  //check if we already have the scenario and if so, check if the version of this one is newer
  QStringList importedScenarios = scenariosConfigGroup.readEntry("AcceptedScenarioOffers", QStringList());
  foreach (const QString& importedScenario, importedScenarios) {
    if (importedScenario.startsWith(scenarioId+"/")) {
      QString oldScenarioId = importedScenario.mid(importedScenario.indexOf('/') + 1);
      //is this scenario still installed?
      if (getAllAvailableScenarioIds().contains(oldScenarioId)) {
        Scenario s(oldScenarioId);
        s.skim();
        if (s.version() >= scenario->version()) {
          qWarning() << "Rejecting because not newer as already installed version.";
          return ScenarioManager::Rejected;
        }
      }
    }
  }

  if (m_scenarioOfferUi && !m_scenarioOfferUi->askToAcceptScenario(requester, scenario->name(), scenario->authors())) {
    scenario->deleteLater();
    return ScenarioManager::Rejected;
  }
  scenario->deleteLater();

  QStringList explodedScenarios = Scenario::explode(tempPath);
  if (explodedScenarios.isEmpty())
    return ScenarioManager::Incompatible;
  else
    importedScenarios << scenarioId+'/'+explodedScenarios[0];
  for (int i = 0; i < explodedScenarios.count(); ++i) {
    scenario = new Scenario(explodedScenarios[i]);
    if (!scenario->init() || !scenario->save()) {
      qWarning() << "Failed to initialize (sub-) scenario: " << explodedScenarios[i];
      scenario->deleteLater();

      //delete scenarios extracted so far
      for (int j=i-1; j >= 0; --j)
        if (!QFile::remove(Scenario::pathFromId(explodedScenarios[j])))
          qWarning() << "Couldn't rollback scenario extraction: " << explodedScenarios[j];
      return ScenarioManager::Incompatible;
    }
    scenario->deleteLater();
  }
  // save scenarios
  scenariosConfigGroup.writeEntry("SelectedScenarios",
                                  scenariosConfigGroup.readEntry("SelectedScenarios", QStringList())
                                          << explodedScenarios);
  scenariosConfigGroup.writeEntry("LastModified", KDateTime::currentUtcDateTime().dateTime());
  scenariosConfigGroup.writeEntry("AcceptedScenarioOffers", importedScenarios);
  scenariosConfigGroup.sync();

  if (!setupScenarios(true))
    qWarning() << "Failed to reload scenarios";

  return ScenarioManager::Accepted;
}

ScenarioManager::~ScenarioManager()
{
  QList<Scenario*> oldScenarios  = scenarios;
  scenarios.clear();

  blockSignals(true);
  foreach (Scenario *s, oldScenarios) {
    s->blockSignals(true);
    s->deleteLater();
  }

  qDeleteAll(listInterfaceCommands);
}
