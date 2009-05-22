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
#include <KMessageBox>
#include <KIcon>
#include <QWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QSize>
#include <simonactions/actionmanager.h>
#include <simonactions/commandmodel.h>
#include "newcommand.h"
#include "commandpreviewwidget.h"


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

	connect(ActionManager::getInstance(), SIGNAL(categoriesChanged(const QList<KIcon>&, const QStringList&)),
			this, SLOT(categoriesChanged(const QList<KIcon>&, const QStringList&)));
	connect(ActionManager::getInstance(), SIGNAL(commandAdded(Command*)), this, SLOT(commandAdded(Command*)));
	connect(ActionManager::getInstance(), SIGNAL(commandRemoved(const QString&, const QString&)), 
			this, SLOT(commandRemoved(const QString&, const QString&)));

	connect(ui.lwCommands, SIGNAL(currentRowChanged(int)), this, SLOT(updateCommandDetail()));
	connect(ui.lwCategories, SIGNAL(currentRowChanged(int)), this, SLOT(fetchCommandsFromCategory()));
	connect(ui.pbTrigger, SIGNAL(clicked()), this, SLOT(triggerCommand()));

	ActionManager::getInstance()->publishCategories();

	ui.lwCategories->setIconSize(QSize(24,24));
	ui.lwCommands->setIconSize(QSize(24,24));

	ui.pbNewCommand->setIcon(KIcon("list-add"));
	ui.pbImportActivities->setIcon(KIcon("document-import"));
	ui.pbEditCommand->setIcon(KIcon("edit-rename"));
	ui.pbDeleteCommand->setIcon(KIcon("edit-delete"));
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
	newCommand->registerCreators(ActionManager::getInstance()->getCreateCommandWidgets(NULL/*newCommand*/));
	
	QListWidgetItem *catItem = ui.lwCategories->item(ui.lwCategories->currentRow());
	Command *com=NULL;
	if (catItem)
		com = newCommand->newCommand(catItem->text());
	else 
		com = newCommand->newCommand();
	if (com)
	{
		ActionManager::getInstance()->addCommand(com);
		updateCommandDetail();
	}
	
	delete newCommand;
}


Command* RunCommandViewPrivate::getCurrentCommand()
{
	QListWidgetItem *catItem = ui.lwCategories->item(ui.lwCategories->currentRow());
	QListWidgetItem *comItem = ui.lwCommands->item(ui.lwCommands->currentRow());
	if (!catItem || !comItem)
		return NULL;
	return ActionManager::getInstance()->getCommand(catItem->text(),comItem->text());
	
}

void RunCommandViewPrivate::fetchCommandsFromCategory()
{
	QListWidgetItem *catItem = ui.lwCategories->item(ui.lwCategories->currentRow());
	if (!catItem) return;

	CommandList* commands = ActionManager::getInstance()->getCommandsOfCategory(catItem->text());
	if (!commands) return;

	ui.frmCommandInfo->hide();
	ui.lwCommands->clear();
	foreach (Command* com, *commands)
	{
		ui.lwCommands->addItem(new QListWidgetItem(com->getIcon(), com->getTrigger()));
	}
}

void RunCommandViewPrivate::updateCommandDetail()
{
	Command *com = getCurrentCommand();
	if (!com)
	{
		ui.frmCommandInfo->hide();
		ui.pbEditCommand->setEnabled(false);
		ui.pbDeleteCommand->setEnabled(false);
	}
	else 
	{
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

void RunCommandViewPrivate::categoriesChanged(const QList<KIcon>& icons, const QStringList& names)
{
	Q_ASSERT(icons.count() == names.count());
	ui.lwCategories->clear();
	int i=0;
	foreach (const QString& name, names)
	{
		ui.lwCategories->addItem(new QListWidgetItem(icons[i], name));
		i++;
	}
	if (names.count() == 0)
		ui.lwCommands->clear();
	else ui.lwCategories->setCurrentRow(0);
}

void RunCommandViewPrivate::commandAdded(Command* com)
{
	if ((!ui.lwCategories->item(ui.lwCategories->currentRow())) ||
		(com->getCategoryText() != ui.lwCategories->item(ui.lwCategories->currentRow())->text()))
	{
		if (ui.lwCategories->findItems(com->getCategoryText(), Qt::MatchExactly).isEmpty())
			ui.lwCategories->addItem(new QListWidgetItem(com->getCategoryIcon(), com->getCategoryText()));

		return;
	}

	ui.lwCommands->addItem(new QListWidgetItem(com->getIcon(), com->getTrigger()));
}

void RunCommandViewPrivate::commandRemoved(const QString& trigger, const QString& category)
{
	if ((!ui.lwCategories->item(ui.lwCategories->currentRow())) ||
		(category != ui.lwCategories->item(ui.lwCategories->currentRow())->text()))
		return;

	QList<QListWidgetItem*> items = ui.lwCommands->findItems(trigger, Qt::MatchExactly);
	foreach (QListWidgetItem* item, items)
		delete item;
}


void RunCommandViewPrivate::editCommand()
{
	Command *command = getCurrentCommand();
	if (!command) return;

	NewCommand *editCommand = new NewCommand(this);
	editCommand->registerCreators(ActionManager::getInstance()->getCreateCommandWidgets(NULL/*editCommand*/));
	editCommand->init(command);
	Command *newCommand = editCommand->newCommand();
	if (newCommand)
	{
		ActionManager::getInstance()->deleteCommand(command);
		ActionManager::getInstance()->addCommand(newCommand);
	}
}

void RunCommandViewPrivate::deleteCommand()
{
	Command *command = getCurrentCommand();
	if (!command) return;
	
	if (KMessageBox::questionYesNoCancel(this, i18n("Are you sure that you want to irreversibly remove that command?"), i18n("Remove Command")) == KMessageBox::Yes)
	{
		ActionManager::getInstance()->deleteCommand(command);
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
	//do nothing - RunCommand should be preserved as it is a singleton...
}
