#include "simoncalendarwidget.h"
#include <QCalendarWidget>
#include <KLocalizedString>
#include <KMessageBox>
#include <QRegExp>
#include <QDate>



SimonCalendarWidget::SimonCalendarWidget( QWidget * parent ) : QCalendarWidget(parent)
{
	//whatis = 1 Day received
	//whatis = 2 Month received
	//whatis = 3 Year received
	this->whatis = 0;
}


SimonCalendarWidget::~SimonCalendarWidget()
{

}

void SimonCalendarWidget::setDate(QString text)
{
	if (text.contains(i18n("Tag")))
		this->whatis = 1;
	if (text.contains(i18n("Monat")))
		this->whatis = 2;
	if (text.contains(i18n("Jahr")))
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
		KMessageBox::information(this,i18n("Der gewünschte Tag existiert in diesem Monat nicht.\nBitte führen Sie den Befehl mit einem anderen Tag nochmal aus.\n\nEs wurde der erste des Monats als Default Datum gewählt"));
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
		KMessageBox::information(this,i18n("Der gewünschte Monat existiert nicht.\nBitte widerholen Sie ihren Befehl mit einem anderen Wert"));
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
		if (KMessageBox::questionYesNoCancel(0, i18n("Das von Ihnen gewünschte Jahr ist: ")+QString::number(value)+i18n("\nWollen sie wirklich dieses Jahr anzeigen?"))==KMessageBox::Yes)
			this->setSelectedDate (date);
		else return;
	}
	else
		KMessageBox::information(this,i18n("Ihr gewünschter Wert überschreitet den Max/Min Wert.\nBitte führen sie den Befehl erneut aus, mit einem Jahr zw.\n-4714 & +8000\n\nDer Programmierer dankt"));
}



