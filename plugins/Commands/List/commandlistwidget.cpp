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

#include "commandlistwidget.h"
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QObject>
#include <QTableWidgetSelectionRange>
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>
#include <QVariant>
#include <KIcon>
#include <KLocalizedString>


CommandListWidget::CommandListWidget() : QWidget(0, Qt::Dialog|Qt::WindowStaysOnTopHint)
{
	ui.setupUi(this);

	ui.pbCancel->setIcon(KIcon("dialog-cancel"));
	ui.twCommands->verticalHeader()->hide();
	connect(ui.twCommands, SIGNAL(itemActivated(QTableWidgetItem*)), this, SLOT(runCommand()));
	connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(close()));
}

void CommandListWidget::runCommand()
{
	int row = ui.twCommands->currentRow();
	if (row == -1)
		return;

	//even if we don't have a back-button the commands
	//start at 0
	if ((!(currentFlags & HasBack)))
		row++;

	emit runRequest(row);
}

void CommandListWidget::closeEvent(QCloseEvent *)
{
	emit canceled();
}

void CommandListWidget::init(const QStringList& iconsrcs, const QStringList commands, Flags flags)
{
	if (commands.count() == 0) return;

	Q_ASSERT(commands.count() == iconsrcs.count());

	currentFlags = flags;
	int rowCount = commands.count();
	int rowInsertionModifier=0;
	if (flags & HasNext)
		rowCount++;
	if (flags & HasBack)
	{
		rowCount++;
		rowInsertionModifier=1;
	}
	//showing list
	ui.twCommands->setRowCount(rowCount);

	if (flags & HasBack)
	{
		QTableWidgetItem *num = new QTableWidgetItem(QString::number(0));
		num->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		QTableWidgetItem *com = new QTableWidgetItem(KIcon("go-previous"), i18n("Back"));
		com->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		ui.twCommands->setItem(0, 0, num);
		ui.twCommands->setItem(0, 1, com);

	}

	int i;
	for (i=rowInsertionModifier; i-rowInsertionModifier<commands.count();i++)
	{
		QString command = commands[i-rowInsertionModifier];
		QString iconsrc = iconsrcs[i-rowInsertionModifier];

		QTableWidgetItem *num = new QTableWidgetItem(QString::number(i-rowInsertionModifier+1));
		num->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		QTableWidgetItem *com = new QTableWidgetItem(KIcon(iconsrc), command);
		com->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		ui.twCommands->setItem(i, 0, num);
		ui.twCommands->setItem(i, 1, com);
	}
	if (flags & HasNext)
	{
		QTableWidgetItem *num = new QTableWidgetItem(QString::number(i-rowInsertionModifier+1));
		num->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		QTableWidgetItem *com = new QTableWidgetItem(KIcon("go-next"), i18n("Next"));
		com->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		ui.twCommands->setItem(i, 0, num);
		ui.twCommands->setItem(i, 1, com);

	}
	ui.twCommands->resizeColumnsToContents();

	QHeaderView *vhview = ui.twCommands->verticalHeader();
	QHeaderView *hhview = ui.twCommands->horizontalHeader();
	resize(QSize(hhview->sectionSize(0)+hhview->sectionSize(1)+25, 
				(rowCount*vhview->sectionSize(0))+ui.pbCancel->height()+40));

	//move to center of screen
	QDesktopWidget* tmp = QApplication::desktop();
	int x,y;
	x=(tmp->width()/2) - (width()/2);
	y=(tmp->height()/2)-(height()/2);
	tmp->deleteLater();
	
	move(x,y);
}

CommandListWidget::~CommandListWidget()
{
}

