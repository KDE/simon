/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
 *   Copyright (C) 2010 Manfred Scheucher <deadalps@gmail.com>
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

#include "shortcutcommand.h"
#include <QVariant>
#include <QDomDocument>
#include <QDomElement>
#include <KLocalizedString>
#include <eventsimulation/eventhandler.h>

const QString ShortcutCommand::staticCategoryText()
{
  return i18n("Shortcut");
}


const QString ShortcutCommand::getCategoryText() const
{
  return ShortcutCommand::staticCategoryText();
}


const QIcon ShortcutCommand::staticCategoryIcon()
{
  return QIcon::fromTheme("go-jump-locationbar");
}


const QIcon ShortcutCommand::getCategoryIcon() const
{
  return ShortcutCommand::staticCategoryIcon();
}


const QMap<QString,QVariant> ShortcutCommand::getValueMapPrivate() const
{
  QMap<QString,QVariant> out;

  out.insert(i18n("Shortcut"), shortcut.toString());
  out.insert(i18n("Mode"), prettyPrintMode());
  return out;
}


QString ShortcutCommand::prettyPrintMode() const
{
  switch(mode)
  {
    case EventSimulation::Press:
      return i18n("Only Press");
    case EventSimulation::Release:
      return i18n("Only Release");
    default:
      return i18n("Press and release");
  }
}

bool ShortcutCommand::triggerPrivate(int *state)
{
  Q_UNUSED(state);
  EventHandler::getInstance()->sendShortcut(getShortcut(),mode);
  return true;
}


QDomElement ShortcutCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
  QDomElement shortcutElem = doc->createElement("shortcut");
  QDomElement modeElem = doc->createElement("mode");
  
  shortcutElem.appendChild(doc->createTextNode(shortcut.toString()));
  modeElem.appendChild(doc->createTextNode(QString::number(mode)));

  commandElem.appendChild(shortcutElem);
  commandElem.appendChild(modeElem);

  return commandElem;
}


bool ShortcutCommand::deSerializePrivate(const QDomElement& commandElem)
{
  QDomElement shortcutElem = commandElem.firstChildElement("shortcut");
  if (shortcutElem.isNull()) return false;
  
  shortcut = QKeySequence(shortcutElem.text());
  
  QDomElement modeElem = commandElem.firstChildElement("mode");  
  if (modeElem.isNull())
    mode = (EventSimulation::PressMode) ( EventSimulation::Press | EventSimulation::Release );
  else
    mode = (EventSimulation::PressMode) modeElem.text().toInt();

  return true;
}


STATIC_CREATE_INSTANCE_C(ShortcutCommand);
