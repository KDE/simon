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

#include "listcommand.h"
#include "commandlistwidget.h"
#include "actionmanager.h"
#include <simonscenarios/commandmanager.h>
#include <simonscenarios/voiceinterfacecommand.h>
#include <unistd.h>
#include <QList>
#include <QTableWidget>
#include <QDomElement>
#include <QDomDocument>
#include <QVariant>
#include <QIcon>
#include <KI18n/klocalizedstring.h>

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

ListCommand::ListCommand(CommandManager *parentManger, const QString& name, const QString& iconSrc, const QString& description,
const QStringList& commands_,
const QStringList& iconSrcs_, const QStringList& commandTypes_) : Command(name, iconSrc, description),
GreedyReceiver(0 /* no manager */),
m_parentManager(parentManger),
clw(new CommandListWidget()),
startIndex(0),
iconsrcs(iconSrcs_),
commands(commands_),
commandTypes(commandTypes_)
{
  init();
}


ListCommand::ListCommand(CommandManager *parentManager) : Command(),
GreedyReceiver(0 /* no manager */),
m_parentManager(parentManager),
clw(new CommandListWidget()),
startIndex(0)
{
  init();
}


void ListCommand::init()
{
  connect(clw, SIGNAL(canceled()), this, SLOT(cancel()));
  connect(clw, SIGNAL(runRequest(int)), this, SLOT(processRequest(int)));
}


QDomElement ListCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
  Q_ASSERT(commands.count() == commandTypes.count());
  Q_ASSERT(commandTypes.count() == iconsrcs.count());

  QDomElement childCommandsElement = doc->createElement("childCommands");

  for (int i=0; i < commands.count(); i++) {
    QDomElement childComElement = doc->createElement("childCommand");
    QDomElement childTriggerElem = doc->createElement("trigger");
    QDomElement childIconElem = doc->createElement("icon");
    QDomElement childCategoryElem = doc->createElement("category");

    childTriggerElem.appendChild(doc->createTextNode(commands[i]));
    childIconElem.appendChild(doc->createTextNode(iconsrcs[i]));
    childCategoryElem.appendChild(doc->createTextNode(commandTypes[i]));

    childComElement.appendChild(childTriggerElem);
    childComElement.appendChild(childIconElem);
    childComElement.appendChild(childCategoryElem);
    childCommandsElement.appendChild(childComElement);
  }
  commandElem.appendChild(childCommandsElement);

  return commandElem;
}


bool ListCommand::processRequest(int index)
{
  Q_ASSERT(commands.count() == commandTypes.count());

  if (index > commands.count())
    return false;

  if (index == 0) {
    //go back
    if (startIndex > 0)
      startIndex -= 8;
    listCurrentCommandSection();
    return true;
  } else if (index == 9)
  {
    //go forward
    if (startIndex+8 < commands.count())
      startIndex += 8;
    listCurrentCommandSection();
    return true;
  }
  else {
    //execute list entry
    // if index==1, we want it to represent the _first_ entry in the list (index==0)
    index--;
    index += startIndex;

    Q_ASSERT(commands.count() == commandTypes.count());
    if (index >= commands.count())
      return false;

    clw->runRequestSent();
    clw->close();
    stopGreedy();
    #ifndef Q_OS_WIN32
    usleep(300000);
    #else
    Sleep(300);
    #endif
    //TODO: Clean solution
    ActionManager::getInstance()->triggerCommand(commandTypes[index], commands[index]);
    emit entrySelected();
  }
  return false;
}


void ListCommand::cancel()
{
  clw->close();
  stopGreedy();
  emit canceled();
}


bool ListCommand::greedyTrigger(const QString& inputText)
{
  qDebug() << "Triggering greedy " << inputText;

  QHash<CommandListElements::Element, VoiceInterfaceCommand*> adaption = getAdaption();

  QList<CommandListElements::Element> adaptionKeys = adaption.keys();
  foreach (const CommandListElements::Element& element, adaptionKeys) {
    QList<VoiceInterfaceCommand*> interfaceCommands = adaption.values(element);
    foreach (VoiceInterfaceCommand* command, interfaceCommands) {
      if (command->matches(0, inputText)) {
        switch (element) {
          case CommandListElements::Back: return processRequest(0);
          case CommandListElements::One: return processRequest(1);
          case CommandListElements::Two: return processRequest(2);
          case CommandListElements::Three: return processRequest(3);
          case CommandListElements::Four: return processRequest(4);
          case CommandListElements::Five: return processRequest(5);
          case CommandListElements::Six: return processRequest(6);
          case CommandListElements::Seven: return processRequest(7);
          case CommandListElements::Eight: return processRequest(8);
          case CommandListElements::Next: return processRequest(9);
          case CommandListElements::Cancel:
            clw->close();
            return true;
        }
      }

    }

  }

  /*
  if (inputText.toUpper() == i18n("Cancel").toUpper())
  {
    clw->close();
    return true;
  }

  bool ok=false;
  int index = inputText.toInt(&ok);
  if (!ok)
  {
  while ((index < numberIdentifiers.count()) && (numberIdentifiers.at(index).toUpper() != inputText.toUpper()))
  index++;

  if (index == numberIdentifiers.count()) return false;
  }

  return processRequest(index);
  */
  return false;
}


ListCommand* ListCommand::createInstance(CommandManager *manager, const QDomElement& element)
{
  ListCommand *command = new ListCommand(manager);
  if (!command->deSerialize(element)) {
    delete command;
    return 0;
  }
  return command;
}


const QString ListCommand::staticCategoryText()
{
  return i18n("List");
}


const QString ListCommand::getCategoryText() const
{
  return ListCommand::staticCategoryText();
}


const QIcon ListCommand::staticCategoryIcon()
{
  return QIcon::fromTheme("view-choose");
}


const QIcon ListCommand::getCategoryIcon() const
{
  return ListCommand::staticCategoryIcon();
}


const QMap<QString,QVariant> ListCommand::getValueMapPrivate() const
{
  QMap<QString,QVariant> out;
  out.insert(i18n("Commands"), commands.join("\n"));
  return out;
}


void ListCommand::listCurrentCommandSection()
{
  QStringList nowIconSrcs, nowCommands;
  for (int i=startIndex; (i < commands.count()) && (i-startIndex < 8); i++) {
    nowIconSrcs << iconsrcs[i];
    nowCommands << commands[i];
  }
  CommandListWidget::Flags flags;
  if (startIndex > 0) {
    if (startIndex+8 < commands.count())
      flags = CommandListWidget::HasNext|CommandListWidget::HasBack;
    else
      flags = CommandListWidget::HasBack;
  } else
  if (startIndex+8 < commands.count())
    flags = CommandListWidget::HasNext;

  clw->init(nowIconSrcs, nowCommands, flags);
}


QHash<CommandListElements::Element, VoiceInterfaceCommand*> ListCommand::getAdaption()
{
  //adapt to either parent manager or commandsettings
  if (m_parentManager) {
    qDebug() << "Adapting to scenario";
    //adapt to current scenarios list configuration
    return ActionManager::getInstance()->getListInterfaceCommands();
  }

  qDebug() << "Adapting to global configuration";
  //adapt to commandsettings list configuration
  return ActionManager::getInstance()->getGlobalListInterfaceCommands();
}


bool ListCommand::triggerPrivate(int *state)
{
  Q_UNUSED(state);

  if (commands.count() == 0) return false;

  Q_ASSERT(commands.count() == commandTypes.count());
  //showing list

  clw->setWindowIcon(QIcon::fromTheme(getIconSrc()));
  clw->setWindowTitle(getTrigger());

  listCurrentCommandSection();

  startGreedy();

  m_subCommands.clear();

  QHash<CommandListElements::Element, VoiceInterfaceCommand*> adaption = getAdaption();

  QList<CommandListElements::Element> allElements;
  allElements << CommandListElements::Back
    << CommandListElements::One
    << CommandListElements::Two
    << CommandListElements::Three
    << CommandListElements::Four
    << CommandListElements::Five
    << CommandListElements::Six
    << CommandListElements::Seven
    << CommandListElements::Eight
    << CommandListElements::Next
    << CommandListElements::Cancel;

  QList<CommandListElements::Element> adaptionKeys = adaption.keys();
  foreach (const CommandListElements::Element& element, adaptionKeys) {
    QList<VoiceInterfaceCommand*> interfaceCommands = adaption.values(element);
    //list cant be empty so we do not need to check
    clw->adaptToVoiceElement(element, interfaceCommands[0]);
    m_subCommands << interfaceCommands;

    allElements.removeAll(element);
  }

  foreach (const CommandListElements::Element& elem, allElements)
    clw->adaptToVoiceElement(elem, 0);            // hide the rest

  clw->show();

  return true;
}


bool ListCommand::deSerializePrivate(const QDomElement& commandElem)
{
  QDomElement childCommandsElem = commandElem.firstChildElement("childCommands");
  if (childCommandsElem.isNull()) return false;

  iconsrcs.clear();
  commands.clear();
  commandTypes.clear();

  QDomElement childCommandElem = childCommandsElem.firstChildElement();

  while (!childCommandElem.isNull()) {
    QDomElement childCommandTriggerElem = childCommandElem.firstChildElement();
    QDomElement childCommandIconElem = childCommandTriggerElem.nextSiblingElement();
    QDomElement childCommandCategoryElem = childCommandIconElem.nextSiblingElement();
    commands << childCommandTriggerElem.text();
    iconsrcs << childCommandIconElem.text();
    commandTypes << childCommandCategoryElem.text();
    childCommandElem = childCommandElem.nextSiblingElement();
  }
  qDebug() << "Triggers: " << commands;
  qDebug() << "Icons: " << iconsrcs;
  qDebug() << "Categories: " << commandTypes;

  return true;
}


void ListCommand::setFont(const QFont& font)
{
  qDebug() << "Setting font...";
  clw->setFont(font);
}


ListCommand::~ListCommand()
{
  stopGreedy();
  clw->deleteLater();
}
