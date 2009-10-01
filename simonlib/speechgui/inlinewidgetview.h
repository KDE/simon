/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SIMON_INLINEWIDGETVIEW_H_279F7C1C95C94FFEA3A7CC7B1B1A2B9F
#define SIMON_INLINEWIDGETVIEW_H_279F7C1C95C94FFEA3A7CC7B1B1A2B9F

#include <KTabWidget>
#include "speechgui_export.h"

class InlineWidget;
class QKeyEvent;

/**
 \class InlineWidgetView
 \author Peter Grasch
 \brief Extends the QTabWidget to display InlienWidgets
 \version 0.1
 \date 10.8.2007
*/
class SPEECHGUI_EXPORT InlineWidgetView : public KTabWidget
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
