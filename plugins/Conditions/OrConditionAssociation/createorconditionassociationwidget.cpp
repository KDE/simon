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

#include "createorconditionassociationwidget.h"
#include "orconditionassociation.h"
#include "newassociationcondition.h"

#include <simoncontextdetection/contextmanager.h>

#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <KMessageBox>


CreateOrConditionAssociationWidget::CreateOrConditionAssociationWidget(CompoundCondition *compoundCondition,
QWidget *parent) : CreateConditionWidget(compoundCondition, parent)
{
  ui.setupUi(this);

  //setWindowIcon();
  setWindowTitle("Or Condition Association");

  //initialize compound association condition
  QDomDocument * doc = new QDomDocument();
  m_compoundAssociationCondition = CompoundCondition::createInstance(CompoundCondition::createEmpty(doc));
  delete doc;

  ui.pbNewCondition->setIcon(KIcon("list-add"));
  ui.pbEditCondition->setIcon(KIcon("edit-rename"));
  ui.pbDeleteCondition->setIcon(KIcon("edit-delete"));

  m_conditionsProxy = new QSortFilterProxyModel(this);
  m_conditionsProxy->setFilterKeyColumn(0);
  m_conditionsProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  ui.lvConditions->setModel(m_conditionsProxy);

  m_conditionsProxy->setSourceModel((QAbstractItemModel*) m_compoundAssociationCondition->getProxy());

  connect(ui.pbNewCondition, SIGNAL(clicked()), this, SLOT(addAssociationCondition()));
  connect(ui.pbEditCondition, SIGNAL(clicked()), this, SLOT(editAssociationCondition()));
  connect(ui.pbDeleteCondition, SIGNAL(clicked()), this, SLOT(deleteAssociationCondition()));

  connect(ui.lvConditions->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(selectionChanged()));
}

CreateOrConditionAssociationWidget::~CreateOrConditionAssociationWidget()
{
    m_compoundAssociationCondition->deleteLater();
}

bool CreateOrConditionAssociationWidget::isComplete()
{
  return (m_compoundAssociationCondition->getConditions().count() > 1);
}

bool CreateOrConditionAssociationWidget::init(Condition *condition)
{
  Q_ASSERT(condition);

    kDebug() << "Initializing the create or condition association widget";

  OrConditionAssociation *orConditionAssociation = dynamic_cast<OrConditionAssociation*>(condition);
  if (!orConditionAssociation) return false;

  m_compoundAssociationCondition->deleteLater();

  QDomDocument * doc = new QDomDocument();
  m_compoundAssociationCondition = CompoundCondition::createInstance(CompoundCondition::createEmpty(doc));
  delete doc;

  QList<Condition*> associationConditions = orConditionAssociation->getConditions();

  foreach(Condition* associationCondition, associationConditions)
  {
      m_compoundAssociationCondition->addCondition(associationCondition);
  }

  m_conditionsProxy->setSourceModel((QAbstractItemModel*) m_compoundAssociationCondition->getProxy());
  ui.lvConditions->setCurrentIndex(m_conditionsProxy->index(0,0));

  return true;
}

Condition* CreateOrConditionAssociationWidget::createCondition()
{
    kDebug() << "Creating Or Condition Association";
    QDomDocument doc;
    QDomElement conditionElem = doc.createElement("condition");
    conditionElem.setAttribute("name", "simonorconditionassociationplugin.desktop");

    QDomElement invertElem = doc.createElement("inverted");
    invertElem.appendChild(doc.createTextNode("0"));
    conditionElem.appendChild(invertElem);

    //add each condition
    QList<Condition*> associationConditions = m_compoundAssociationCondition->getConditions();
    foreach(Condition* associationCondition, associationConditions)
    {
        conditionElem.appendChild(associationCondition->serialize(&doc));
    }

    ContextManager* manager = ContextManager::instance();

    return manager->getCondition(conditionElem);
}

Condition* CreateOrConditionAssociationWidget::getCurrentCondition()
{
    QModelIndex index = m_conditionsProxy->mapToSource(ui.lvConditions->currentIndex());
    if (!index.isValid()) return 0;

    return static_cast<Condition*>(index.internalPointer());
}


void CreateOrConditionAssociationWidget::addAssociationCondition()
{
    NewAssociationCondition *associationConditionDlg = new NewAssociationCondition(this);
    QList<CreateConditionWidget*>* widgets = new QList<CreateConditionWidget*>();
    QList<Condition*>* conditions;
    ContextManager* manager = ContextManager::instance();

    conditions = manager->getConditions();

    foreach (Condition* condition, *conditions)
    {
        widgets->push_back(condition->getCreateConditionWidget(m_compoundAssociationCondition, this));
    }
    delete conditions;

    associationConditionDlg->registerCreators(widgets);
    associationConditionDlg->newAssociationCondition();
    delete associationConditionDlg;

    ui.lvConditions->setCurrentIndex(m_conditionsProxy->index(0,0));
    emit completeChanged();
}

void CreateOrConditionAssociationWidget::editAssociationCondition()
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
        widgets->push_back(c->getCreateConditionWidget(m_compoundAssociationCondition, this));
    }
    delete conditions;

    //prepare the edit condition dialog and launch it
    NewAssociationCondition *editCondition = new NewAssociationCondition(this);
    editCondition->registerCreators(widgets);
    editCondition->init(condition);
    bool succ = editCondition->newAssociationCondition();

    //on confirmation of the edit, the old condition is deleted and the new one made by the NewCondition replaces it
    if (succ)
    {
      m_compoundAssociationCondition->removeCondition(condition);
      ui.lvConditions->setCurrentIndex(m_conditionsProxy->index(m_conditionsProxy->rowCount()-1, 0));
    }
    delete editCondition;
}

void CreateOrConditionAssociationWidget::deleteAssociationCondition()
{
    Condition* condition = getCurrentCondition();

    if (!condition)
        return;

    if (KMessageBox::questionYesNoCancel(this, i18n("Are you sure that you want to irreversibly remove the condition \"%1\"?", condition->name()), i18n("Remove Condition")) == KMessageBox::Yes)
    {
        if (!m_compoundAssociationCondition->removeCondition(condition))
        {
            kDebug() << "Error removing condition!";
        }
    }

    emit completeChanged();
}

void CreateOrConditionAssociationWidget::selectionChanged()
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
