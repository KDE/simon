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

#include "compositecommand.h"
#include <simonactions/actionmanager.h>
#include <QIcon>
#include <KLocalizedString>
#include <QVariant>
#include <QDomElement>
#include <QDomDocument>
#ifdef Q_OS_WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <QDebug>

const QString CompositeCommand::staticCategoryText()
{
  return i18n("Composite");
}


const QString CompositeCommand::getCategoryText() const
{
  return CompositeCommand::staticCategoryText();
}


const QIcon CompositeCommand::staticCategoryIcon()
{
  return QIcon::fromTheme("view-choose");
}


const QIcon CompositeCommand::getCategoryIcon() const
{
  return CompositeCommand::staticCategoryIcon();
}


const QMap<QString,QVariant> CompositeCommand::getValueMapPrivate() const
{
  QMap<QString,QVariant> out;
  out.insert(i18n("Commands"), commands.join("\n"));
  return out;

}


QDomElement CompositeCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
  Q_ASSERT(commands.count() == commandTypes.count());

  QDomElement passThroughElem = doc->createElement("passThrough");
  passThroughElem.appendChild(doc->createTextNode(passThrough ? "1" : "0"));
  
  commandElem.appendChild(passThroughElem);
  
  QDomElement childCommandsElement = doc->createElement("childCommands");

  for (int i=0; i < commands.count(); i++) {
    QDomElement childComElement = doc->createElement("childCommand");
    QDomElement childTriggerElem = doc->createElement("trigger");
    QDomElement childCategoryElem = doc->createElement("category");

    childTriggerElem.appendChild(doc->createTextNode(commands[i]));
    childCategoryElem.appendChild(doc->createTextNode(commandTypes[i]));

    childComElement.appendChild(childTriggerElem);
    childComElement.appendChild(childCategoryElem);
    childCommandsElement.appendChild(childComElement);
  }
  commandElem.appendChild(childCommandsElement);

  return commandElem;
}


void CompositeCommand::triggerPrivateThread()
{
  for (int i=0; i<commands.count();i++) {
    QString type = commandTypes[i];
    qDebug() << type << "Delay";
    if (type==i18n("Delay")) {
      bool ok=true;
      qDebug() << commands[i];
      int amount = commands[i].toInt(&ok);
      if (!ok) {
        qDebug() << "Not ok";
        continue;
      }
      
      #ifdef Q_OS_WIN32
      Sleep(amount);
      #else
      usleep(amount*1000);
      #endif
    }
    else
      QMetaObject::invokeMethod(ActionManager::getInstance(), "triggerCommand", Qt::QueuedConnection, 
          QGenericReturnArgument(), Q_ARG(QString, commandTypes[i]), Q_ARG(QString, commands[i]), Q_ARG(bool, true));
  }
}


bool CompositeCommand::triggerPrivate(int *state)
{
  Q_UNUSED(state);
  Q_ASSERT(commands.count() == commandTypes.count());

  //QtConcurrent::run(this, &CompositeCommand::triggerPrivateThread);
  triggerPrivateThread();

  qDebug() << "Returning " << !passThrough;
  return !passThrough;
}


bool CompositeCommand::deSerializePrivate(const QDomElement& commandElem)
{
  QDomElement childCommandsElem = commandElem.firstChildElement("childCommands");
  if (childCommandsElem.isNull()) return false;

  passThrough = (commandElem.firstChildElement("passThrough").text() == "1");
  
  commands.clear();
  commandTypes.clear();

  QDomElement childCommandElem = childCommandsElem.firstChildElement();

  while (!childCommandElem.isNull()) {
    QDomElement childCommandTriggerElem = childCommandElem.firstChildElement();
    QDomElement childCommandCategoryElem = childCommandTriggerElem.nextSiblingElement();
    commands << childCommandTriggerElem.text();
    commandTypes << childCommandCategoryElem.text();
    childCommandElem = childCommandElem.nextSiblingElement();
  }
  qDebug() << "Triggers: " << commands;
  qDebug() << "Categories: " << commandTypes;

  return true;
}


STATIC_CREATE_INSTANCE_C(CompositeCommand);
