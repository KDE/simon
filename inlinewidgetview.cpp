//
// C++ Implementation: inlinewidgetview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "inlinewidgetview.h"
#include "inlinewidget.h"
#include <QTabBar>
#include <QPalette>
#include <QBrush>

InlineWidgetView::InlineWidgetView(QWidget* parent): QTabWidget(parent)
{
	setAutoFillBackground(false);
	tabBar()->hide();
	QPalette inlinePalette;
	QBrush inlineBrush(QColor(255, 255, 255, 100));
	inlineBrush.setStyle(Qt::SolidPattern);
	inlinePalette.setBrush(QPalette::Active, QPalette::Window, inlineBrush);
	setPalette(inlinePalette);
	setAutoFillBackground(true);
}

void InlineWidgetView::registerPage(InlineWidget *page)
{
	if(!page) return;

	int newpage = addTab(page, page->getTitle());
	setTabIcon(newpage, page->getIcon());
	setTabToolTip(newpage, page->getDesc());
	setCurrentIndex(newpage);
	
	if (count() > 1) tabBar()->show();
}

void InlineWidgetView::unRegisterPage(InlineWidget *page)
{
	if(!page) return;

	removeTab(indexOf(page));

	if (count() < 2) tabBar()->hide();
	if (count() == 0) hide();
}

InlineWidgetView::~InlineWidgetView()
{
}


