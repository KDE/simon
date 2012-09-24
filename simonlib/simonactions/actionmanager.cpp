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

#include "actionmanager.h"
#include "actionmanageradaptor.h"

#include "listcommand.h"
#include "commandsettingsinternal.h"

#include <simoninfo/simoninfo.h>
#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/commandmanager.h>
#include <simonscenarios/createcommandwidget.h>
#include <simonscenarios/actioncollection.h>
#include <simonscenarios/scenario.h>

#include <QFile>
#include <QMetaObject>
#include <QDBusConnection>
#include <QCoreApplication>

#include <KMessageBox>
#include <KLocalizedString>
#include <KService>
#include <KServiceTypeTrader>
#include <KStandardDirs>
#include <KDesktopFile>
#include <KDebug>
#include <KLocale>
#include <KXmlGuiWindow>
#include <KXMLGUIClient>
#include <KXMLGUIFactory>

ActionManager* ActionManager::instance;

ActionManager::ActionManager(QObject* parent) : QObject(parent),
minimumConfidenceThreshold(0.45),
useDYM(false)
{
  retrieveRecognitionResultFilteringParameters();

  new ActionManagerAdaptor(this);
  QDBusConnection dbus = QDBusConnection::sessionBus();
  dbus.registerObject("/ActionManager", this);
  dbus.registerService("org.simon-listens.ActionManager");
}

ActionManager* ActionManager::getInstance()
{
  if (!instance) {
    instance = new ActionManager();
    connect(qApp, SIGNAL(aboutToQuit()), instance, SLOT(deleteLater()));
  }
  return instance;
}


void ActionManager::registerGreedyReceiver(GreedyReceiver *receiver)
{
  greedyReceivers.insert(0, receiver);
}


void ActionManager::deRegisterGreedyReceiver(GreedyReceiver *receiver)
{
  greedyReceivers.removeAll(receiver);
}


void ActionManager::retrieveRecognitionResultFilteringParameters()
{
  useDYM = CommandSettingsInternal::getInstance()->useDYM();
  minimumConfidenceThreshold = CommandSettingsInternal::getInstance()->minimumConfidence();
  ScenarioManager::getInstance()->setListBaseConfiguration(getGlobalListInterfaceCommands());
}


bool ActionManager::triggerCommand(const QString& type, const QString& trigger, bool silent)
{
  if (type == "simonrecognitionresult" && currentlyPromptedListOfResults.count()) {
    //result from a did-you-mean popup
    QString selectedSentence = trigger;
    selectedSentence.remove(QRegExp("^[0-9][0-9]?[0-9]?%: "));
    foreach (const RecognitionResult &result, currentlyPromptedListOfResults) {
      QString sentence = result.sentence();
      if (sentence == selectedSentence) {
        kDebug() << "Found the result!";
        RecognitionResultList list;
        list.append(result);
        currentlyPromptedListOfResults.clear();
        processRawResults(list);
        return true;
      }
    }
    return true;
  }

  return ScenarioManager::getInstance()->triggerCommand(type, trigger, silent);
}


bool ActionManager::processResult(RecognitionResult recognitionResult)
{
  bool accepted = false;
  if (!greedyReceivers.isEmpty()) {
    if (greedyReceivers.at(0)->greedyTriggerRaw(recognitionResult))
      accepted = true;
  } else
    accepted = ScenarioManager::getInstance()->processResult(recognitionResult);
  
  emit processedRecognitionResult(recognitionResult, accepted);
  return accepted;
}


void ActionManager::processRawResults(const RecognitionResultList &recognitionResults)
{
  if (recognitionResults.isEmpty())
    return;

  kDebug() << "Processing " << recognitionResults.count() << " raw results";

  RecognitionResultList selectedRecognitionResults;

  if (currentlyPromptedListOfResults.isEmpty()) {
    for (int i=0; i < recognitionResults.count(); i++) {
      //foreach (const RecognitionResult& result, recognitionResults) {
      //if the recognition result has:
      //	* One word that has a score of 0
      //	* An average score of below the minimum confidence
      //it will be not be included in the list of results

      QList<float> confidenceScores = recognitionResults.at(i).confidenceScores();
      kDebug() << confidenceScores;

      //calc average
      float avg= recognitionResults.at(i).averageConfidenceScore();
      kDebug() << avg << minimumConfidenceThreshold;

      if (!confidenceScores.contains(0.0f) && (avg >= minimumConfidenceThreshold))
        selectedRecognitionResults.append(recognitionResults.at(i));
    }

    kDebug() << "Viable recognition results: " << selectedRecognitionResults.count();

    if (selectedRecognitionResults.isEmpty()) {
      return;
    }
  }
  else {
    //we are already asking...
    selectedRecognitionResults.append(recognitionResults.at(0));
  }

  kDebug() << "Greedy Recievers: " << greedyReceivers.count();

  if (!greedyReceivers.isEmpty()) {
    greedyReceivers.at(0)->greedyTriggerRawList(selectedRecognitionResults);
    selectedRecognitionResults.clear();
    return;
  }

  if (selectedRecognitionResults.count() == 1) {
    processResult(selectedRecognitionResults.at(0));
  } else {
    presentUserWithResults(selectedRecognitionResults);
  }
  selectedRecognitionResults.clear();
}


CommandList ActionManager::getCommandList()
{
  return ScenarioManager::getInstance()->getCommandList();
}


void ActionManager::resultSelectionDone()
{
  kDebug() << "resultSelectionDone()";
  currentlyPromptedListOfResults.clear();
}


void ActionManager::presentUserWithResults(const RecognitionResultList &recognitionResults)
{
  kDebug() << "More than one possible recognition result ... should display list!";
  if (!useDYM || !currentlyPromptedListOfResults.isEmpty()) {
    //no double did-you-means...
    processResult(recognitionResults.at(0));
    return;
  }
  currentlyPromptedListOfResults.clear();
  
  QStringList sentences;
  QStringList trigger;
  QStringList iconSrcs;
  for (int i=0; i<recognitionResults.count(); i++) {

    QList<float> confidenceScores = recognitionResults.at(i).confidenceScores();

    float avg = recognitionResults.at(i).averageConfidenceScore();
    avg *= 100;

    sentences << QString("%1%: %2").arg(qRound(avg)/*, 0, 'f', 0*/).arg(recognitionResults.at(i).sentence());
    iconSrcs << "";
    trigger << "simonrecognitionresult";
    currentlyPromptedListOfResults.append(recognitionResults.at(i));
  }

  ListCommand *list = new ListCommand(0 /* no manager */, i18n("Did you mean ...?"), "help-hint", i18n("simon is not sure what you meant.\n\nPlease select the correct result from the list below."), sentences, iconSrcs, trigger);
  connect(list, SIGNAL(canceled()), list, SLOT(deleteLater()));
  connect(list, SIGNAL(canceled()), this, SLOT(resultSelectionDone()));
  connect(list, SIGNAL(entrySelected()), list, SLOT(deleteLater()));
  int state = SimonCommand::DefaultState;
  list->trigger(&state, true /* silent */);
}


QFont ActionManager::pluginBaseFont()
{
  return CommandSettingsInternal::getInstance()->pluginBaseFont();
}


QHash<CommandListElements::Element, VoiceInterfaceCommand*> ActionManager::getListInterfaceCommands()
{
  return ScenarioManager::getInstance()->getCurrentScenario()->actionCollection()->getListInterfaceCommands();
}


QHash<CommandListElements::Element, VoiceInterfaceCommand*> ActionManager::getGlobalListInterfaceCommands()
{
  return CommandSettingsInternal::getInstance()->getListInterfaceCommands();
}


ActionManager::~ActionManager()
{
}
