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

#include "inlinewidgetview.h"
#include "inlinewidget.h"
#include <QTabBar>
#include <QKeyEvent>
#include <QCoreApplication>

/**
 * \brief Constructor - inits the gui
 * \author Peter Grasch
 * @param parent the parent of the widget
 */
InlineWidgetView::InlineWidgetView(QWidget* parent): KTabWidget(parent)
{
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

  int newpage = addTab(page, page->getTitle());
  setTabIcon(newpage, page->getIcon());
  setTabToolTip(newpage, page->getDesc());

  emit registeredPage(page);  
  tabBar()->setVisible(count() > 1);
}

void InlineWidgetView::removePage ( QWidget* page )
{
  KTabWidget::removePage ( page );
  tabBar()->setVisible(count() > 1);
}


