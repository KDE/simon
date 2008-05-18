

#ifndef SIMONCALENDARWIDGET_H
#define SIMONCALENDARWIDGET_H

#include <QCalendarWidget>

class SimonCalendarWidget : public QCalendarWidget
{	
	
	Q_OBJECT
	
private:
	int whatis;
public:
	SimonCalendarWidget( QWidget * parent  = 0);
	~SimonCalendarWidget();

public slots:

	void setDate(QString text);
	QString clearString(QString text);
	void setDay(int value);
	void setMonth(int value);
	void setYear(int value);
	
	
};


#endif
