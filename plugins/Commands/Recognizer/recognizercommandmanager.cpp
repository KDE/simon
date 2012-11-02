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
#include "recognizercommandmanager.h"
#include "recognizerconfiguration.h"
#include <eventsimulation/eventhandler.h>
#include <KLocalizedString>
#include <KMessageBox>
#include <QDebug>

K_PLUGIN_FACTORY( RecognizerCommandPluginFactory,
registerPlugin< RecognizerCommandManager >();
)

K_EXPORT_PLUGIN( RecognizerCommandPluginFactory("simonrecognizercommand") )

RecognizerCommandManager::RecognizerCommandManager(QObject* parent, const QVariantList& args) :CommandManager((Scenario*) parent, args)
{
}


bool RecognizerCommandManager::trigger(const QString& triggerName)
{
  QString appendText = static_cast<RecognizerConfiguration*>(config)->appendText();
  EventHandler::getInstance()->sendWord(triggerName+appendText+"yoyoyo");
  return true;
}


const QString RecognizerCommandManager::name() const
{
  return i18n("Recognizer");
}


const QString RecognizerCommandManager::iconSrc() const
{
  return ("text-field");
}


bool RecognizerCommandManager::deSerializeConfig(const QDomElement& elem)
{
  config = new RecognizerConfiguration(parentScenario);
  return config->deSerialize(elem);
}

RecognizerCommandManager::~RecognizerCommandManager()
{
}
