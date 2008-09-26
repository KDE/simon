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

#include "screengrid.h"
#include <QWidget>
#include <QGridLayout>
#include <KPushButton>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QApplication>
#include <QLabel>
#include <QBrush>
#include <QRect>
#include <QColor>
#include "../../../SimonLib/EventSimulation/eventhandler.h"

ScreenGrid::ScreenGrid(QWidget* parent): QWidget(parent, 
		Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint)
{
	background = 0;
	buttons = new QGridLayout(this);
	this->setContentsMargins(0,0,0,0);
	buttons->setSpacing(0);
	QSize desksize = QDesktopWidget().screenGeometry().size();
	this->resize(desksize);
	short btnNr=1;

	QBrush transbrush(QColor(241,241,241,100));
	for (int i=0; i < 3; i++)
	{
		for (int j=0; j<3; j++)
		{
			KPushButton *btn = new KPushButton(QString::number(btnNr), this);

			QPalette pal = btn->palette();
			pal.setBrush(QPalette::Button, transbrush);
			btn->setPalette(pal);

			btn->setMinimumHeight(desksize.height()/3);
			btn->setMinimumWidth(1);
			setButtonFontSize(btn);
			connect(btn, SIGNAL(clicked()), this, SLOT(regionSelected()));
			buttons->addWidget(btn, i, j);
			btnNr++;
		}
	}

	
	if (!false) //Settings::getB("Commands/DesktopGrid/RealTransparency"))
	{
		background = new QLabel(this);
		background->lower();
		deskShot = makeFakeTransparency();
		background->setPixmap(deskShot);
		background->move(0,0);
		background->resize(this->size());
	} else {
		this->setWindowOpacity(0.55);
	}

	buttons->setGeometry(geometry());
	
	
	buttons->setMargin(0);
	this->setLayout(buttons);
}


void ScreenGrid::setButtonFontSize(KPushButton *btn)
{
	QFont f = btn->font();
	f.setPointSize(btn->height()/4-2);
	btn->setFont(f);
}

QPixmap ScreenGrid::makeFakeTransparency()
{
	return QPixmap::grabWindow(QApplication::desktop()->winId()).copy(geometry());
	
	
}

void ScreenGrid::regionSelected()
{
	KPushButton *senderBtn = dynamic_cast<KPushButton*>(sender());
	if (!senderBtn) return;

	if ((senderBtn->width() <= 20) && (senderBtn->height() <= 20))
	{
		int x = this->x()+senderBtn->x()+(senderBtn->width()/2);
		int y = this->y()+senderBtn->y()+(senderBtn->height()/2);
		EventHandler::getInstance()->click(x,y); //click the region
		deleteLater();
		return;
	}
	QSize btnSize = senderBtn->size();
	QPoint pos = senderBtn->pos();
	pos.setX(pos.x()+this->x());
	pos.setY(pos.y()+this->y());

	int btnHeight = senderBtn->height()/3;

	for (int i=0; i < this->children().count(); i++)
	{
		KPushButton *btn = dynamic_cast<KPushButton*>(children().at(i));
		if (btn) {
			setButtonFontSize(btn);
			btn->setMinimumHeight(btnHeight);
			
		}
	}

	this->setMaximumWidth(btnSize.width());
	this->setMaximumHeight(btnSize.height());

	
	buttons->setGeometry(QRect(0, 0, geometry().width(), geometry().height()));
	
	this->move(pos);
	
	repaint();

	
	if (!false) //Settings::getB("Commands/DesktopGrid/RealTransparency"))
	{
		background->resize(size());
		background->move(0,0);
		background->setPixmap(deskShot.copy(geometry()));
	}
}

void ScreenGrid::keyPressEvent(QKeyEvent *event)
{
	if (!event) return;
	if (event->key()== Qt::Key_Escape) this->hide();
}


ScreenGrid::~ScreenGrid()
{
    buttons->deleteLater();
    // #ifdef FAKETRANSPARENCY
	if (background) background->deleteLater();
    // #endif
}


