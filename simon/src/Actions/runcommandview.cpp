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

#include "runcommandview.h"
#include <KMessageBox>
#include <KIcon>
#include <QWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QSize>
#include "actionmanager.h"
#include "newcommand.h"
#include "Commands/commandmodel.h"
#include "commandpreviewwidget.h"


/**
 *	@brief Constructor
 *
 *	@author Peter Grasch
 */
RunCommandView::RunCommandView(QWidget *parent) : InlineWidget(i18n("Kommandos"), KIcon("system-run"), i18n("Direkte ausführung von simon-Befehlen"), parent)
{
	ui.setupUi(this);

	CommandPreviewWidget *commandPreviewWidget = new CommandPreviewWidget(ui.cvCommands);
	connect(ui.cvCommands, SIGNAL(updatePreviewWidget(QModelIndex)), commandPreviewWidget, SLOT(updateCommand(QModelIndex)));
	ui.cvCommands->setPreviewWidget(commandPreviewWidget);

	guessChildTriggers((QObject*)this);

	setSettingsHidden();
	hide();

	connect ( ui.pbNewCommand, SIGNAL(clicked()), this, SLOT(addCommand()));
	connect ( ui.pbEditCommand, SIGNAL(clicked()), this, SLOT(editCommand()));
	connect ( ui.pbDeleteCommand, SIGNAL(clicked()), this, SLOT(deleteCommand()));

	connect(ui.cvCommands, SIGNAL(activated(QModelIndex)), this, SLOT(reflectSelectionStatus(QModelIndex)));
	connect(ui.cvCommands, SIGNAL(clicked(QModelIndex)), this, SLOT(reflectSelectionStatus(QModelIndex)));

	QList<int> colWidths;
	colWidths << 210 << 210 << 320;
	ui.cvCommands->setColumnWidths(colWidths);

	this->loadCommands();


	ui.pbNewCommand->setIcon(KIcon("list-add"));
	ui.pbImportActivities->setIcon(KIcon("document-import"));
	ui.pbEditCommand->setIcon(KIcon("edit-rename"));
	ui.pbDeleteCommand->setIcon(KIcon("edit-delete"));
}


void RunCommandView::reflectSelectionStatus(QModelIndex index)
{
	bool commandSelected = false;
	if (index.internalPointer()) //categories have a 0 pointer
		commandSelected = true;

	ui.pbEditCommand->setEnabled(commandSelected);
	ui.pbDeleteCommand->setEnabled(commandSelected);
}


void RunCommandView::setSettingsVisible()
{
	ui.wgSettings->show();
}

void RunCommandView::setSettingsHidden()
{
	ui.wgSettings->hide();
}


void RunCommandView::addCommand()
{
	NewCommand *newCommand = new NewCommand(this);
	Command *com = newCommand->newCommand();
	if (com)
	{
		ActionManager::getInstance()->addCommand(com);
	}
}




/**
*   \brief Loads and inserts the commandList by using the RunCommand backend
*
*   @author Peter Grasch
*/
void RunCommandView::loadCommands()
{
	CommandModel *model = new CommandModel(ActionManager::getInstance()->getCommandList());
	connect(ActionManager::getInstance(), SIGNAL(commandsChanged(CommandList*)), model, 
			SLOT(updateCommands(CommandList*)));
	ui.cvCommands->setModel(model);
}



Command* RunCommandView::getCommandToModify()
{
	QModelIndex currentIndex = ui.cvCommands->currentIndex();
	Command *command = static_cast<Command*>(currentIndex.internalPointer());
	if (!command)
	{
		KMessageBox::information(this, i18n("Kommando auswählen"), i18n("Bitte wählen Sie zuerst ein Kommando aus der Liste aus"));
		return 0;
	}
	return command;
}

void RunCommandView::editCommand()
{
	Command *command = getCommandToModify();
	if (!command) return;

	NewCommand *editCommand = new NewCommand(this);
	editCommand->init(command);
	Command *newCommand = editCommand->newCommand();
	if (newCommand)
	{
		ActionManager::getInstance()->deleteCommand(command);
		ActionManager::getInstance()->addCommand(newCommand);
	}
}

void RunCommandView::deleteCommand()
{
	Command *command = getCommandToModify();
	if (!command) return;
	
	if (KMessageBox::questionYesNoCancel(this, i18n("Kommando löschen"), i18n("Wollen Sie das ausgewählte Kommando wirklich unwiederruflich löschen?")) == KMessageBox::Yes)
	{
		ActionManager::getInstance()->deleteCommand(command);
	}
}

/**
 *	@brief Destructor
 *
 *	@author Peter Grasch
 */
RunCommandView::~RunCommandView()
{
	//do nothing - RunCommand should be preserved as it is a singleton...
}
