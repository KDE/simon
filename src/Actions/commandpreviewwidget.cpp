/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "commandpreviewwidget.h"
#include "Commands/command.h"
#include <QMap>
#include <QFormLayout>
#include <QLayout>
#include <QUrl>


CommandPreviewWidget::CommandPreviewWidget(QWidget *parent) : QWidget(parent)
{
	command =0;
	ui.setupUi(this);
	
	connect(ui.pbTrigger, SIGNAL(clicked()), this, SLOT(trigger()));
}

void CommandPreviewWidget::trigger()
{
	if (command) command->trigger();
}

void CommandPreviewWidget::updateCommand(const QModelIndex &commandIdx)
{
	Command *command = static_cast<Command*>(commandIdx.internalPointer());
	if (!command) return;

	ui.lbIcon->setPixmap(command->getIcon().pixmap(64,64));
	ui.lbName->setText(command->getTrigger());

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

	QMap<QString,QVariant> details = command->getValueMap();
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

	this->command = command;
}




