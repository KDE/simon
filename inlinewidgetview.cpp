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
#include <QMessageBox>
#include <QKeyEvent>
#include <QCoreApplication>

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
	QBrush inlineBrushInactive(QColor(255, 255, 255, 135));
	inlinePalette.setBrush(QPalette::Inactive, QPalette::Window, inlineBrushInactive);
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


	connect(this, SIGNAL(guiAction(QString)), page, SLOT(doAction(QString)));
	connect(this,SIGNAL(guiAction(QString)), page,SIGNAL(guiAction(QString)));
	
	
	for (int i=0; i < count(); i++)
	{
		if (widget(i) == page){
			QCoreApplication::processEvents();
			setCurrentIndex(i);
			return;
		}
	}


	int newpage = addTab(page, page->getTitle());
	setTabIcon(newpage, page->getIcon());
	setTabToolTip(newpage, page->getDesc());
	setCurrentIndex(newpage);
	
	if (count() > 1) tabBar()->show();
	emit registeredPage(page);
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
	emit unRegisteredPage(page);
}

/**
 * \brief Unregisters the currentPage
 * \author Peter Grasch
 */
void InlineWidgetView::unRegisterCurrentPage()
{
	if (!dynamic_cast<InlineWidget*>(currentWidget())) return;
	unRegisterPage(dynamic_cast<InlineWidget*>(currentWidget()));
}

/**
 * \brief Presses a key
 * \author Peter Grasch
 * @param event The event
 */
void InlineWidgetView::keyPressEvent ( QKeyEvent * event )
{
	if (event->key()==Qt::Key_Escape ) unRegisterCurrentPage();
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
InlineWidgetView::~InlineWidgetView()
{
}
