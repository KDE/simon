/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#ifndef CONTEXTVIEW_H
#define CONTEXTVIEW_H

/** \file contextview.h
 * \brief The file containing the ContextView baseclass header.
 */

#include <simonuicomponents/inlinewidget.h>
#include "simoncontextui_export.h"
#include <simonscenarios/scenariodisplay.h>

class ContextViewPrivate;
class Scenario;

/**
 *	@class ContextView
 *	@brief The ContextView class contains a ContextViewPrivate for changing the context requirements of a scenario
 *
 *
 *
 *      \sa ContextViewPrivate, Scenario, Condition
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class SIMONCONTEXTUI_EXPORT ContextView : public InlineWidget, public ScenarioDisplay
{
  Q_OBJECT

    signals:
  void actionsChanged();
  void manageScenariosTriggered();

  private:
    ContextViewPrivate *d;

  protected:
    void displayScenarioPrivate(Scenario *scenario);

  public:
    ContextView(QWidget *parent);

    ~ContextView();

};

#endif // CONTEXTVIEW_H
