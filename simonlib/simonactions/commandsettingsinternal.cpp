/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "commandsettingsinternal.h"
#include "actionmanager.h"
#include "listconfiguration.h"

#include <simonscenarios/commandmanager.h>
#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/voiceinterfacecommand.h>

#include <KConfigGroup>

CommandSettingsInternal* CommandSettingsInternal::instance;

CommandSettingsInternal::CommandSettingsInternal(QWidget *parent)
  : QObject(parent),
    listConfiguration(new ListConfiguration(parent))
{
  if (instance) instance->deleteLater();
  instance = this;
  config = KSharedConfig::openConfig(KGlobal::mainComponent(), "simoncommandrc");
  load();
}


void CommandSettingsInternal::save(bool dym, float confidence, const QFont& font)
{
  KConfigGroup cg(config, QString());
  cg.writeEntry("MinimumConfidence", confidence);
  cg.writeEntry("UseDYM", dym);

  storedConfidence = confidence;
  storedDYM = dym;

  if (storedFont != font) {
    storedFont = font;
    cg.writeEntry("PluginBaseFont", storedFont);
    ScenarioManager::getInstance()->setPluginFont(storedFont);
  }

  listConfiguration->prepareToSave();

  cg.writeEntry("ListBackTriggers", listConfiguration->getListTriggers(CommandListElements::Back));
  cg.writeEntry("ListOneTriggers", listConfiguration->getListTriggers(CommandListElements::One));
  cg.writeEntry("ListTwoTriggers", listConfiguration->getListTriggers(CommandListElements::Two));
  cg.writeEntry("ListThreeTriggers", listConfiguration->getListTriggers(CommandListElements::Three));
  cg.writeEntry("ListFourTriggers", listConfiguration->getListTriggers(CommandListElements::Four));
  cg.writeEntry("ListFiveTriggers", listConfiguration->getListTriggers(CommandListElements::Five));
  cg.writeEntry("ListSixTriggers", listConfiguration->getListTriggers(CommandListElements::Six));
  cg.writeEntry("ListSevenTriggers", listConfiguration->getListTriggers(CommandListElements::Seven));
  cg.writeEntry("ListEightTriggers", listConfiguration->getListTriggers(CommandListElements::Eight));
  cg.writeEntry("ListNextTriggers", listConfiguration->getListTriggers(CommandListElements::Next));
  cg.writeEntry("ListCancelTriggers", listConfiguration->getListTriggers(CommandListElements::Cancel));

  cg.writeEntry("ListShowBackIcon", listConfiguration->getListShowIcon(CommandListElements::Back));
  cg.writeEntry("ListShowOneIcon", listConfiguration->getListShowIcon(CommandListElements::One));
  cg.writeEntry("ListShowTwoIcon", listConfiguration->getListShowIcon(CommandListElements::Two));
  cg.writeEntry("ListShowThreeIcon", listConfiguration->getListShowIcon(CommandListElements::Three));
  cg.writeEntry("ListShowFourIcon", listConfiguration->getListShowIcon(CommandListElements::Four));
  cg.writeEntry("ListShowFiveIcon", listConfiguration->getListShowIcon(CommandListElements::Five));
  cg.writeEntry("ListShowSixIcon", listConfiguration->getListShowIcon(CommandListElements::Six));
  cg.writeEntry("ListShowSevenIcon", listConfiguration->getListShowIcon(CommandListElements::Seven));
  cg.writeEntry("ListShowEightIcon", listConfiguration->getListShowIcon(CommandListElements::Eight));
  cg.writeEntry("ListShowNextIcon", listConfiguration->getListShowIcon(CommandListElements::Next));
  cg.writeEntry("ListShowCancelIcon", listConfiguration->getListShowIcon(CommandListElements::Cancel));

  cg.writeEntry("ListBackIcon", listConfiguration->getListIcon(CommandListElements::Back));
  cg.writeEntry("ListIconOne", listConfiguration->getListIcon(CommandListElements::One));
  cg.writeEntry("ListIconTwo", listConfiguration->getListIcon(CommandListElements::Two));
  cg.writeEntry("ListIconThree", listConfiguration->getListIcon(CommandListElements::Three));
  cg.writeEntry("ListIconFour", listConfiguration->getListIcon(CommandListElements::Four));
  cg.writeEntry("ListIconFive", listConfiguration->getListIcon(CommandListElements::Five));
  cg.writeEntry("ListIconSix", listConfiguration->getListIcon(CommandListElements::Six));
  cg.writeEntry("ListIconSeven", listConfiguration->getListIcon(CommandListElements::Seven));
  cg.writeEntry("ListIconEight", listConfiguration->getListIcon(CommandListElements::Eight));
  cg.writeEntry("ListIconNext", listConfiguration->getListIcon(CommandListElements::Next));
  cg.writeEntry("ListCancelIcon", listConfiguration->getListIcon(CommandListElements::Cancel));

  cg.writeEntry("ListBackVisibleTrigger", listConfiguration->getListVisibleTrigger(CommandListElements::Back));
  cg.writeEntry("ListOneVisibleTrigger", listConfiguration->getListVisibleTrigger(CommandListElements::One));
  cg.writeEntry("ListTwoVisibleTrigger", listConfiguration->getListVisibleTrigger(CommandListElements::Two));
  cg.writeEntry("ListThreeVisibleTrigger", listConfiguration->getListVisibleTrigger(CommandListElements::Three));
  cg.writeEntry("ListFourVisibleTrigger", listConfiguration->getListVisibleTrigger(CommandListElements::Four));
  cg.writeEntry("ListFiveVisibleTrigger", listConfiguration->getListVisibleTrigger(CommandListElements::Five));
  cg.writeEntry("ListSixVisibleTrigger", listConfiguration->getListVisibleTrigger(CommandListElements::Six));
  cg.writeEntry("ListSevenVisibleTrigger", listConfiguration->getListVisibleTrigger(CommandListElements::Seven));
  cg.writeEntry("ListEightVisibleTrigger", listConfiguration->getListVisibleTrigger(CommandListElements::Eight));
  cg.writeEntry("ListNextVisibleTrigger", listConfiguration->getListVisibleTrigger(CommandListElements::Next));
  cg.writeEntry("ListCancelVisibleTrigger", listConfiguration->getListVisibleTrigger(CommandListElements::Cancel));

  cg.sync();

  ActionManager::getInstance()->retrieveRecognitionResultFilteringParameters();
}

void CommandSettingsInternal::load()
{
  Q_ASSERT(config);

  KConfigGroup cg(config, QString());

  storedConfidence = cg.readEntry("MinimumConfidence", 0.45f);

  storedDYM = cg.readEntry("UseDYM", false);
  storedFont = cg.readEntry("PluginBaseFont", QFont());

  QStringList backTriggers, oneTriggers, twoTriggers, threeTriggers, fourTriggers,
    fiveTriggers, sixTriggers, sevenTriggers, eightTriggers, nextTriggers,
    cancelTriggers;

  backTriggers = cg.readEntry("ListBackTriggers", QStringList() << listConfiguration->getListDefaultTrigger(CommandListElements::Back));
  oneTriggers = cg.readEntry("ListOneTriggers", QStringList() << listConfiguration->getListDefaultTrigger(CommandListElements::One));
  twoTriggers = cg.readEntry("ListTwoTriggers", QStringList() << listConfiguration->getListDefaultTrigger(CommandListElements::Two));
  threeTriggers = cg.readEntry("ListThreeTriggers", QStringList() << listConfiguration->getListDefaultTrigger(CommandListElements::Three));
  fourTriggers = cg.readEntry("ListFourTriggers", QStringList() << listConfiguration->getListDefaultTrigger(CommandListElements::Four));
  fiveTriggers = cg.readEntry("ListFiveTriggers", QStringList() << listConfiguration->getListDefaultTrigger(CommandListElements::Five));
  sixTriggers = cg.readEntry("ListSixTriggers", QStringList() << listConfiguration->getListDefaultTrigger(CommandListElements::Six));
  sevenTriggers = cg.readEntry("ListSevenTriggers", QStringList() << listConfiguration->getListDefaultTrigger(CommandListElements::Seven));
  eightTriggers = cg.readEntry("ListEightTriggers", QStringList() << listConfiguration->getListDefaultTrigger(CommandListElements::Eight));
  nextTriggers = cg.readEntry("ListNextTriggers", QStringList() << listConfiguration->getListDefaultTrigger(CommandListElements::Next));
  cancelTriggers = cg.readEntry("ListCancelTriggers", QStringList() << listConfiguration->getListDefaultTrigger(CommandListElements::Cancel));

  bool showBackIcon, showOneIcon, showTwoIcon, showThreeIcon, showFourIcon,
    showFiveIcon, showSixIcon, showSevenIcon, showEightIcon,
    showNextIcon, showCancelIcon;

  showBackIcon = cg.readEntry("ListShowBackIcon", listConfiguration->getListDefaultShowIcon(CommandListElements::Back));
  showOneIcon = cg.readEntry("ListShowOneIcon", listConfiguration->getListDefaultShowIcon(CommandListElements::One));
  showTwoIcon = cg.readEntry("ListShowTwoIcon", listConfiguration->getListDefaultShowIcon(CommandListElements::Two));
  showThreeIcon = cg.readEntry("ListShowThreeIcon", listConfiguration->getListDefaultShowIcon(CommandListElements::Three));
  showFourIcon = cg.readEntry("ListShowFourIcon", listConfiguration->getListDefaultShowIcon(CommandListElements::Four));
  showFiveIcon = cg.readEntry("ListShowFiveIcon", listConfiguration->getListDefaultShowIcon(CommandListElements::Five));
  showSixIcon = cg.readEntry("ListShowSixIcon", listConfiguration->getListDefaultShowIcon(CommandListElements::Six));
  showSevenIcon = cg.readEntry("ListShowSevenIcon", listConfiguration->getListDefaultShowIcon(CommandListElements::Seven));
  showEightIcon = cg.readEntry("ListShowEightIcon", listConfiguration->getListDefaultShowIcon(CommandListElements::Eight));
  showNextIcon = cg.readEntry("ListShowNextIcon", listConfiguration->getListDefaultShowIcon(CommandListElements::Next));
  showCancelIcon = cg.readEntry("ListShowCancelIcon", listConfiguration->getListDefaultShowIcon(CommandListElements::Cancel));

  QString backIcon, oneIcon, twoIcon, threeIcon, fourIcon, fiveIcon, sixIcon,
    sevenIcon, eightIcon, nextIcon, cancelIcon;
  backIcon = cg.readEntry("ListIconBack", listConfiguration->getListDefaultIcon(CommandListElements::Back));
  oneIcon = cg.readEntry("ListIconOne", listConfiguration->getListDefaultIcon(CommandListElements::One));
  twoIcon = cg.readEntry("ListIconTwo", listConfiguration->getListDefaultIcon(CommandListElements::Two));
  threeIcon = cg.readEntry("ListIconThree", listConfiguration->getListDefaultIcon(CommandListElements::Three));
  fourIcon = cg.readEntry("ListIconFour", listConfiguration->getListDefaultIcon(CommandListElements::Four));
  fiveIcon = cg.readEntry("ListIconFive", listConfiguration->getListDefaultIcon(CommandListElements::Five));
  sixIcon = cg.readEntry("ListIconSix", listConfiguration->getListDefaultIcon(CommandListElements::Six));
  sevenIcon = cg.readEntry("ListIconSeven", listConfiguration->getListDefaultIcon(CommandListElements::Seven));
  eightIcon = cg.readEntry("ListIconEight", listConfiguration->getListDefaultIcon(CommandListElements::Eight));
  nextIcon = cg.readEntry("ListIconNext", listConfiguration->getListDefaultIcon(CommandListElements::Next));
  cancelIcon = cg.readEntry("ListIconCancel", listConfiguration->getListDefaultIcon(CommandListElements::Cancel));

  QString backVisibleTrigger, oneVisibleTrigger, twoVisibleTrigger, threeVisibleTrigger,
    fourVisibleTrigger, fiveVisibleTrigger, sixVisibleTrigger, sevenVisibleTrigger,
    eightVisibleTrigger, nextVisibleTrigger, cancelVisibleTrigger;
  backVisibleTrigger = cg.readEntry("ListBackVisibleTrigger", listConfiguration->getListDefaultVisibleTrigger(CommandListElements::Back));
  oneVisibleTrigger = cg.readEntry("ListOneVisibleTrigger", listConfiguration->getListDefaultVisibleTrigger(CommandListElements::One));
  twoVisibleTrigger = cg.readEntry("ListTwoVisibleTrigger", listConfiguration->getListDefaultVisibleTrigger(CommandListElements::Two));
  threeVisibleTrigger = cg.readEntry("ListThreeVisibleTrigger", listConfiguration->getListDefaultVisibleTrigger(CommandListElements::Three));
  fourVisibleTrigger = cg.readEntry("ListFourVisibleTrigger", listConfiguration->getListDefaultVisibleTrigger(CommandListElements::Four));
  fiveVisibleTrigger = cg.readEntry("ListFiveVisibleTrigger", listConfiguration->getListDefaultVisibleTrigger(CommandListElements::Five));
  sixVisibleTrigger = cg.readEntry("ListSixVisibleTrigger", listConfiguration->getListDefaultVisibleTrigger(CommandListElements::Six));
  sevenVisibleTrigger = cg.readEntry("ListSevenVisibleTrigger", listConfiguration->getListDefaultVisibleTrigger(CommandListElements::Seven));
  eightVisibleTrigger = cg.readEntry("ListEightVisibleTrigger", listConfiguration->getListDefaultVisibleTrigger(CommandListElements::Eight));
  nextVisibleTrigger = cg.readEntry("ListNextVisibleTrigger", listConfiguration->getListDefaultVisibleTrigger(CommandListElements::Next));
  cancelVisibleTrigger = cg.readEntry("ListCancelVisibleTrigger", listConfiguration->getListDefaultVisibleTrigger(CommandListElements::Cancel));

  cg.sync();

  listConfiguration->prepareToLoad();

  listConfiguration->registerVoiceInterfaceCommand(CommandListElements::Back, backTriggers, backVisibleTrigger, showBackIcon, backIcon);
  listConfiguration->registerVoiceInterfaceCommand(CommandListElements::One, oneTriggers, oneVisibleTrigger, showOneIcon, oneIcon);
  listConfiguration->registerVoiceInterfaceCommand(CommandListElements::Two, twoTriggers, twoVisibleTrigger, showTwoIcon, twoIcon);
  listConfiguration->registerVoiceInterfaceCommand(CommandListElements::Three, threeTriggers, threeVisibleTrigger, showThreeIcon, threeIcon);
  listConfiguration->registerVoiceInterfaceCommand(CommandListElements::Four, fourTriggers, fourVisibleTrigger, showFourIcon, fourIcon);
  listConfiguration->registerVoiceInterfaceCommand(CommandListElements::Five, fiveTriggers, fiveVisibleTrigger, showFiveIcon, fiveIcon);
  listConfiguration->registerVoiceInterfaceCommand(CommandListElements::Six, sixTriggers, sixVisibleTrigger, showSixIcon, sixIcon);
  listConfiguration->registerVoiceInterfaceCommand(CommandListElements::Seven, sevenTriggers, sevenVisibleTrigger, showSevenIcon, sevenIcon);
  listConfiguration->registerVoiceInterfaceCommand(CommandListElements::Eight, eightTriggers, eightVisibleTrigger, showEightIcon, eightIcon);
  listConfiguration->registerVoiceInterfaceCommand(CommandListElements::Next, nextTriggers, nextVisibleTrigger, showNextIcon, nextIcon);
  listConfiguration->registerVoiceInterfaceCommand(CommandListElements::Cancel, cancelTriggers, cancelVisibleTrigger, showCancelIcon, cancelIcon);

  listConfiguration->loadFinished();
  ActionManager::getInstance()->retrieveRecognitionResultFilteringParameters();
}

void CommandSettingsInternal::defaults()
{
  KConfigGroup cg(config, "");
  cg.writeEntry("MinimumConfidence", 0.45f);
  cg.writeEntry("UseDYM", false);
  cg.writeEntry("PluginBaseFont", QFont());
  ScenarioManager::getInstance()->setPluginFont(QFont());

  cg.deleteEntry("ListBackTriggers");
  cg.deleteEntry("ListOneTriggers");
  cg.deleteEntry("ListTwoTriggers");
  cg.deleteEntry("ListThreeTriggers");
  cg.deleteEntry("ListFourTriggers");
  cg.deleteEntry("ListFiveTriggers");
  cg.deleteEntry("ListSixTriggers");
  cg.deleteEntry("ListSevenTriggers");
  cg.deleteEntry("ListEightTriggers");
  cg.deleteEntry("ListNextTriggers");
  cg.deleteEntry("ListCancelTriggers");

  cg.deleteEntry("ListShowBackIcon");
  cg.deleteEntry("ListShowOneIcon");
  cg.deleteEntry("ListShowTwoIcon");
  cg.deleteEntry("ListShowThreeIcon");
  cg.deleteEntry("ListShowFourIcon");
  cg.deleteEntry("ListShowFiveIcon");
  cg.deleteEntry("ListShowSixIcon");
  cg.deleteEntry("ListShowSevenIcon");
  cg.deleteEntry("ListShowEightIcon");
  cg.deleteEntry("ListShowNextIcon");
  cg.deleteEntry("ListShowCancelIcon");

  cg.deleteEntry("ListBackIcon");
  cg.deleteEntry("ListIconOne");
  cg.deleteEntry("ListIconTwo");
  cg.deleteEntry("ListIconThree");
  cg.deleteEntry("ListIconFour");
  cg.deleteEntry("ListIconFive");
  cg.deleteEntry("ListIconSix");
  cg.deleteEntry("ListIconSeven");
  cg.deleteEntry("ListIconEight");
  cg.deleteEntry("ListIconNext");
  cg.deleteEntry("ListCancelIcon");

  cg.deleteEntry("ListBackVisibleTrigger");
  cg.deleteEntry("ListOneVisibleTrigger");
  cg.deleteEntry("ListTwoVisibleTrigger");
  cg.deleteEntry("ListThreeVisibleTrigger");
  cg.deleteEntry("ListFourVisibleTrigger");
  cg.deleteEntry("ListFiveVisibleTrigger");
  cg.deleteEntry("ListSixVisibleTrigger");
  cg.deleteEntry("ListSevenVisibleTrigger");
  cg.deleteEntry("ListEightVisibleTrigger");
  cg.deleteEntry("ListNextVisibleTrigger");
  cg.deleteEntry("ListCancelVisibleTrigger");

  storedConfidence = 0.45f;
  storedDYM = false;
  storedFont = QFont();

  cg.sync();
}


float CommandSettingsInternal::minimumConfidence()
{
  return storedConfidence;
}


bool CommandSettingsInternal::useDYM()
{
  return storedDYM;
}


QFont CommandSettingsInternal::pluginBaseFont()
{
  return storedFont;
}


ListConfiguration* CommandSettingsInternal::getListConfiguration()
{
  return listConfiguration;
}


QHash<CommandListElements::Element, VoiceInterfaceCommand*> CommandSettingsInternal::getListInterfaceCommands()
{
  return listConfiguration->getListInterfaceCommands();
}


CommandSettingsInternal::~CommandSettingsInternal()
{
  kDebug() << "DELETING COMMAND SETTINGS INTERNAL";
  instance = 0;
}

