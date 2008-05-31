//
// C++ Implementation: runapplicationview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "runcommandview.h"
#include <QMessageBox>
#include <QIcon>
#include <QWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include <QHeaderView>
#include <QSize>
#include "../SimonLib/Settings/settings.h"
#include "../SimonLib/SimonInfo/simoninfo.h"
#include "actionmanager.h"
#include "newcommand.h"
#include "Commands/commandmodel.h"
#include "commandpreviewwidget.h"

#ifdef __WIN32
#include "../SimonLib/WindowsLib/windowsresourcehandler.h"
#endif


/**
 *	@brief Constructor
 *
 *	@author Peter Grasch
 */
RunCommandView::RunCommandView(QWidget *parent) : InlineWidget(tr("Kommandos"), QIcon(":/images/icons/system-run.svg"), tr("Direkte ausführung von simon-Befehlen"), parent)
{
	ui.setupUi(this);

	CommandPreviewWidget *commandPreviewWidget = new CommandPreviewWidget(ui.cvCommands);
	connect(ui.cvCommands, SIGNAL(updatePreviewWidget(QModelIndex)), commandPreviewWidget, SLOT(updateCommand(QModelIndex)));
	ui.cvCommands->setPreviewWidget(commandPreviewWidget);

	guessChildTriggers((QObject*)this);

	setSettingsHidden();
	hide();

	connect ( ui.leSearch, SIGNAL(textChanged(QString)), this, SLOT(filterByPattern(QString)) );
	connect ( ui.pbClearSearch, SIGNAL(clicked()), ui.leSearch, SLOT(clear()) );
	connect ( ui.pbNewCommand, SIGNAL(clicked()), this, SLOT(addCommand()));
	connect ( ui.pbEditCommand, SIGNAL(clicked()), this, SLOT(editCommand()));
	connect ( ui.pbDeleteCommand, SIGNAL(clicked()), this, SLOT(deleteCommand()));

	connect(ui.cvCommands, SIGNAL(activated(QModelIndex)), this, SLOT(reflectSelectionStatus(QModelIndex)));
	connect(ui.cvCommands, SIGNAL(clicked(QModelIndex)), this, SLOT(reflectSelectionStatus(QModelIndex)));

	registerControl(tr("Leere Suche"), ui.pbClearSearch, SLOT(animateClick()));
	
	QList<int> colWidths;
	colWidths << 230 << 230 << 340;
	ui.cvCommands->setColumnWidths(colWidths);

	this->loadCommands();
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
 *	@brief Filters the List of commands (search) by the given string
 *	
 *	@author Peter Grasch
 *	@param QString filter
 *	The filter (case insensetive)
 */
void RunCommandView::filterByPattern(QString filter)
{
// 	CommandList commands;
// 	if (!commlist)
// 		commands = run->getCommands();
// 	else commands = *commlist;
// 	
// 	for (int i = 0; i < commands.size(); i++)
// 	{
// 		if (commands.at(i)->getName().toUpper().indexOf( filter.toUpper(), Qt::CaseInsensitive ) == -1) {
// 			commands.removeAt(i);
// 			--i;
// 		}
// 	}
// 	commands =  filterByCategory(getCategory( ui.cbShow->currentText() ), &commands);
// 	insertCommands( commands );
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
		QMessageBox::information(this, tr("Kommando auswählen"), tr("Bitte wählen Sie zuerst ein Kommando aus der Liste aus"));
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
	
	if (QMessageBox::question(this, tr("Kommando löschen"), tr("Wollen Sie das ausgewählte Kommando wirklich unwiederruflich löschen?"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
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
