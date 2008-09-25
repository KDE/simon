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



#include "simonwidget.h"
#include <KPushButton>
#include <KLineEdit>
#include <QLabel>
#include <QCheckBox>
#include "simongroupbox.h"
#include "simoncombobox.h"
#include "simonspinbox.h"
#include "simonlistwidget.h"
#include "simontablewidget.h"
#include "simontabwidget.h"
#include "simoncalendarwidget.h"
#include "simonslider.h"


void SimonWidget::guessChildTriggers(QObject *current) 
{
	QObjectList childList = getChildren(current);
	QString text = "";
	int i = 0;

	while((i <childList.count()))
	{	
		QObjectList temp = QObjectList();
		temp = getChildren(childList.at(i));
		if (temp.count() != 0)
		{
			guessChildTriggers(childList.at(i));
		}
	//QAbstractButton
		if ((dynamic_cast<QAbstractButton*>(childList.at(i))))
		{
			text = dynamic_cast<QAbstractButton*>(childList.at(i))->text();
			text = editChildKeys(text);
			
			registerControl(text, childList.at(i), SLOT(animateClick()));
		}
	//SimonTabWidget
		if (dynamic_cast<SimonTabWidget*>(childList.at(i)))
		{
			int j = 0;
			while(j < dynamic_cast<SimonTabWidget*>(childList.at(i))->count())
			{
				text = dynamic_cast<SimonTabWidget*>(childList.at(i))->tabText(j);
				text = editChildKeys(text);
				registerControl(text, childList.at(i), SLOT(setTab(QString)));
				j++;
			}
		}
	//QLabel
		if (dynamic_cast<QLabel*>(childList.at(i)))
		{
			registerBuddy(dynamic_cast<QLabel*>(childList.at(i)));
		}
	//SimonGroupBox
		if (dynamic_cast<SimonGroupBox*>(childList.at(i)))
		{
			text = dynamic_cast<SimonGroupBox*>(childList.at(i))->title();
			text = editChildKeys(text);
			registerControl(text, childList.at(i), SLOT(changeCheckState()));
		}
		
		i++;
		
	}
}



void SimonWidget::registerBuddy(QLabel* label)
{
	QString text;
	text = label->text();
	
	text = editChildKeys(text);
//KLineEdit
	if (dynamic_cast<KLineEdit*>(label->buddy()))
	{
		registerControl(text,label->buddy(),SLOT(setFocus()));
	}
//SimonListWidget
	if (dynamic_cast<SimonListWidget*>(label->buddy()))
	{
		registerControl(text,label->buddy(),SLOT(showLineEdit()));
	}
//SimonComboBox
	if (dynamic_cast<SimonComboBox*>(label->buddy()))
	{
		registerControl(text,label->buddy(),SLOT(schowPopUp()));
	}
//SimonSpinBox
	if (dynamic_cast<SimonSpinBox*>(label->buddy()))
	{
		registerControl(text,label->buddy(),SLOT(setFocus()));
		registerControl(text+" up",label->buddy(),SLOT(changeValue(QString)));
		registerControl(text+" down",label->buddy(),SLOT(changeValue(QString)));
	}
//SimonTableWidget
	if (dynamic_cast<SimonTableWidget*>(label->buddy()))
	{
		SimonTableWidget * temp = dynamic_cast<SimonTableWidget*>(label->buddy());
		registerControl(text,label->buddy(),SLOT(initLineEdit(QString)));
		for (int i = 0; i<temp->columnCount(); i++)
		{
			QString str = temp->horizontalHeaderItem (i)->text();
			str = text + " " + str;
			registerControl(str,label->buddy(),SLOT(initLineEdit(QString)));
		}
	}
//SimonCalendarWidget
	if (dynamic_cast<SimonCalendarWidget*>(label->buddy()))
	{
		QString extendedtext;
		extendedtext = text+" nächster Monat";
		extendedtext = text + " Tag ";
		registerControl(extendedtext,label->buddy(),SLOT(setDate(QString)));
		extendedtext = text + " Monat ";
		registerControl(extendedtext,label->buddy(),SLOT(setDate(QString)));
		extendedtext = text + " Jahr ";
		registerControl(extendedtext,label->buddy(),SLOT(setDate(QString)));
	}
//SimonSlider
	if (dynamic_cast<SimonSlider*>(label->buddy()))
	{
		registerControl(text,label->buddy(),SLOT(changeValue()));
	}
	
	
	
}


QString SimonWidget::editChildKeys(QString oldKey)
{	
	oldKey.remove("&");
	oldKey.remove(":");
	oldKey.remove("(");
	oldKey.remove(")");
	oldKey.remove("<");
	oldKey.remove(">");
	return oldKey;
}
