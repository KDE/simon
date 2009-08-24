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
#include <KPushButton>
#include <QVBoxLayout>
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
	QVBoxLayout *lay = new QVBoxLayout(this);

	lay->setMargin(0);
	lay->setSpacing(0);

	pbCancel = new KPushButton(this);
	pbCancel->setText(i18n("Cancel"));
	pbCancel->setIcon(KIcon("dialog-cancel"));
	twCommands = new QTableWidget(this);
	twCommands->verticalHeader()->hide();
	if (twCommands->columnCount() < 2)
		twCommands->setColumnCount(2);
	QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
	twCommands->setHorizontalHeaderItem(0, __qtablewidgetitem);
	QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
	twCommands->setHorizontalHeaderItem(1, __qtablewidgetitem1);
	twCommands->setObjectName(QString::fromUtf8("twCommands"));
	twCommands->setSelectionMode(QAbstractItemView::SingleSelection);
	twCommands->setSelectionBehavior(QAbstractItemView::SelectRows);

	QTableWidgetItem *___qtablewidgetitem = twCommands->horizontalHeaderItem(0);
	___qtablewidgetitem->setText(tr2i18n("Number", 0));
	QTableWidgetItem *___qtablewidgetitem1 = twCommands->horizontalHeaderItem(1);
	___qtablewidgetitem1->setText(tr2i18n("Command", 0));

	lay->addWidget(twCommands);
	lay->addWidget(pbCancel);
	setLayout(lay);

	connect(twCommands, SIGNAL(itemActivated(QTableWidgetItem*)), this, SLOT(runCommand()));
	connect(pbCancel, SIGNAL(clicked()), this, SLOT(close()));
	runRequestEmitted = false;
}

void CommandListWidget::runRequestSent()
{
	runRequestEmitted = true;
}

void CommandListWidget::runCommand()
{
	int row = twCommands->currentRow();
	if (row == -1)
		return;

	//even if we don't have a back-button the commands
	//start at 0
	if ((!(currentFlags & HasBack)))
		row++;

	runRequestEmitted = true;
	emit runRequest(row);
}

void CommandListWidget::closeEvent(QCloseEvent *)
{
	if (!runRequestEmitted) {
		fprintf(stderr, "Emitting cancel...\n");
		emit canceled();
	}
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
	twCommands->setRowCount(rowCount);

	if (flags & HasBack)
	{
		QTableWidgetItem *num = new QTableWidgetItem(QString::number(0));
		num->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		QTableWidgetItem *com = new QTableWidgetItem(KIcon("go-previous"), i18n("Back"));
		com->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		twCommands->setItem(0, 0, num);
		twCommands->setItem(0, 1, com);

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
		twCommands->setItem(i, 0, num);
		twCommands->setItem(i, 1, com);
	}
	if (flags & HasNext)
	{
		QTableWidgetItem *num = new QTableWidgetItem(QString::number(i-rowInsertionModifier+1));
		num->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		QTableWidgetItem *com = new QTableWidgetItem(KIcon("go-next"), i18n("Next"));
		com->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		twCommands->setItem(i, 0, num);
		twCommands->setItem(i, 1, com);

	}
	twCommands->resizeColumnsToContents();

	QHeaderView *vhview = twCommands->verticalHeader();
	QHeaderView *hhview = twCommands->horizontalHeader();
	resize(QSize(hhview->sectionSize(0)+hhview->sectionSize(1)+25, 
				(rowCount*vhview->sectionSize(0))+pbCancel->height()+40));

	//move to center of screen
	QDesktopWidget* tmp = QApplication::desktop();
	int x,y;
	x=(tmp->width()/2) - (width()/2);
	y=(tmp->height()/2)-(height()/2);
	
	move(x,y);

	runRequestEmitted = false;
}

CommandListWidget::~CommandListWidget()
{
}

