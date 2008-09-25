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

#include "simonslider.h"
#include <KMessageBox>
#include <QColor>
#include <QBrush>
#include <QPalette>
#include <KLineEdit>
#include <KLocalizedString>



SimonSlider::SimonSlider( QWidget * parent ) : QSlider(parent)
{	
	this->lePlusMinus = new KLineEdit(dynamic_cast<QWidget*>(this->parent()));
	this->lePlusMinus->setInputMask ("99");
	this->lePlusMinus->setVisible(false);
	this->setMinimum(0);
	this->setMaximum(100);
	this->lePlusMinus->setMaximumHeight( 18 ) ;
	this->lePlusMinus->setMinimumHeight ( 18 ) ;
	QColor color(255,255,255,127);
	QBrush brush(color, Qt::SolidPattern);
	QPalette palette = this->lePlusMinus->palette();
	palette.setBrush(QPalette::Base,brush);
	this->lePlusMinus->setPalette(palette);
	
	connect(lePlusMinus,SIGNAL(returnPressed()),this,SLOT(doCommand())); 
}	

SimonSlider::~SimonSlider()
{
	lePlusMinus->deleteLater();
}

void SimonSlider::changeValue()
{
	this->lePlusMinus->setVisible(true);
	this->lePlusMinus->move(dynamic_cast<QWidget*>(lePlusMinus->parent())->width()-lePlusMinus->width(),10);
	this->lePlusMinus->setFocus();
	this->lePlusMinus->clear();
}

void SimonSlider::doCommand()
{
	if ((this->lePlusMinus->text().toInt() >100) || (this->lePlusMinus->text().toInt() < 0))
	{	
		KMessageBox::information(this,i18n("Der von Ihnen eingegebene Wert ist auserhalb des Bereiches der eingestellt werden kann.\nBitte geben sie einen Wert zwischen\n\t0 und 100\nein."));
		return;
	}
		this->setValue(this->lePlusMinus->text().toInt()); 
		this->lePlusMinus->setVisible(false);
}


