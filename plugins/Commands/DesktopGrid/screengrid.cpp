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
#include "desktopgridconfiguration.h"
#include <eventsimulation/eventhandler.h>
#include <simonactions/actionmanager.h>
#include <simonactions/commandlistwidget.h>
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

QStringList ScreenGrid::numberIdentifiers;


ScreenGrid::ScreenGrid(DesktopGridConfiguration *_config, QWidget* parent): 
	QWidget(parent, Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint), 
	m_x(0),
	m_y(0),
	m_startX(0),
	m_startY(0),
	m_isDragging(false),
	config(_config),
	buttons(new QGridLayout(this)),
	commandListWidget(new CommandListWidget()),
	background(0)
{
	this->setContentsMargins(0,0,0,0);
	buttons->setSpacing(0);

	if (numberIdentifiers.isEmpty())
		numberIdentifiers << i18n("One") << i18n("Two") 
			<< i18n("Three") << i18n("Four") << i18n("Five") <<
			i18n("Six") << i18n("Seven") << i18n("Eight") << i18n("Nine");

	QSize desksize = QDesktopWidget().screenGeometry().size();
	QBrush transbrush(QColor(241,241,241,100));
	short btnNr=1;
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
			btns << btn;
		}
	}

	buttons->setGeometry(geometry());
	
	buttons->setMargin(0);
	this->setLayout(buttons);
	
	startGreedy();

	commandListWidget->init(QStringList() << "input-mouse" << "input-mouse" << 
			"input-mouse" << "input-mouse" << "input-mouse", 
			QStringList() << i18n("Left click") << 
			i18n("Double click") << 
			i18n("Right click") << 
			i18n("Middle click") <<
			i18n("Drag & Drop"), 0); //Add Elements for the list

	connect(commandListWidget, SIGNAL(runRequest(int)), this, SLOT(clickRequestReceived(int)));
	connect(commandListWidget, SIGNAL(canceled()), this, SLOT(deleteLater()));

	init();
}


void ScreenGrid::init()
{
	this->move(0,0);
	QSize deskSize = QDesktopWidget().screenGeometry().size();
	this->raise();

	kDebug() << size() << pos();

	buttons->setGeometry(geometry());

	if (!static_cast<DesktopGridConfiguration*>(config)->useRealTransparency())
	{
		if (background) background->deleteLater();

		background = new QLabel(this);
		background->lower();
		deskShot = makeFakeTransparency();
		background->setPixmap(deskShot);
		background->move(0,0);
		background->resize(this->size());
	} else
		this->setWindowOpacity(0.55);
	
	for (int i=0; i < this->children().count(); i++)
	{
		KPushButton *btn = dynamic_cast<KPushButton*>(children().at(i));
		if (btn) {
			setButtonFontSize(btn);
			btn->setMinimumHeight(deskSize.height()/3);
			btn->setMinimumWidth(1);
			setButtonFontSize(btn);
		}
	}

	setMaximumWidth(deskSize.width());
	setMaximumHeight(deskSize.height());
	setMinimumWidth(deskSize.width());
	setMinimumHeight(deskSize.height());

	this->resize(deskSize);
	
	buttons->setGeometry(QRect(0, 0, geometry().width(), geometry().height()));
	

	show();
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


void ScreenGrid::sendDragAndDrop()
{
	EventHandler::getInstance()->dragAndDrop(m_startX, m_startY, m_x,m_y);
}

void ScreenGrid::click(KPushButton* btn)
{
	m_x = this->x()+btn->x()+(btn->width()/2);
	m_y = this->y()+btn->y()+(btn->height()/2);
	hide();

	if (m_isDragging) {
//		sendClick(EventSimulation::LMBDown);
//		sendClick(EventSimulation::LMBUp);
		sendDragAndDrop();
		deleteLater();
		return;
	}

	DesktopGridConfiguration::ActionSelection modeSelection = static_cast<DesktopGridConfiguration*>(config)->actionSelection();
	EventSimulation::ClickMode mode = static_cast<DesktopGridConfiguration*>(config)->clickMode();
	kDebug() << modeSelection << mode;

	switch (modeSelection) {
		case DesktopGridConfiguration::AlwaysAsk:
			commandListWidget->show();
			break;
		case DesktopGridConfiguration::UseDefault:
			clickRequestReceived((int) mode);
			break;
		case DesktopGridConfiguration::AskButDefaultAfterTimeout:
			commandListWidget->show();
			kDebug() << "Timeout: " << static_cast<DesktopGridConfiguration*>(config)->askTimeout();
			commandListWidget->selectAfterTimeout((int) mode, static_cast<DesktopGridConfiguration*>(config)->askTimeout());
			break;
	}
}

void ScreenGrid::clickRequestReceived(int index)
{
	kDebug() << "CommandListWidget was hidden...";
	commandListWidget->hide();
	commandListWidget->abortTimeoutSelection();

	switch (index)
	{
		case 1: sendClick(EventSimulation::LMB);
			break;
		case 2: sendClick(EventSimulation::LMBDouble);
			break;
		case 3: sendClick(EventSimulation::RMB);
			break;
		case 4: sendClick(EventSimulation::MMB);
			break;
		case 5: 
			m_isDragging = true;
			m_startX = m_x;
			m_startY = m_y;
			init();
			return; //don't delete me
	}

	deleteLater();
}

void ScreenGrid::sendClick(EventSimulation::ClickMode clickMode)
{
	EventHandler::getInstance()->click(m_x,m_y, clickMode);
}

void ScreenGrid::regionSelected()
{
	KPushButton *senderBtn = dynamic_cast<KPushButton*>(sender());
	if (!senderBtn) return;

	if ((senderBtn->width() <= 20) && (senderBtn->height() <= 20))
	{
		click(senderBtn);
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

	setMinimumWidth(btnSize.width());
	setMinimumHeight(btnSize.height());
	this->setMaximumWidth(btnSize.width());
	this->setMaximumHeight(btnSize.height());


	
	buttons->setGeometry(QRect(0, 0, geometry().width(), geometry().height()));
	
	this->move(pos);
	
	repaint();
	
	if (!static_cast<DesktopGridConfiguration*>(config)->useRealTransparency())
	{
		background->resize(size());
		background->move(0,0);
		background->setPixmap(deskShot.copy(geometry()));
	}
}

void ScreenGrid::keyPressEvent(QKeyEvent *event)
{
	if (!event) return;
	if (event->key()== Qt::Key_Escape) deleteLater();
}

bool ScreenGrid::greedyTrigger(const QString& input)
{
	if (!static_cast<DesktopGridConfiguration*>(config)->useRealTransparency())
	if (input.toUpper() == static_cast<DesktopGridConfiguration*>(config)->cancelTrigger().toUpper())
	{
		deleteLater();
		return true;
	}

	bool ok=false;
	int index = input.toInt(&ok)-1;
	if (!ok)
	{
		index = 0;
		while ((index < numberIdentifiers.count()) && (numberIdentifiers.at(index).toUpper() != input.toUpper()))
			index++;

		if (index == numberIdentifiers.count()) return false;
	}
	kDebug() << index;

	if (commandListWidget->isVisible()) {
		clickRequestReceived(index+1);
		return true;
	}

	if (index > btns.count()) return false;

	KPushButton *btn = btns[index];
	if (!btn) return false;

	btn->animateClick();
	return true;
}

ScreenGrid::~ScreenGrid()
{
	commandListWidget->deleteLater();
	buttons->deleteLater();
	stopGreedy();

	if (background) background->deleteLater();
}

