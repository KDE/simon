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

#include "listcommand.h"
#include <simonactions/actionmanager.h>
#include <unistd.h>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QObject>
#include <QDesktopWidget>
#include <QTableWidgetSelectionRange>
#include <QHeaderView>
#include <QApplication>
#include <QVariant>
#include <KIcon>
#include <KLocalizedString>

QStringList ListCommand::numberIdentifiers;

ListCommand::ListCommand(const QString& name, const QString& iconSrc, const QStringList& commands, 
		const QStringList& iconSrcs, const QStringList& commandTypes) : Command(name, iconSrc)
{
	KLocale::setMainCatalog("simonlib");
	this->iconsrcs = iconSrcs;
	this->commands = commands;
	this->commandTypes = commandTypes;
	
	w = new QTableWidget(0);
	w->setColumnCount(2);
	w->verticalHeader()->hide();
	w->setWindowFlags(Qt::Dialog|Qt::WindowStaysOnTopHint);
	connect(w, SIGNAL(itemActivated(QTableWidgetItem*)), this, SLOT(runCommand()));

	if (numberIdentifiers.isEmpty())
		numberIdentifiers << i18n("Zero") << i18n("One") << i18n("Two") 
			<< i18n("Three") << i18n("Four") << i18n("Five") <<
			i18n("Six") << i18n("Seven") << i18n("Eight") << i18n("Nine");
}

void ListCommand::runCommand()
{
	int row = w->currentRow();
	if (row == -1)
		return;

	Q_ASSERT(commands.count() == commandTypes.count());

	if (row > commands.count())
		return;

	ActionManager::getInstance()->triggerCommand(commandTypes[row], commands[row]);
	w->close();
	ActionManager::getInstance()->deRegisterPrompt(this, "executeSelection");
}

bool ListCommand::executeSelection(QString inputText)
{
	kWarning() << "ListCommand received input" << inputText;

	//setting correct index
	int index = 0;
	while ((index < numberIdentifiers.count()) && (numberIdentifiers.at(index).toUpper() != inputText.toUpper()))
		index++;

	kWarning() << numberIdentifiers << inputText;
	if (index == numberIdentifiers.count()) return false;
	if (index == 0)
	{
		//go back
	} else if (index == 9)
	{
		//go forward

	} else {
		//execute list entry
		// if index==1, we want it to represent the _first_ entry in the list (index==0)
		index--;

		Q_ASSERT(commands.count() == commandTypes.count());
		if (index >= commands.count())
			return false;

		w->setRangeSelected(QTableWidgetSelectionRange(index, 0, index, 1), true);



		usleep(300000);
		w->close();
		usleep(150000);
		ActionManager::getInstance()->triggerCommand(commandTypes[index], commands[index]);
		ActionManager::getInstance()->deRegisterPrompt(this, "executeSelection");
	}

	return true;
}

const QString ListCommand::staticCategoryText()
{
	return i18n("List");
}

const QString ListCommand::getCategoryText() const
{
	return ListCommand::staticCategoryText();
}

const KIcon ListCommand::staticCategoryIcon()
{
	return KIcon("view-choose");
}

const KIcon ListCommand::getCategoryIcon() const
{
	return ListCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> ListCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;
	out.insert(i18n("Commands"), commands.join("\n"));
	return out;
}

bool ListCommand::triggerPrivate()
{
	if (commands.count() == 0) return false;

	Q_ASSERT(commands.count() == commandTypes.count());
	//showing list

	w->setWindowIcon(getIcon());
	w->setWindowTitle(getTrigger());
	w->clear();
	w->setHorizontalHeaderItem(0, new QTableWidgetItem());
	w->horizontalHeaderItem(0)->setText(i18n("Number"));
	w->setHorizontalHeaderItem(1, new QTableWidgetItem());
	w->horizontalHeaderItem(1)->setText(i18n("Command"));
	w->setSelectionMode(QAbstractItemView::SingleSelection);
	w->setSelectionBehavior(QAbstractItemView::SelectRows);

	w->setRowCount(commands.count());

	for (int i=0; i<commands.count();i++)
	{
		QString type = commandTypes[i];
		QString command = commands[i];
		QString iconsrc = iconsrcs[i];

		QTableWidgetItem *num = new QTableWidgetItem(QString::number(i+1));
		num->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		QTableWidgetItem *com = new QTableWidgetItem(KIcon(iconsrc), command);
		com->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		w->setItem(i, 0, num);
		w->setItem(i, 1, com);
	}
	w->resizeColumnsToContents();

	QHeaderView *vhview = w->verticalHeader();
	QHeaderView *hhview = w->horizontalHeader();
	w->resize(QSize(hhview->sectionSize(0)+hhview->sectionSize(1)+20, 
				(commands.count()*vhview->sectionSize(0))+35));

	//move to center of screen
	QDesktopWidget* tmp = QApplication::desktop();
	int x,y;
	x=(tmp->width()/2) - (w->width()/2);
	y=(tmp->height()/2)-(w->height()/2);
	
	w->move(x,y);

	w->show();
	ActionManager::getInstance()->registerPrompt(this, "executeSelection");

	return true;
}

ListCommand::~ListCommand()
{
	ActionManager::getInstance()->deRegisterPrompt(this, "executeSelection");
	w->deleteLater();
}

