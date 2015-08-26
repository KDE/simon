/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

/**
 * \brief Constructor - inits the gui
 * \author Peter Grasch
 * @param parent the parent of the widget
 */
InlineWidgetView::InlineWidgetView(QWidget* parent): KPageWidget(parent)
{
  setFaceType(List);
}

void InlineWidgetView::focusPage(InlineWidget *page)
{
  Q_ASSERT(page);
  setCurrentPage(resolvePage(page));
}

/**
 * \brief Registers the given InlineWidget as a new page and displays it
 * \author Peter Grasch
 * @param page the page to register
 */
void InlineWidgetView::registerPage(InlineWidget *page)
{
  Q_ASSERT(page);

  KPageWidgetItem *item = addPage(page, page->getTitle());
  item->setIcon(page->getIcon());
  item->setHeader(page->getDesc());
  
  emit registeredPage(page);  
}

void InlineWidgetView::removePage ( QWidget* page )
{
  KPageWidgetItem *p = resolvePage(page);
  
  KPageWidget::removePage(p);
}

KPageWidgetItem* InlineWidgetView::resolvePage ( QWidget* content )
{
  KPageWidgetModel *m = static_cast<KPageWidgetModel*>(model());
  int count = m->rowCount();
  
  for (int i=0; i < count; i++) {
    KPageWidgetItem* it = m->item(m->index(i, 0));
    Q_ASSERT(it);
    if (it->widget() == content)
      return it;
  }
  return 0;
}
