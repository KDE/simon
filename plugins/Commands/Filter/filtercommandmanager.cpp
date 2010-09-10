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
  Logger::log(i18n("[INF] Changing filter state..."));

  isActive = !isActive;
  updateAction();
}

void FilterCommandManager::activateFilter()
{
  if (isActive) return;
  toggle();
}

void FilterCommandManager::deactivateFilter()
{
  if (!isActive) return;
  toggle();
}


bool FilterCommandManager::trigger(const QString& triggerName)
{
  if (CommandManager::trigger(triggerName))
    return true;

  //would pass through - should it?
  if (!isActive)
    return false;

  if (triggerName.contains(QRegExp(dynamic_cast<FilterConfiguration*>(config)->regExp())))
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
    SimonCommand::DefaultState+1 /* consider this command when in this state */,
    SimonCommand::DefaultState,                   /* if executed switch to this state */
    QString() /* take default visible id from action name */,
    "stopsFiltering" /* id */);

  if (!succ)
    kDebug() << "Something went wrong!";

  return succ;
}


const QString FilterCommandManager::preferredTrigger() const
{
  return "";
}


FilterCommandManager::~FilterCommandManager()
{
}
