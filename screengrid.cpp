//
// C++ Implementation: screengrid
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "screengrid.h"
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>

ScreenGrid::ScreenGrid(QWidget* parent): QWidget(parent, 
		Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint)
{
	QGridLayout *test = new QGridLayout(this);
	test->setSpacing(0);
	QSize desksize = QDesktopWidget().screenGeometry().size();
	short btnNr=1;
	for (int i=0; i < 3; i++)
	{
		for (int j=0; j<3; j++)
		{
			QPushButton *btn = new QPushButton(QString::number(btnNr), this);
// 			btn->setFlat(true);
			QFont f = btn->font();
			f.setPointSize(32);
			f.setBold(true);
			btn->setFont(f);
			btn->setMinimumHeight(desksize.height()/3);
			connect(btn, SIGNAL(clicked()), this, SLOT(regionSelected()));
			test->addWidget(btn, i, j);
			btnNr++;
		}
	}
	this->resize(desksize);
	this->setWindowOpacity(0.55);
}

void ScreenGrid::regionSelected()
{
// 	QMessageBox::critical(this, "sidfjsidjf", "sdijsfoij");
	QPushButton *senderBtn = dynamic_cast<QPushButton*>(sender());
	if (!senderBtn) return;
	QSize btnSize = senderBtn->size();
	QPoint pos = senderBtn->pos();
	pos.setX(pos.x()+this->x());
	pos.setY(pos.y()+this->y());
	qDebug() << pos.y()+this->y();

	int btnHeight = senderBtn->height()/3;

	for (int i=0; i < this->children().count(); i++)
	{
		QPushButton *btn = dynamic_cast<QPushButton*>(children().at(i));
		if (btn)
			btn->setMinimumHeight(btnHeight);
	}


	this->setMaximumSize(btnSize);
	this->move(pos);
	
}

void ScreenGrid::keyPressEvent(QKeyEvent *event)
{
	if (!event) return;
	if (event->key()== Qt::Key_Escape) this->hide();
}


ScreenGrid::~ScreenGrid()
{
}


