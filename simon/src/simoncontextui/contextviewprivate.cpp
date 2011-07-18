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
#include "simonscenarios/scenariotreemodel.h"
#include "newchildscenario.h"

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

  connect (ui.pbAddChild, SIGNAL(clicked()), this, SLOT(addChild()));
  connect (ui.pbRemoveChild, SIGNAL(clicked()), this, SLOT(removeChild()));

  ui.lvConditions->setIconSize(QSize(24,24));
  ui.lvConditions->setSpacing(2);

  ui.pbNewCondition->setIcon(KIcon("list-add"));
  ui.pbEditCondition->setIcon(KIcon("edit-rename"));
  ui.pbDeleteCondition->setIcon(KIcon("edit-delete"));

  ui.pbAddChild->setIcon(KIcon("list-add"));
  ui.pbRemoveChild->setIcon(KIcon("edit-delete"));

  conditionsProxy = new QSortFilterProxyModel(this);
  conditionsProxy->setFilterKeyColumn(0);
  conditionsProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  ui.lvConditions->setModel(conditionsProxy);

  m_childrenTreeModel = new ScenarioTreeModel(this);
  ui.tvChildScenarios->setModel(m_childrenTreeModel);

  connect(ui.leConditionsFilter, SIGNAL(textChanged(const QString&)), conditionsProxy, SLOT(setFilterRegExp(const QString&)));

  connect(ui.lvConditions->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(selectionChanged()));

  connect(ui.tvChildScenarios->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(selectedChildChanged()));
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

void ContextViewPrivate::addChild()
{
    //TODO: filter out invalid children (disable adding children that are already parents)
    NewChildScenario* newChild = new NewChildScenario(this);

    if (newChild->exec())
    {
        scenario->addChild(newChild->selectedScenarioId());
        m_childrenTreeModel->update();
    }
}

void ContextViewPrivate::removeChild()
{
    scenario->removeChild(ui.tvChildScenarios->currentIndex().data().toString());
    m_childrenTreeModel->update();
}

void ContextViewPrivate::selectedChildChanged()
{
    if (scenario->childScenarioIds().contains(ui.tvChildScenarios->currentIndex().data().toString()))
    {
        ui.pbRemoveChild->setEnabled(true);
    }
    else
    {
        ui.pbRemoveChild->setEnabled(false);
    }
}

void ContextViewPrivate::displayScenarioPrivate(Scenario *scenario)
{
  CompoundCondition *compoundCondition = scenario->compoundCondition();
  conditionsProxy->setSourceModel((QAbstractItemModel*) compoundCondition->getProxy());
  ui.lvConditions->setCurrentIndex(conditionsProxy->index(0,0));

  m_childrenTreeModel->setRootScenario(scenario);
  ui.tvChildScenarios->setModel(m_childrenTreeModel);
  ui.tvChildScenarios->setCurrentIndex(m_childrenTreeModel->index(0, 0));
}

ContextViewPrivate::~ContextViewPrivate()
{
}
