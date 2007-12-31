

#include "simonwidget.h"
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QMessageBox>
#include <QRegExp>
#include <QSpinBox>
#include "simongroupbox.h"
#include "simoncombobox.h"
#include "simonspinbox.h"
#include "simonlistwidget.h"
#include "simontablewidget.h"
#include "simontabwidget.h"
#include "simoncalendarwidget.h"
#include "simonslider.h"
#include <QCoreApplication>


void SimonWidget::guessChildTriggers(QObject *current) 
{
	
	QCoreApplication::processEvents();
	
	QObjectList childList = getChildren(current);
	QString text = "";
	int i = 0;
	//QMessageBox::information(0, current->objectName(), QString("count is %1").arg(childList.count()));
	while((i <childList.count()) && (childList.count() != 0))
	{	
		QObjectList temp = QObjectList();
		temp = getChildren(childList.at(i));
		if (temp.count() != 0)
		{
			guessChildTriggers(childList.at(i));
		}
		
		if ((dynamic_cast<QAbstractButton*>(childList.at(i))) || (dynamic_cast<QCheckBox*>(childList.at(i))))
		{
			text = dynamic_cast<QAbstractButton*>(childList.at(i))->text();
			text = editChildKeys(text);
			//QMessageBox::critical(NULL,"",text);
			registerControl(text, childList.at(i), SLOT(animateClick()));
		}
		//TODO
		if (dynamic_cast<SimonTabWidget*>(childList.at(i)))
		{
			int j = 0;
			while(j < dynamic_cast<SimonTabWidget*>(childList.at(i))->count())
			{
				text = dynamic_cast<SimonTabWidget*>(childList.at(i))->tabText(j);
				text = editChildKeys(text);
				//QMessageBox::critical(NULL,text,"tabw");
				registerControl(text, childList.at(i), SLOT(setTab(QString)));
				j++;
			}
		}
		if (dynamic_cast<QLabel*>(childList.at(i)))
		{
			registerBuddy(dynamic_cast<QLabel*>(childList.at(i)));
		}
		
		//todo
		if (dynamic_cast<SimonGroupBox*>(childList.at(i)))
		{
			text = dynamic_cast<SimonGroupBox*>(childList.at(i))->title();
			text = editChildKeys(text);
			//QMessageBox::critical(NULL,text,"gb");
			registerControl(text, childList.at(i), SLOT(changeCheckState()));
		}
		
		i++;
		//if (text != "")
		//QMessageBox::critical(NULL,text,text);
	}
}



void SimonWidget::registerBuddy(QLabel* label)
{
	QString text;
	text = label->text();
	
	text = editChildKeys(text);
	
	//TODO____________________________
	/*if(text == "Shortcuts")
		registerControl(text,label->buddy(),SLOT(showLineEdit()));*/
	//___________________________________
	
	
	if (dynamic_cast<QLineEdit*>(label->buddy()))
	{
		registerControl(text,label->buddy(),SLOT(setFocus()));
	}
	
	if (dynamic_cast<SimonListWidget*>(label->buddy()))
	{
		registerControl(text,label->buddy(),SLOT(showLineEdit()));
	}
	
	if (dynamic_cast<SimonComboBox*>(label->buddy()))
	{
		registerControl(text,label->buddy(),SLOT(schowPopUp()));
	}
	
	if (dynamic_cast<SimonSpinBox*>(label->buddy()))
	{
		registerControl(text,label->buddy(),SLOT(setFocus()));
		registerControl(text+" up",label->buddy(),SLOT(changeValue(QString)));
		registerControl(text+" down",label->buddy(),SLOT(changeValue(QString)));
	}
	
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
	
	if (dynamic_cast<SimonCalendarWidget*>(label->buddy()))
	{
		QString extendedtext;
		extendedtext = text+" nächster Monat";
		registerControl(extendedtext,label->buddy(),SLOT(showNextMonth()));
		//QMessageBox::information(NULL,label->buddy()->objectName(),extendedtext);
		extendedtext = text+" vorheriger Monat";
		registerControl(extendedtext,label->buddy(),SLOT(showPreviousMonth()));
		//QMessageBox::information(NULL,"",extendedtext);
		extendedtext = text + " Tag ";
		//QMessageBox::information(NULL,"",extendedtext);
		registerControl(extendedtext,label->buddy(),SLOT(setDate(QString)));
		extendedtext = text + " Monat ";
		//QMessageBox::information(NULL,"",extendedtext);
		registerControl(extendedtext,label->buddy(),SLOT(setDate(QString)));
		extendedtext = text + " Jahr ";
		registerControl(extendedtext,label->buddy(),SLOT(setDate(QString)));
		//todo...regexp
		//registerControl(text,label->buddy(),SLOT());
		//setCurrentDate(int,int)
		
		//todo...regexp
		//registerControl(text,label->buddy(),SLOT());
		//simon Kalender <zahl>
	}
	if (dynamic_cast<SimonSlider*>(label->buddy()))
	{
		registerControl(text,label->buddy(),SLOT(changeValue()));
	}
	
	
	
}


QString SimonWidget::editChildKeys(QString oldKey)
{	
	/*if (oldKey.contains("&"))
		return oldKey.remove(oldKey.indexOf("&"),1); 
	if (oldKey.contains(":"))
		return oldKey.remove(oldKey.indexOf(":"),1); 
	if (oldKey.contains("("))
		return oldKey.remove(oldKey.indexOf("("),1); 
	if (oldKey.contains(")"))
		return oldKey.remove(oldKey.indexOf(")"),1); 
	if (oldKey.contains("<"))
		return oldKey.remove(oldKey.indexOf(">"),1); 
	else
		return oldKey;*/
	//WTF?
	
	oldKey.remove("&");
	oldKey.remove(":");
	oldKey.remove("(");
	oldKey.remove(")");
	oldKey.remove("<");
	oldKey.remove(">");
	return oldKey;
}
