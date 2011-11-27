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

#ifndef SIMON_TRAININGVIEW_H_5E15EE6E6EEE4D1BA26A9AD8900F7E60
#define SIMON_TRAININGVIEW_H_5E15EE6E6EEE4D1BA26A9AD8900F7E60
#include <simonuicomponents/inlinewidget.h>
#include <simonscenarios/word.h>
#include <simonscenarios/scenariodisplay.h>
#include "simonmodelmanagementui_export.h"

class TrainingViewPrivate;
class Scenario;

class SIMONMODELMANAGEMENTUI_EXPORT TrainingView : public InlineWidget, public ScenarioDisplay
{
  Q_OBJECT

  public:
    TrainingView(QWidget *parent=0);
    ~TrainingView();

  protected:
    void displayScenarioPrivate(Scenario *scenario);

  private:
    TrainingViewPrivate *d;
};
#endif
