//
// C++ Interface: inlinewidgetview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INLINEWIDGETVIEW_H
#define INLINEWIDGETVIEW_H

#include <QWidget>
#include <QTabWidget>

class InlineWidget;

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class InlineWidgetView : public QTabWidget
{
Q_OBJECT
signals:
	void hidden();
private:
	
	void hideEvent(QHideEvent *event) { emit hidden(); return QWidget::hideEvent(event); }
public:
    InlineWidgetView(QWidget* parent=0);
	void registerPage(InlineWidget *page);
	void unRegisterPage(InlineWidget *page);

    ~InlineWidgetView();

};

#endif
