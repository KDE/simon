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

#include "runcommandviewprivate.h"
#include <simonscenarios/action.h>
#include <simonscenarios/commandmanager.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/actioncollection.h>
#include "newcommand.h"
#include "commandpreviewwidget.h"

#include <QWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QSize>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>

#include <KMessageBox>
#include <KIcon>
#include <KCMultiDialog>


/**
 *	@brief Constructor
 *
 *	@author Peter Grasch
 */
RunCommandViewPrivate::RunCommandViewPrivate(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	ui.frmCommandInfo->hide();

	connect ( ui.pbNewCommand, SIGNAL(clicked()), this, SLOT(addCommand()));
	connect ( ui.pbEditCommand, SIGNAL(clicked()), this, SLOT(editCommand()));
	connect ( ui.pbDeleteCommand, SIGNAL(clicked()), this, SLOT(deleteCommand()));
	connect ( ui.pbManagePlugins, SIGNAL(clicked()), this, SLOT(managePlugIns()));

	connect(ui.pbTrigger, SIGNAL(clicked()), this, SLOT(triggerCommand()));

	ui.lvActions->setIconSize(QSize(24,24));
	ui.lvCommands->setIconSize(QSize(24,24));

	ui.pbNewCommand->setIcon(KIcon("list-add"));
	ui.pbEditCommand->setIcon(KIcon("edit-rename"));
	ui.pbDeleteCommand->setIcon(KIcon("edit-delete"));

	commandsProxy = new QSortFilterProxyModel(this);
	commandsProxy->setFilterKeyColumn(0);
	commandsProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
	ui.lvCommands->setModel(commandsProxy);

	actionsProxy = new QSortFilterProxyModel(this);
	actionsProxy->setFilterKeyColumn(0);
	actionsProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
	ui.lvActions->setModel(actionsProxy);

	connect(ui.leCommandsFilter, SIGNAL(textChanged(const QString&)), commandsProxy, SLOT(setFilterRegExp(const QString&)));
	connect(ui.leActionsFilter, SIGNAL(textChanged(const QString&)), actionsProxy, SLOT(setFilterRegExp(const QString&)));

	connect(ui.lvActions->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(fetchCommandsFromCategory()));
	connect(ui.lvCommands->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(updateCommandDetail()));
}

void RunCommandViewPrivate::managePlugIns()
{
	KCMultiDialog *dlg = (new KCMultiDialog(this));

	KPageWidgetItem *commandSettingsItem = dlg->addModule("simonactionsconfig"/*, QStringList() << ""*/);
/*	if (commandSettingsItem) {
		KCModuleProxy *proxy = static_cast<KCModuleProxy*>(commandSettingsItem->widget());
		KCModule *module = proxy->realModule();
	}*/

	if (dlg->exec()) {
		;
	}
	dlg->deleteLater();
}

void RunCommandViewPrivate::displayScenarioPrivate(Scenario *scenario)
{
	ui.leActionsFilter->clear();

	ActionCollection *actionCollection = scenario->actionCollection();
	commandsProxy->setSourceModel(NULL);
	actionsProxy->setSourceModel(actionCollection);
	ui.lvActions->setCurrentIndex(actionsProxy->index(0,0));
//	fetchCommandsFromCategory();
}



void RunCommandViewPrivate::triggerCommand()
{
	Command *com = getCurrentCommand();
	if (com)
		com->trigger();
}


void RunCommandViewPrivate::addCommand()
{
	NewCommand *newCommand = new NewCommand(this);

	newCommand->registerCreators(scenario->actionCollection()->getCreateCommandWidgets(NULL/*newCommand*/));
	
	Command *com=NULL;

	Action *a = getCurrentlySelectedAction();

	if (a && a->manager())
		com = newCommand->newCommand(a->manager()->name());
	else 
		com = newCommand->newCommand();

	if (com) {
		if (!scenario->addCommand(com))
			KMessageBox::error(0, i18n("Couldn't add Command \"%1\".", com->getTrigger()));

		ui.lvCommands->setCurrentIndex(commandsProxy->index(commandsProxy->rowCount()-1, 0));
	}
	
	delete newCommand;
}


Action* RunCommandViewPrivate::getCurrentlySelectedAction()
{
	QModelIndex index = actionsProxy->mapToSource(ui.lvActions->currentIndex());
	if (!index.isValid()) return NULL;

	return static_cast<Action*>(index.internalPointer());
}

void RunCommandViewPrivate::fetchCommandsFromCategory()
{
	Action *a = getCurrentlySelectedAction();
	if (!a) return;

	CommandManager *cm = a->manager();
	if (!cm) return;

	ui.leCommandsFilter->clear();

	commandsProxy->setSourceModel(cm);
	if (cm->hasCommands()) {
		ui.lvCommands->setCurrentIndex(commandsProxy->index(0,0));
	}
}

Command* RunCommandViewPrivate::getCurrentCommand()
{
	QModelIndex index = commandsProxy->mapToSource(ui.lvCommands->currentIndex());
	if (!index.isValid()) return NULL;

	return static_cast<Command*>(index.internalPointer());
}

void RunCommandViewPrivate::updateCommandDetail()
{
	Command *com = getCurrentCommand();
	if (!com)
	{
		ui.frmCommandInfo->hide();
		ui.pbEditCommand->setEnabled(false);
		ui.pbDeleteCommand->setEnabled(false);
		ui.lbCompleteTrigger->clear();
	}
	else 
	{
		Action* a = getCurrentlySelectedAction();
		if (!a) return;
		ui.lbCompleteTrigger->setText("\""+QString(a->trigger()+" "+com->getTrigger()).trimmed()+"\"");

		ui.frmCommandInfo->show();
		ui.pbEditCommand->setEnabled(true);
		ui.pbDeleteCommand->setEnabled(true);
		

		ui.lbName->setText(com->getTrigger());
		ui.lbIcon->setPixmap(KIcon(com->getIcon()).pixmap(64,64));

		QLayoutItem *child;
		while (ui.flDetails->count() > 0)
		{
			child = ui.flDetails->takeAt(0);
			ui.flDetails->removeItem(child);

			QWidget *widget = child->widget();
			if (widget) widget->deleteLater();
			if (child)
				delete child;
		}

		QMap<QString,QVariant> details = com->getValueMap();
		QStringList keys = details.keys();

		for (int i=0; i < keys.count(); i++)
		{
			QLabel *label = new QLabel(this);
			label->setOpenExternalLinks(true);
			label->setWordWrap(true);
			QVariant value = details.value(keys[i]);
			
			QString strValue;
			if (value.type() == QVariant::Url)
				strValue = QString("<a href=\"%1\">%1</a>").arg(value.toUrl().toString());
			else strValue = value.toString();

			label->setText(strValue);
			
			ui.flDetails->addRow(keys[i]+":", label);
		}
	}

}




void RunCommandViewPrivate::editCommand()
{
	Command *command = getCurrentCommand();
	if (!command) return;

	NewCommand *editCommand = new NewCommand(this);
	editCommand->registerCreators(scenario->actionCollection()->getCreateCommandWidgets(NULL/*editCommand*/));
	editCommand->init(command);
	Command *newCommand = editCommand->newCommand();
	if (newCommand)
	{
		ScenarioManager::getInstance()->getCurrentScenario()->removeCommand(command);
		ScenarioManager::getInstance()->getCurrentScenario()->addCommand(newCommand);
		ui.lvCommands->setCurrentIndex(commandsProxy->index(commandsProxy->rowCount()-1, 0));
	}
}

void RunCommandViewPrivate::deleteCommand()
{
	Command *command = getCurrentCommand();
	if (!command) return;
	
	if (KMessageBox::questionYesNoCancel(this, i18n("Are you sure that you want to irreversibly remove the command \"%1\"?", command->getTrigger()), i18n("Remove Command")) == KMessageBox::Yes)
	{
		ScenarioManager::getInstance()->getCurrentScenario()->removeCommand(command);
		updateCommandDetail();
	}
}

/**
 *	@brief Destructor
 *
 *	@author Peter Grasch
 */
RunCommandViewPrivate::~RunCommandViewPrivate()
{
}
