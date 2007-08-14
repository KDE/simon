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

/**
 * \brief Constructor - inits the gui
 * \author Peter Grasch
 * @param parent the parent of the widget 
 */
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

/**
 * \brief Registers the given InlineWidget as a new page and displays it
 * \author Peter Grasch
 * @param page the page to register
 */
void InlineWidgetView::registerPage(InlineWidget *page)
{
	if(!page) return;

	int newpage = addTab(page, page->getTitle());
	setTabIcon(newpage, page->getIcon());
	setTabToolTip(newpage, page->getDesc());
	setCurrentIndex(newpage);
	
	if (count() > 1) tabBar()->show();
}
/**
 * \brief Unregisters the given InlineWidget as a new page and hides it
 * \author Peter Grasch
 * @param page the page to remove
 */
void InlineWidgetView::unRegisterPage(InlineWidget *page)
{
	if(!page) return;

	removeTab(indexOf(page));

	if (count() < 2) tabBar()->hide();
	if (count() == 0) hide();
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
InlineWidgetView::~InlineWidgetView()
{
}


