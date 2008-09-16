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

#include <KTabWidget>

class InlineWidget;
class QKeyEvent;

/**
 \class InlineWidgetView
 \author Peter Grasch
 \brief Extends the QTabWidget to display InlienWidgets
 \version 0.1
 \date 10.8.2007
*/
class InlineWidgetView : public KTabWidget
{
Q_OBJECT
signals:
	void hidden();

	void guiAction(QString);
	
	void registeredPage(InlineWidget *page);
	void unRegisteredPage(InlineWidget *page);
	
private slots:
	void processCloseRequest(QWidget* page);

private:
	void hideEvent(QHideEvent *event) { emit hidden(); return QWidget::hideEvent(event); }

public:
    InlineWidgetView(QWidget* parent=0);
	void registerPage(InlineWidget *page);
	void focusPage(InlineWidget *page);
	void keyPressEvent(QKeyEvent * event);
	void unRegisterPage(InlineWidget *page);
	void unRegisterCurrentPage();
	
	void toggleDisplay(InlineWidget *page);

    ~InlineWidgetView();

};

#endif
