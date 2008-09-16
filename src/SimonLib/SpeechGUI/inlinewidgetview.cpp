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
InlineWidgetView::InlineWidgetView(QWidget* parent): KTabWidget(parent)
{
	tabBar()->hide();
	
	connect(this, SIGNAL(closeRequest (QWidget *)), this, SLOT(processCloseRequest(QWidget*)));
}

void InlineWidgetView::processCloseRequest(QWidget* page)
{
	InlineWidget *iwPage = dynamic_cast<InlineWidget*>(page);
	if (!iwPage) return;
	
	unRegisterPage(iwPage);
}

void InlineWidgetView::toggleDisplay(InlineWidget *page)
{
	if (!page) return;
	
	if (! page->isShown()) registerPage(page);
	else unRegisterPage(page);
}


void InlineWidgetView::focusPage(InlineWidget *page)
{
	if (!page) return;
	

	int index = indexOf(page);
	if (index == -1) return;
	
	setCurrentIndex(index);
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
// 	connect(this,SIGNAL(guiAction(QString)), page,SIGNAL(guiAction(QString)));
	
	
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
	if(!page || (indexOf(page) == -1)) return;

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
