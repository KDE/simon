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

#include "simoncalendarwidget.h"
#include <QCalendarWidget>
#include <KLocalizedString>
#include <KMessageBox>
#include <QRegExp>
#include <QDate>
#include <KLocale>



SimonCalendarWidget::SimonCalendarWidget(QWidget* parent) : QCalendarWidget(parent),
	//whatis = 1 Day received
	//whatis = 2 Month received
	//whatis = 3 Year received
	whatis(0)
{
	KLocale::setMainCatalog("simonlib");
}


SimonCalendarWidget::~SimonCalendarWidget()
{

}

void SimonCalendarWidget::setDate(QString text)
{
	if (text.contains(i18n("Day")))
		this->whatis = 1;
	if (text.contains(i18n("Month")))
		this->whatis = 2;
	if (text.contains(i18n("Year")))
		this->whatis = 3;
	
	text = clearString(text);
	int value = text.toInt();
	
	 switch(this->whatis)
		{
		case 1:
			setDay(value);
			break;
		case 2:
			setMonth(value);
			break;
		case 3:
			setYear(value);
			break;
	}

}
	
QString SimonCalendarWidget::clearString(QString text)
{
	text = text.remove(0,9);
	text = text.remove(QRegExp("[^0-9]"));
	text = text.trimmed();
	return text;
}


void SimonCalendarWidget::setDay(int value)
{
	QDate currentdate = this->selectedDate();
	int month = currentdate.month();
	int year = currentdate.year();
	int daycount = currentdate.daysInMonth();
	if((value <= daycount) && (value >0))
	{
		QDate date(year,month,value);
		this->setSelectedDate (date);
	}
	else
	{
		KMessageBox::information(this,i18n("The selected day does not exist in that month.\nPlease try again with a vaild date."));
		QDate date(year,month,1);
		this->setSelectedDate (date);
	}
	
	
}

void SimonCalendarWidget::setMonth(int value)
{
	QDate currentdate = this->selectedDate();
	int year = currentdate.year();
	if((value <= 13) && (value >0))
	{
		QDate date(year,value,1);
		this->setSelectedDate (date);
	}
	else
	{
		KMessageBox::information(this,i18n("The selected month does not exist.\n\nPlease try again with a valid date."));
	}
	
}
//-4713
//+7999
void SimonCalendarWidget::setYear(int value)
{
	QDate date(value,1,1);
	if ((value > 1000) && (value <3000))
	{
		this->setSelectedDate (date);
	}
	else if((value > -4713) && (value <7999))
	{
		if (KMessageBox::questionYesNoCancel(0, i18n("You selected the year: %1.\n\nDo you really want to show that year?", QString::number(value)))==KMessageBox::Yes)
			this->setSelectedDate (date);
		else return;
	}
	else
		KMessageBox::information(this,i18n("The value you selected exceeds the boundaries of allowed values."));
}



