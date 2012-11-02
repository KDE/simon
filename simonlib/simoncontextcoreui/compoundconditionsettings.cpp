/*
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
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


#include "compoundconditionsettings.h"
#include "newcondition.h"
#include "ui_compoundconditionsettings.h"
#include <simoncontextdetection/condition.h>
#include <simoncontextdetection/contextmanager.h>
#include <simoncontextdetection/compoundcondition.h>
#include <QSortFilterProxyModel>
#include <KMessageBox>
#include <KDebug>

CompoundConditionSettings::CompoundConditionSettings ( QWidget* parent, Qt::WindowFlags f ) : QWidget(parent, f),
    ui(new Ui::CompoundConditionSettings()),
    m_conditions(0)
{
  ui->setupUi(this);
  connect ( ui->pbNewCondition, SIGNAL(clicked()), this, SLOT(addCondition()));
  connect ( ui->pbEditCondition, SIGNAL(clicked()), this, SLOT(editCondition()));
  connect ( ui->pbDeleteCondition, SIGNAL(clicked()), this, SLOT(deleteCondition()));

  ui->lvConditions->setIconSize(QSize(24,24));
  ui->lvConditions->setSpacing(2);

  ui->pbNewCondition->setIcon(KIcon("list-add"));
  ui->pbEditCondition->setIcon(KIcon("edit-rename"));
  ui->pbDeleteCondition->setIcon(KIcon("edit-delete"));

  conditionsProxy = new QSortFilterProxyModel(this);
  conditionsProxy->setFilterKeyColumn(0);
  conditionsProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  ui->lvConditions->setModel(conditionsProxy);

  connect(ui->leConditionsFilter, SIGNAL(textChanged(QString)), conditionsProxy, SLOT(setFilterRegExp(QString)));

  connect(ui->lvConditions->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(selectionChanged()));
}

void CompoundConditionSettings::setConditions ( CompoundCondition* c )
{
  m_conditions = c;
  conditionsProxy->setSourceModel(c);
  ui->pbNewCondition->setEnabled(c);
  ui->lvConditions->setCurrentIndex(conditionsProxy->index(0,0));
  selectionChanged();
}

Condition* CompoundConditionSettings::getCurrentCondition()
{
    if (!m_conditions) return 0;
    
    QModelIndex index = conditionsProxy->mapToSource(ui->lvConditions->currentIndex());
    if (!index.isValid()) return 0;

    return static_cast<Condition*>(index.internalPointer());
}

void CompoundConditionSettings::addCondition()
{
    NewCondition *newCondition = new NewCondition(this);
    QList<CreateConditionWidget*> widgets;
    QList<Condition*> conditions;
    ContextManager* manager = ContextManager::instance();

    conditions = manager->getConditions();

    foreach (Condition* condition, conditions)
        widgets.push_back(condition->getCreateConditionWidget(this));

    newCondition->registerCreators(widgets);
    Condition *c = newCondition->newCondition();
    if (c)
      m_conditions->addCondition(c);

    delete newCondition;
}

void CompoundConditionSettings::editCondition()
{
    //get the condition to edit
    Condition* condition = getCurrentCondition();
    if (!condition)
        return;

    //get the CreateConditionWidgets
    QList<CreateConditionWidget*> widgets;
    QList<Condition*> conditions;
    ContextManager* manager = ContextManager::instance();

    conditions = manager->getConditions();

    foreach (Condition* c, conditions)
    {
        widgets.push_back(c->getCreateConditionWidget(this));
    }

    //prepare the edit condition dialog and launch it
    NewCondition *editCondition = new NewCondition(this);
    editCondition->registerCreators(widgets);
    editCondition->init(condition);
    Condition *c = editCondition->newCondition();

    //on confirmation of the edit, the old condition is deleted and the new one made by the NewCondition replaces it
    if (c)
    {
      m_conditions->removeCondition(condition);
      m_conditions->addCondition(c);
      ui->lvConditions->setCurrentIndex(conditionsProxy->index(conditionsProxy->rowCount()-1, 0));
    }
    delete editCondition;
}

void CompoundConditionSettings::deleteCondition()
{
    Condition* condition = getCurrentCondition();

    if (!condition)
        return;

    if (KMessageBox::questionYesNoCancel(this, i18nc("%1 is condition name", "Are you sure that you want to irreversibly remove the condition \"%1\"?", condition->name()), i18n("Remove Condition")) == KMessageBox::Yes)
    {
        if (!m_conditions->removeCondition(condition))
        {
            kDebug() << "Error removing condition!";
        }
    }
}

void CompoundConditionSettings::selectionChanged()
{
    if (getCurrentCondition() == 0)
    {
        ui->pbDeleteCondition->setEnabled(false);
        ui->pbEditCondition->setEnabled(false);
    }
    else
    {
        ui->pbDeleteCondition->setEnabled(true);
        ui->pbEditCondition->setEnabled(true);
    }
}