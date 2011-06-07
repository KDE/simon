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
#include <simonscenarios/action.h>
#include <simonscenarios/commandmanager.h>
#include <simonscenarios/scenario.h>
#include "newcondition.h"
#include "simoncontextdetection/contextmanager.h"
//#include "manageactionsdialog.h"

#include <QWidget>
#include <QPointer>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QSize>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>

#include <KDE/KMessageBox>
#include <KDE/KIcon>
#include <KDE/KCMultiDialog>

ContextViewPrivate::ContextViewPrivate(QWidget *parent) : QWidget(parent)
{
  ui.setupUi(this);

  connect ( ui.pbNewCondition, SIGNAL(clicked()), this, SLOT(addCondition()));
  //connect ( ui.pbEditCommand, SIGNAL(clicked()), this, SLOT(editCommand()));
  //connect ( ui.pbDeleteCommand, SIGNAL(clicked()), this, SLOT(deleteCommand()));
  //connect ( ui.pbManagePlugins, SIGNAL(clicked()), this, SLOT(managePlugIns()));

  ui.lvConditions->setIconSize(QSize(24,24));
  ui.lvConditions->setSpacing(2);

  ui.pbNewCondition->setIcon(KIcon("list-add"));

  conditionsProxy = new QSortFilterProxyModel(this);
  conditionsProxy->setFilterKeyColumn(0);
  conditionsProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  ui.lvConditions->setModel(conditionsProxy);

  connect(ui.leConditionsFilter, SIGNAL(textChanged(const QString&)), conditionsProxy, SLOT(setFilterRegExp(const QString&)));

  //connect(ui.leActionsFilter, SIGNAL(textChanged(const QString&)), this, SLOT(fetchCommandsFromCategory()));
  //connect(ui.lvActions->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(fetchCommandsFromCategory()));
  //connect(ui.lvCommands->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(updateCommandDetail()));
}

void ContextViewPrivate::addCondition()
{
    NewCondition *newCondition = new NewCondition(this);
    QList<CreateConditionWidget*>* widgets = new QList<CreateConditionWidget*>();
    QList<Condition*>* conditions = new QList<Condition*>();
    ContextManager* manager = ContextManager::instance();

    conditions = manager->getConditions();

    foreach (Condition* condition, *conditions)
    {
        widgets->push_back(condition->getCreateConditionWidget(scenario->compoundCondition(), this));
    }
    qDeleteAll(*conditions);
    delete conditions;

    newCondition->registerCreators(widgets);
    newCondition->newCondition();

    delete newCondition;
}


void ContextViewPrivate::displayScenarioPrivate(Scenario *scenario)
{
//  ActionCollection *actionCollection = scenario->actionCollection();

//  conditionsProxy->setSourceModel((QAbstractItemModel*) actionCollection->getProxy());

//  ui.lvConditions->setCurrentIndex(conditionsProxy->index(0,0));

  CompoundCondition *compoundCondition = scenario->compoundCondition();

  conditionsProxy->setSourceModel((QAbstractItemModel*) compoundCondition->getProxy());

  ui.lvConditions->setCurrentIndex(conditionsProxy->index(0,0));
}

ContextViewPrivate::~ContextViewPrivate()
{
}
