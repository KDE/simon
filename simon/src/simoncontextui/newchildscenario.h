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


#ifndef NEWCHILDSCENARIO_H
#define NEWCHILDSCENARIO_H

#include "simoncontextui_export.h"
#include <QWidget>
#include <KDialog>
#include "ui_newchildscenario.h"
#include "simonscenarios/scenario.h"
#include "simonscenarios/scenariolistmodel.h"

/**
 *	@class NewChildScenario
 *	@brief The NewChildScenario class is a dialog for adding child scenarios to a scenario
 *
 *
 *
 *      \sa ContextViewPrivate, Scenario
 *
 *	@version 0.1
 *	@date 7.18.2011
 *	@author Adam Nash
 */

class SIMONCONTEXTUI_EXPORT NewChildScenario : public KDialog
{
  Q_OBJECT

  private:
    Ui::NewChildScenario ui;
    ScenarioListModel* m_listModel;

  private slots:

  public:
    NewChildScenario(QWidget *parent=0);
    ~NewChildScenario();

    QString selectedScenarioId();

  public slots:
};

#endif // NEWCHILDSCENARIO_H
