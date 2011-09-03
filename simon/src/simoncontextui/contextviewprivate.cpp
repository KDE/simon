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
#include <simonscenarios/scenario.h>
#include "newcondition.h"
#include "simoncontextdetection/contextmanager.h"

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

  connect ( ui.pbNewCondition, SIGNAL(clicked()), this, SLOT(addCondition()));
  connect ( ui.pbEditCondition, SIGNAL(clicked()), this, SLOT(editCondition()));
  connect ( ui.pbDeleteCondition, SIGNAL(clicked()), this, SLOT(deleteCondition()));

  connect ( ui.pbEditHierarchy, SIGNAL(clicked()), this, SIGNAL(manageScenariosTriggered()));

  ui.lvConditions->setIconSize(QSize(24,24));
  ui.lvConditions->setSpacing(2);

  ui.pbNewCondition->setIcon(KIcon("list-add"));
  ui.pbEditCondition->setIcon(KIcon("edit-rename"));
  ui.pbDeleteCondition->setIcon(KIcon("edit-delete"));

  conditionsProxy = new QSortFilterProxyModel(this);
  conditionsProxy->setFilterKeyColumn(0);
  conditionsProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  ui.lvConditions->setModel(conditionsProxy);

  connect(ui.leConditionsFilter, SIGNAL(textChanged(const QString&)), conditionsProxy, SLOT(setFilterRegExp(const QString&)));

  connect(ui.lvConditions->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(selectionChanged()));
}

Condition* ContextViewPrivate::getCurrentCondition()
{
    QModelIndex index = conditionsProxy->mapToSource(ui.lvConditions->currentIndex());
    if (!index.isValid()) return 0;

    return static_cast<Condition*>(index.internalPointer());
}

void ContextViewPrivate::addCondition()
{
    NewCondition *newCondition = new NewCondition(this);
    QList<CreateConditionWidget*>* widgets = new QList<CreateConditionWidget*>();
    QList<Condition*>* conditions;
    ContextManager* manager = ContextManager::instance();

    conditions = manager->getConditions();

    foreach (Condition* condition, *conditions)
    {
        widgets->push_back(condition->getCreateConditionWidget(scenario->compoundCondition(), this));
    }
    delete conditions;

    newCondition->registerCreators(widgets);
    newCondition->newCondition();

    delete newCondition;
}

void ContextViewPrivate::editCondition()
{
    //get the condition to edit
    Condition* condition = getCurrentCondition();
    if (!condition)
        return;

    //get the CreateConditionWidgets
    QList<CreateConditionWidget*>* widgets = new QList<CreateConditionWidget*>();
    QList<Condition*>* conditions = new QList<Condition*>();
    ContextManager* manager = ContextManager::instance();

    conditions = manager->getConditions();

    foreach (Condition* c, *conditions)
    {
        widgets->push_back(c->getCreateConditionWidget(scenario->compoundCondition(), this));
    }
    delete conditions;

    //prepare the edit condition dialog and launch it
    NewCondition *editCondition = new NewCondition(this);
    editCondition->registerCreators(widgets);
    editCondition->init(condition);
    bool succ = editCondition->newCondition();

    //on confirmation of the edit, the old condition is deleted and the new one made by the NewCondition replaces it
    if (succ)
    {
      scenario->compoundCondition()->removeCondition(condition);
      ui.lvConditions->setCurrentIndex(conditionsProxy->index(conditionsProxy->rowCount()-1, 0));
    }
    delete editCondition;
}

void ContextViewPrivate::deleteCondition()
{
    Condition* condition = getCurrentCondition();

    if (!condition)
        return;

    if (KMessageBox::questionYesNoCancel(this, i18n("Are you sure that you want to irreversibly remove the condition \"%1\"?", condition->name()), i18n("Remove Condition")) == KMessageBox::Yes)
    {
        if (!scenario->compoundCondition()->removeCondition(condition))
        {
            kDebug() << "Error removing condition!";
        }
    }
}

void ContextViewPrivate::selectionChanged()
{
    if (getCurrentCondition() == 0)
    {
        ui.pbDeleteCondition->setEnabled(false);
        ui.pbEditCondition->setEnabled(false);
    }
    else
    {
        ui.pbDeleteCondition->setEnabled(true);
        ui.pbEditCondition->setEnabled(true);
    }
}

void ContextViewPrivate::displayScenarioPrivate(Scenario *scenario)
{
  CompoundCondition *compoundCondition = scenario->compoundCondition();
  conditionsProxy->setSourceModel((QAbstractItemModel*) compoundCondition->getProxy());
  ui.lvConditions->setCurrentIndex(conditionsProxy->index(0,0));

  //fill in inherited activation requirments list
  ui.lwParentActivationRequirements->clear();
  QStringList inheritedRequirements;
  Scenario* parent = scenario->parentScenario();
  while (parent)
  {
      QList<Condition*> conditions = parent->compoundCondition()->getConditions();
      foreach(Condition* c, conditions)
      {
          inheritedRequirements.push_back("[" + parent->name() + "]: " + c->name());
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
