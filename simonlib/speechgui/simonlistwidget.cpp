/*
 *   Copyright (C) 2008 Phillip Goriup <goriup@simon-listens.org>
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

#include "simonlistwidget.h"
#include <KMessageBox>
#include <KLineEdit>
#include <QScrollBar>
#include <QPalette>
#include <QCoreApplication>
#include <QKeyEvent>




SimonListWidget::SimonListWidget( QWidget * parent ) : KListWidget(parent)
{
	line = new KLineEdit(this);
	line->hide();
//	this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
//	this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff);
	this->redFlag = true;
	connect(line,SIGNAL(textChanged(QString)),this,SLOT(filterEntries(QString)));
	//connect(line,SIGNAL(returnPressed () ),this,SLOT(selectItem()));
}

SimonListWidget::~SimonListWidget()
{
	delete line;
}

void SimonListWidget::showLineEdit()
{
	moveLineEdit();
	this->line->clear();
	line->show();
	line->setFocus () ;
}

void SimonListWidget::moveLineEdit()
{
	line->move(this->width() - line->width()-10,this->height() - line->height()-10);
}


void SimonListWidget::resizeEvent ( QResizeEvent * event )
{
	Q_UNUSED(event);
	this->moveLineEdit();
}


void SimonListWidget::selectItem()
{
	int i;
	
	if((this->redFlag )|| (this->line->text().isEmpty()))
		return;

	for (i = 0; i<this->count(); i++)
	{
		if (!this->item(i)->isHidden())
		{	
			this->setCurrentItem(this->item(i));
			scrollToItem (this->item(i), QAbstractItemView::PositionAtCenter);
			showAllEntries();
			this->line->hide();
			this->setFocus();
			return;
		}
	}
}

void SimonListWidget::filterEntries(QString text)
{
	int i = 0;
	this->redFlag = true;
	
	if (text.isEmpty())
	{
		this->redFlag = false;	
		controlRedFlag();
		showAllEntries();
		return;
	}
	for (i = 0; i<this->count(); i++)
	{
		if(this->item(i)->text().startsWith(text, Qt::CaseInsensitive))
		{
			this->item(i)->setHidden(false);
			//return;
			this->redFlag = false;
		}
		else
		{
			this->item(i)->setHidden (true); 
		}
	}
	controlRedFlag();
}

void SimonListWidget::controlRedFlag()
{
	if (this->redFlag)
	{
		QColor color(224,67,67);
		QBrush brush(color, Qt::SolidPattern);
		QPalette palette = this->line->palette();
		palette.setBrush(QPalette::Base,brush);
		this->line->setPalette(palette);
	}
	else
	{
		QColor color(255,255,255);
		QBrush brush(color, Qt::SolidPattern);
		QPalette palette = this->line->palette();
		palette.setBrush(QPalette::Base,brush);
		this->line->setPalette(palette);	
	}
}

void SimonListWidget::showAllEntries()
{
	int i = 0;
	for (i = 0; i<this->count(); i++)
	{
		this->item(i)->setHidden(false);
	}	
}


void SimonListWidget::keyPressEvent ( QKeyEvent * event )
{
	
	if (event->key()==Qt::Key_Return && !this->line->isHidden())        //i have no notion, why i have to add 1 to the e->key(), that it gaves the same value as Qt::Key_Enter
	{
		this->selectItem();
		return;
	}
	

// 	char c;
// 	QByteArray array = event->text().toLatin1();
// 	c = *array.data();
// 	if(!((c<=126) && (c>=33)))
// 	{	
// 		return;
// 	}	
// 	if(c == '\n')
// 	{
// 		return;
// 	}
	QString eventText = event->text();
	
	if (eventText.trimmed().isEmpty())
		return;
	
	if(this->line->isHidden())
		showLineEdit();
	
	this->line->setText(eventText);
}

