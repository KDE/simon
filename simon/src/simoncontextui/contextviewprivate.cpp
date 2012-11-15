/*
 *   Copyright (C) 2011 Adam Nash adam.t.nash@gmail.com
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

#include "contextviewprivate.h"
#include <simoncontextdetection/contextmanager.h>
#include <simonscenarios/scenario.h>
#include <simonscenarioui/scenariomanagementdialog.h>
#include <simoncontextcoreui/newcondition.h>

#include <QWidget>
#include <QPointer>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QSize>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>

#include <KMessageBox>
#include <KIcon>
#include <KCMultiDialog>

ContextViewPrivate::ContextViewPrivate(QWidget *parent) : QWidget(parent)
{
  ui.setupUi(this);
  connect ( ui.pbEditHierarchy, SIGNAL(clicked()), this, SLOT(manageScenarios()));
}

void ContextViewPrivate::manageScenarios()
{
  ScenarioManagementDialog::configureScenarios(this);
}

void ContextViewPrivate::displayScenarioPrivate(Scenario *scenario)
{
  CompoundCondition *compoundCondition = scenario->compoundCondition();
  ui.wgCompoundConditionSettings->setConditions(compoundCondition);

  //fill in inherited activation requirments list
  ui.lwParentActivationRequirements->clear();
  QStringList inheritedRequirements;
  Scenario* parent = scenario->parentScenario();
  while (parent)
  {
      QList<Condition*> conditions = parent->compoundCondition()->getConditions();
      foreach(Condition* c, conditions)
      {
          inheritedRequirements.push_back('[' + parent->name() + "]: " + c->name());
      }

      parent = parent->parentScenario();
  }
  ui.lwParentActivationRequirements->addItems(inheritedRequirements);

  //fill in child scenario tree
  ui.twChildScenarios->clear();
  addChildScenarios(ui.twChildScenarios->invisibleRootItem(), scenario);
}

void ContextViewPrivate::addChildScenarios(QTreeWidgetItem *parentItem, Scenario *parentScenario)
{
    QList<Scenario*> childScenarios;
    childScenarios = parentScenario->childScenarios();

    foreach(Scenario* s, childScenarios)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
        item->setText(0, s->name());
        item->setIcon(0, s->icon());
        addChildScenarios(item, s);
    }
}

ContextViewPrivate::~ContextViewPrivate()
{
}
