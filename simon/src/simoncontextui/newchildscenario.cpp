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


#include "newchildscenario.h"
#include "ui_newchildscenario.h"

NewChildScenario::NewChildScenario(QWidget* parent) : KDialog(parent)
{
  QWidget *widget = new QWidget( this );
  ui.setupUi(widget);

  setMainWidget( widget );
  setCaption( i18n("Add Child Scenario") );

  m_listModel = new ScenarioListModel();

  ui.lvScenarios->setModel(m_listModel);
}

NewChildScenario::~NewChildScenario()
{
}

QString NewChildScenario::selectedScenarioId()
{
    return ui.lvScenarios->currentIndex().data().toString();
}
