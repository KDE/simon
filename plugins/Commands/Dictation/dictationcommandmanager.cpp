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
#include "dictationcommandmanager.h"
#include "dictationconfiguration.h"
#include <eventsimulation/eventhandler.h>
#include <KLocalizedString>

K_PLUGIN_FACTORY( DictationCommandPluginFactory,
registerPlugin< DictationCommandManager >();
)

K_EXPORT_PLUGIN( DictationCommandPluginFactory("simondictationcommand") )

DictationCommandManager::DictationCommandManager(QObject* parent, const QVariantList& args) :CommandManager((Scenario*) parent, args)
{
}


bool DictationCommandManager::trigger(const QString& triggerName)
{
  QString appendText = static_cast<DictationConfiguration*>(config)->appendText();
  EventHandler::getInstance()->sendWord(triggerName+appendText);
  return true;
}


const QString DictationCommandManager::name() const
{
  return i18n("Dictation");
}


const QString DictationCommandManager::iconSrc() const
{
  return ("text-field");
}


bool DictationCommandManager::deSerializeConfig(const QDomElement& elem)
{
  config = new DictationConfiguration(parentScenario);
  return config->deSerialize(elem);
}

DictationCommandManager::~DictationCommandManager()
{
}
