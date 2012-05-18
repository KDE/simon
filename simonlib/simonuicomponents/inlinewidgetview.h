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

#include <KPageWidget>
#include "simonuicomponents_export.h"

class InlineWidget;
class QKeyEvent;
class KPageWidget;

/**
 \class InlineWidgetView
 \author Peter Grasch
 \brief Extends the QTabWidget to display InlineWidgets
 \version 0.1
 \date 10.8.2007
*/
class SIMONUICOMPONENTS_EXPORT InlineWidgetView : public KPageWidget
{
    Q_OBJECT
  
signals:
    void guiAction(QString);
    void registeredPage(InlineWidget *page);

public:
      InlineWidgetView(QWidget* parent=0);
      void registerPage(InlineWidget *page);
      void removePage(QWidget *page);
      void focusPage(InlineWidget *page);
      
private:
      KPageWidgetItem* resolvePage(QWidget *content);
};
#endif
