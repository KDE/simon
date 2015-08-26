/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#include "listconfiguration.h"
#include <KIconThemes/KIconButton>
#include "ui_listconfigurationdlg.h"

#include <simonscenarios/voiceinterfacecommand.h>

#include <QListWidget>


/**
 * \brief Constructor
 *
 * Initializes the Systemwidget by giving name, icon and description
 *
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
ListConfiguration::ListConfiguration(QWidget* parent): QWidget(parent)
{
  ui = new Ui::ListConfigurationDlg();
  ui->setupUi(this);

  QObject::connect(ui->elbTriggers, SIGNAL(changed()), this, SIGNAL(changed()));
  QObject::connect(ui->cbShowIcon, SIGNAL(toggled(bool)), this, SIGNAL(changed()));
  QObject::connect(ui->leVisibleTrigger, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  QObject::connect(ui->ibIcon, SIGNAL(iconChanged(QString)), this, SIGNAL(changed()));

  QObject::connect(ui->lwActions, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(listActionsItemChanged(QListWidgetItem*,QListWidgetItem*)));
}


void ListConfiguration::prepareToSave()
{
  storeCurrentlyDisplayedElement(getRowElementType(ui->lwActions->currentRow()));
}


void ListConfiguration::prepareToLoad()
{
  qDeleteAll(listInterfaceCommands);
  listInterfaceCommands.clear();
}


void ListConfiguration::loadFinished()
{
  ui->lwActions->setCurrentRow(0);
  displayListElement(CommandListElements::Back);
}


CommandListElements::Element ListConfiguration::getRowElementType(int row)
{
  switch (row) {
    case 0: return CommandListElements::Back;
    case 1: return CommandListElements::One;
    case 2: return CommandListElements::Two;
    case 3: return CommandListElements::Three;
    case 4: return CommandListElements::Four;
    case 5: return CommandListElements::Five;
    case 6: return CommandListElements::Six;
    case 7: return CommandListElements::Seven;
    case 8: return CommandListElements::Eight;
    case 9: return CommandListElements::Next;
    default: return CommandListElements::Cancel;
  }
}


QString ListConfiguration::getListSelectionId(CommandListElements::Element type)
{
  switch (type) {
    case CommandListElements::Back: return "listSelectBack";
    case CommandListElements::One: return "listSelectOne";
    case CommandListElements::Two: return "listSelectTwo";
    case CommandListElements::Three: return "listSelectThree";
    case CommandListElements::Four: return "listSelectFour";
    case CommandListElements::Five: return "listSelectFive";
    case CommandListElements::Six: return "listSelectSix";
    case CommandListElements::Seven: return "listSelectSeven";
    case CommandListElements::Eight: return "listSelectEight";
    case CommandListElements::Next: return "listSelectNext";
    case CommandListElements::Cancel: return "listSelectCancel";
  }

  return "";
}


QString ListConfiguration::getListDefaultIcon(CommandListElements::Element type)
{
  if (type == CommandListElements::Cancel) return "dialog-cancel";

  return "view-choose";
}


bool ListConfiguration::getListDefaultShowIcon(CommandListElements::Element type)
{
  if (type == CommandListElements::Cancel) return true;

  return false;
}


QString ListConfiguration::getListSelectionDescription(CommandListElements::Element type)
{
  switch (type) {
    case CommandListElements::Back: return i18n("Selects the \"Back\" option");
    case CommandListElements::One: return i18n("Selects the \"One\" option");
    case CommandListElements::Two: return i18n("Selects the \"Two\" option");
    case CommandListElements::Three: return i18n("Selects the \"Three\" option");
    case CommandListElements::Four: return i18n("Selects the \"Four\" option");
    case CommandListElements::Five: return i18n("Selects the \"Five\" option");
    case CommandListElements::Six: return i18n("Selects the \"Six\" option");
    case CommandListElements::Seven: return i18n("Selects the \"Seven\" option");
    case CommandListElements::Eight: return i18n("Selects the \"Eight\" option");
    case CommandListElements::Next: return i18n("Selects the \"Next\" option");
    case CommandListElements::Cancel: return i18n("Selects the \"Cancel\" option");
  }

  return "";
}


QString ListConfiguration::getListDefaultTrigger(CommandListElements::Element type)
{
  switch (type) {
    case CommandListElements::Back: return i18nc("Number", "Zero");
    case CommandListElements::One: return i18nc("Number", "One");
    case CommandListElements::Two: return i18nc("Number", "Two");
    case CommandListElements::Three: return i18nc("Number", "Three");
    case CommandListElements::Four: return i18nc("Number", "Four");
    case CommandListElements::Five: return i18nc("Number", "Five");
    case CommandListElements::Six: return i18nc("Number", "Six");
    case CommandListElements::Seven: return i18nc("Number", "Seven");
    case CommandListElements::Eight: return i18nc("Number", "Eight");
    case CommandListElements::Next: return i18nc("Number", "Nine");
    case CommandListElements::Cancel: return i18n("Cancel");
  }

  return "";
}


QString ListConfiguration::getListDefaultVisibleTrigger(CommandListElements::Element type)
{
  switch (type) {
    case CommandListElements::Back: return "0";
    case CommandListElements::One: return "1";
    case CommandListElements::Two: return "2";
    case CommandListElements::Three: return "3";
    case CommandListElements::Four: return "4";
    case CommandListElements::Five: return "5";
    case CommandListElements::Six: return "6";
    case CommandListElements::Seven: return "7";
    case CommandListElements::Eight: return "8";
    case CommandListElements::Next: return "9";
    case CommandListElements::Cancel: return i18n("Cancel");
  }

  return "";
}


void ListConfiguration::storeCurrentlyDisplayedElement(CommandListElements::Element type)
{
  QList<VoiceInterfaceCommand*> commands = listInterfaceCommands.values(type);
  listInterfaceCommands.remove(type);
  qDeleteAll(commands);

  registerVoiceInterfaceCommand(type, ui->elbTriggers->items(), ui->leVisibleTrigger->text(),
    ui->cbShowIcon->isChecked(), ui->ibIcon->icon());
}


void ListConfiguration::listActionsItemChanged(QListWidgetItem *newItem, QListWidgetItem *item)
{
  if (!item || !newItem || (ui->lwActions->row(item) == ui->lwActions->row(newItem))) return;

  //store current item
  storeCurrentlyDisplayedElement(getRowElementType(ui->lwActions->row(item)));

  //display new item
  displayListElement(getRowElementType(ui->lwActions->row(newItem)));
}


QStringList ListConfiguration::getListTriggers(CommandListElements::Element type)
{
  QStringList out;

  QList<VoiceInterfaceCommand*> commands = listInterfaceCommands.values(type);

  foreach (VoiceInterfaceCommand* c, commands)
    out << c->getTrigger();

  return out;
}


bool ListConfiguration::getListShowIcon(CommandListElements::Element type)
{
  QList<VoiceInterfaceCommand*> commands = listInterfaceCommands.values(type);
  if (commands.isEmpty())
    return getListDefaultShowIcon(type);
  return commands[0]->showIcon();
}


QString ListConfiguration::getListIcon(CommandListElements::Element type)
{
  QList<VoiceInterfaceCommand*> commands = listInterfaceCommands.values(type);
  if (commands.isEmpty())
    return getListDefaultIcon(type);
  return commands[0]->getIconSrc();
}


QString ListConfiguration::getListVisibleTrigger(CommandListElements::Element type)
{
  QList<VoiceInterfaceCommand*> commands = listInterfaceCommands.values(type);
  if (commands.isEmpty())
    return getListDefaultTrigger(type);
  return commands[0]->visibleTrigger();
}


void ListConfiguration::displayListElement(CommandListElements::Element type)
{
  ui->cbShowIcon->setChecked(getListShowIcon(type));
  ui->ibIcon->setIcon(getListIcon(type));
  ui->leVisibleTrigger->setText(getListVisibleTrigger(type));
  ui->elbTriggers->setItems(getListTriggers(type));
}


void ListConfiguration::registerVoiceInterfaceCommand(CommandListElements::Element element,
const QStringList& triggers, const QString& visibleTrigger, bool showIcon, const QString& iconSrc)
{
  foreach (const QString& trigger, triggers) {
    //qDebug() << "Storing element: " << trigger;
    listInterfaceCommands.insertMulti(element, new VoiceInterfaceCommand(0, trigger, iconSrc,
      getListSelectionDescription(element), getListSelectionId(element),
      QList<int>() << 0, 0, visibleTrigger, showIcon, false));
  }

}


void ListConfiguration::registerVoiceInterfaceCommands(QHash<CommandListElements::Element, VoiceInterfaceCommand*> commands)
{
  QHashIterator<CommandListElements::Element, VoiceInterfaceCommand*> i(commands);
  while (i.hasNext()) {
    i.next();
    listInterfaceCommands.insertMulti(i.key(), i.value());
  }
}


QHash<CommandListElements::Element, VoiceInterfaceCommand*> ListConfiguration::getListInterfaceCommands()
{
  return listInterfaceCommands;
}


ListConfiguration::~ListConfiguration()
{
	qDebug() << "DELETING LIST CONFIGURATION";
  delete ui;
}
