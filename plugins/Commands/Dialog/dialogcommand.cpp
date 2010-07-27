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

#include "dialogcommand.h"
#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QVariant>
#include <KIcon>
#include <KLocalizedString>
#include <KDebug>

const QString DialogCommand::staticCategoryText()
{
  return i18n("Dialog");
}


const QString DialogCommand::getCategoryText() const
{
  return DialogCommand::staticCategoryText();
}


const KIcon DialogCommand::staticCategoryIcon()
{
  return KIcon("im-user");
}


const KIcon DialogCommand::getCategoryIcon() const
{
  return DialogCommand::staticCategoryIcon();
}


const QMap<QString,QVariant> DialogCommand::getValueMapPrivate() const
{
  QMap<QString,QVariant> out;
  out.insert(i18nc("Label for a bool value", "Switch state"), m_changeDialogState ? i18n("Yes") : i18n("No"));
  out.insert(i18n("Next state"), QString::number(m_nextDialogState));
  out.insert(i18nc("Label for a bool value", "Execute command(s)"), m_executeCommands ? i18n("Yes") : i18n("No"));
  out.insert(i18n("Commands"), m_commands.join("\n"));
  return out;
}


bool DialogCommand::triggerPrivate(int *state)
{
  Q_UNUSED(state);
  kDebug() << "Triggering...";
  return true;
}


QDomElement DialogCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
  QDomElement presentationElem = doc->createElement("presentation");
  QDomElement textElem = doc->createElement("text");
  textElem.appendChild(doc->createTextNode(m_text));
  presentationElem.appendChild(textElem);
  QDomElement showIconElem = doc->createElement("icon");
  showIconElem.setAttribute("enabled", m_showIcon );
  presentationElem.appendChild(showIconElem);
  commandElem.appendChild(presentationElem);

  QDomElement switchStateElem = doc->createElement("switchState");
  switchStateElem.setAttribute("enabled", m_changeDialogState);
  switchStateElem.appendChild(doc->createTextNode(QString::number(m_nextDialogState)));
  commandElem.appendChild(switchStateElem);

  QDomElement childCommandsElement = doc->createElement("childCommands");
  childCommandsElement.setAttribute("enabled", m_executeCommands);

  for (int i=0; i < m_commands.count(); i++) {
    QDomElement childComElement = doc->createElement("childCommand");
    QDomElement childTriggerElem = doc->createElement("trigger");
    QDomElement childCategoryElem = doc->createElement("category");

    childTriggerElem.appendChild(doc->createTextNode(m_commands[i]));
    childCategoryElem.appendChild(doc->createTextNode(m_commandTypes[i]));

    childComElement.appendChild(childTriggerElem);
    childComElement.appendChild(childCategoryElem);
    childCommandsElement.appendChild(childComElement);
  }
  commandElem.appendChild(childCommandsElement);

  return commandElem;
}


bool DialogCommand::deSerializePrivate(const QDomElement& commandElem)
{
  QDomElement presentationElem = commandElem.firstChildElement("presentation");
  if (presentationElem.isNull()) return false;

  QDomElement textElem = presentationElem.firstChildElement("text");
  QDomElement showIconElem = presentationElem.firstChildElement("icon");
  m_text = textElem.text();
  m_showIcon = showIconElem.attribute("enabled").toInt();

  QDomElement switchStateElem = commandElem.firstChildElement("switchState");
  m_changeDialogState = switchStateElem.attribute("enabled").toInt();
  m_nextDialogState = switchStateElem.text().toInt();

  QDomElement childCommandsElem = commandElem.firstChildElement("childCommands");

  m_executeCommands = childCommandsElem.attribute("enabled").toInt();

  m_commands.clear();
  m_commandTypes.clear();

  QDomElement childCommandElem = childCommandsElem.firstChildElement();

  while (!childCommandElem.isNull()) {
    QDomElement childCommandTriggerElem = childCommandElem.firstChildElement();
    QDomElement childCommandCategoryElem = childCommandTriggerElem.nextSiblingElement();
    m_commands << childCommandTriggerElem.text();
    m_commandTypes << childCommandCategoryElem.text();
    childCommandElem = childCommandElem.nextSiblingElement();
  }
  kDebug() << "Triggers: " << m_commands;
  kDebug() << "Categories: " << m_commandTypes;

  kDebug() << "Deserialized " << m_text;
  return true;
}


STATIC_CREATE_INSTANCE_C(DialogCommand);
