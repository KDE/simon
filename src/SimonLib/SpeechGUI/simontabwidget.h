

#ifndef SIMONTABWIDGET_H
#define SIMONTABWIDGET_H

#include <QTabWidget>

class SimonTabWidget : public QTabWidget
{	
	
	Q_OBJECT
	
private:
	
	
public:
	SimonTabWidget( QWidget * parent  = 0);
	~SimonTabWidget();

public slots:
	void setTab(QString text);

	
};


#endif
