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

#include "filtercommandmanager.h"
#include <simonlogging/logger.h>
#include <simoninfo/simoninfo.h>
#include <QRegExp>
#include <QTimer>
#include <KLocalizedString>
#include <KGenericFactory>
#include <KAction>
#include "filterconfiguration.h"

K_PLUGIN_FACTORY( FilterPluginFactory,
registerPlugin< FilterCommandManager >();
)

K_EXPORT_PLUGIN( FilterPluginFactory("simonfiltercommand") )

FilterCommandManager::FilterCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args),
isActive(false),
stageOne(false),
activateAction(new KAction(this))
{
  activateAction->setText(i18n("Activate Filter"));
  activateAction->setIcon(KIcon("view-filter"));
  connect(activateAction, SIGNAL(triggered(bool)),
    this, SLOT(toggle()));

  guiActions << activateAction;
}


const QString FilterCommandManager::name() const
{
  return i18n("Filter");
}


const QString FilterCommandManager::iconSrc() const
{
  return ("view-filter");
}


void FilterCommandManager::updateAction()
{
  if (!isActive) {
    //SimonInfo::showMessage(i18n("Filter deactivated"), 2500, new KIcon("view-filter"));
    activateAction->setText(i18n("Activate Filter"));
  }
  else {
    //SimonInfo::showMessage(i18n("Filter activated"), 2500, new KIcon("view-filter"));
    activateAction->setText(i18n("Deactivate Filter"));
  }
}


void FilterCommandManager::toggle()
{
  Logger::log(i18n("Changing filter state..."));

  isActive = !isActive;
  updateAction();
}

void FilterCommandManager::activateFilter()
{
  if (isActive) return;
  toggle();
  stageOne = false;
}

void FilterCommandManager::deactivateFilter()
{
  if (!isActive) return;
  toggle();
  stageOne = false;
  timeoutTimer.stop();
}

void FilterCommandManager::deactivateOnce()
{
  if (!isActive) return;
  stageOne = true;
  kDebug() << "Deactivating once..."  << configuration()->autoLeaveStageOne();
  if (configuration()->autoLeaveStageOne())
  {
    kDebug() << "Starting timeout...";
    timeoutTimer.stop();
    timeoutTimer.setInterval(configuration()->autoLeaveStageOneTimeout());
    timeoutTimer.start();
  }
}

FilterConfiguration* FilterCommandManager::configuration()
{
  return static_cast<FilterConfiguration*>(config);
}

void FilterCommandManager::leaveStageOne()
{
  if (!configuration()->twoStage() || !stageOne) return;
  kDebug() << "Leaving stage one...";

  stageOne = false;
  switchToState(SimonCommand::DefaultState+1);
}

bool FilterCommandManager::trigger(const QString& triggerName, bool silent)
{
  kDebug() << "Filter state: " << m_currentState;
  if ((m_currentState == SimonCommand::DefaultState+1) && (!configuration()->twoStage()))
  {
    switchToState(SimonCommand::DefaultState+2); // if not in two stage mode, "upgrade" immediatly
  }

  kDebug() << "Triggering: " << triggerName;
  if (CommandManager::trigger(triggerName, silent))
    return true;

  kDebug() << "Still here";
  if (configuration()->twoStage() && stageOne) {
    kDebug() << "Switching to " << SimonCommand::DefaultState+1;
    switchToState(SimonCommand::DefaultState+1);
  }

  //would pass through - should it?
  if (!isActive || (configuration()->twoStage() && stageOne && configuration()->relayStageOne()))
  {
    stageOne = false;
    return false;
  }

  stageOne = false;

  if (triggerName.contains(QRegExp(configuration()->regExp())))
    return true;                                  //matches so filter it out!

  //not for us
  return false;
}


bool FilterCommandManager::deSerializeConfig(const QDomElement& elem)
{
  bool succ = true;

  config = new FilterConfiguration(parentScenario);
  succ = config->deSerialize(elem);

  succ &= installInterfaceCommand(this, "activateFilter", i18n("Activate filter"), "view-filter",
    i18n("Starts filtering"), true /* announce */, true /* show icon */,
    SimonCommand::DefaultState /* consider this command when in this state */,
    SimonCommand::DefaultState+1,                 /* if executed switch to this state */
    QString() /* take default visible id from action name */,
    "startFiltering" /* id */);

  succ &= installInterfaceCommand(this, "deactivateFilter", i18n("Deactivate filter"), "view-filter",
    i18n("Stops filtering"), true /* announce */, true /* show icon */,
    QList<int>() << SimonCommand::DefaultState+2,
    SimonCommand::DefaultState,                   /* if executed switch to this state */
    QString() /* take default visible id from action name */,
    "stopsFiltering" /* id */);

  succ &= installInterfaceCommand(this, "deactivateOnce", i18n("Deactivate filter once"), "view-filter",
    i18n("When two stage mode is activated; Will consider the next recognition result and either only listen for the command to deactivate the filter completely or also pass the next result through to other plugins depending on the setting in the configuration."), true /* announce */, true /* show icon */,
    /* consider this command when in this state */
    QList<int>() << SimonCommand::DefaultState+1 << SimonCommand::DefaultState+2,
    SimonCommand::DefaultState+2,                   /* if executed switch to this state */
    QString() /* take default visible id from action name */,
    "stopsFilteringOnce" /* id */);

  if (!succ)
    kDebug() << "Something went wrong!";
  
  connect(&timeoutTimer, SIGNAL(timeout()), this, SLOT(leaveStageOne()));
  timeoutTimer.setSingleShot(true);

  return succ;
}


const QString FilterCommandManager::preferredTrigger() const
{
  return "";
}


FilterCommandManager::~FilterCommandManager()
{
}
