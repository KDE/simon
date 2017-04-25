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
#include <simoncontextcoreui/newcondition.h>

#include <simoncontextdetection/contextmanager.h>

#include <QSortFilterProxyModel>
#include <KMessageBox>


CreateOrConditionAssociationWidget::CreateOrConditionAssociationWidget(QWidget *parent) : CreateConditionWidget(parent)
{
  ui.setupUi(this);

  setWindowTitle("Or Condition Association");
  setWindowIcon(QIcon::fromTheme("view-choose"));

  //initialize compound association condition
  QDomDocument * doc = new QDomDocument();
  m_compoundAssociationCondition = CompoundCondition::createInstance(CompoundCondition::createEmpty(doc));
  delete doc;

  ui.pbNewCondition->setIcon(QIcon::fromTheme("list-add"));
  ui.pbEditCondition->setIcon(QIcon::fromTheme("edit-rename"));
  ui.pbDeleteCondition->setIcon(QIcon::fromTheme("edit-delete"));

  m_conditionsProxy = new QSortFilterProxyModel(this);
  m_conditionsProxy->setFilterKeyColumn(0);
  m_conditionsProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  ui.lvConditions->setModel(m_conditionsProxy);

  m_conditionsProxy->setSourceModel(m_compoundAssociationCondition);

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

  OrConditionAssociation *orConditionAssociation = dynamic_cast<OrConditionAssociation*>(condition);
  if (!orConditionAssociation) return false;

  m_compoundAssociationCondition->deleteLater();

  QDomDocument * doc = new QDomDocument();
  m_compoundAssociationCondition = CompoundCondition::createInstance(CompoundCondition::createEmpty(doc));
  delete doc;

  QList<Condition*> associationConditions = orConditionAssociation->getConditions();

  foreach(Condition* associationCondition, associationConditions)
  {
      ContextManager::instance()->refCondition(associationCondition);
      m_compoundAssociationCondition->addCondition(associationCondition);
  }

  m_conditionsProxy->setSourceModel(m_compoundAssociationCondition);
  ui.lvConditions->setCurrentIndex(m_conditionsProxy->index(0,0));

  return true;
}

Condition* CreateOrConditionAssociationWidget::createCondition(QDomDocument* doc, QDomElement& conditionElem)
{
    conditionElem.setAttribute("name", "simonorconditionassociationplugin.desktop");

    //add each condition
    QList<Condition*> associationConditions = m_compoundAssociationCondition->getConditions();
    foreach(Condition* associationCondition, associationConditions)
    {
        conditionElem.appendChild(associationCondition->serialize(doc));
    }

    return ContextManager::instance()->getCondition(conditionElem);
}

Condition* CreateOrConditionAssociationWidget::getCurrentCondition()
{
    QModelIndex index = m_conditionsProxy->mapToSource(ui.lvConditions->currentIndex());
    if (!index.isValid()) return 0;

    return static_cast<Condition*>(index.internalPointer());
}


void CreateOrConditionAssociationWidget::addAssociationCondition()
{
    NewCondition *associationConditionDlg = new NewCondition(this);
    QList<CreateConditionWidget*> widgets;
    QList<Condition*> conditions;
    ContextManager* manager = ContextManager::instance();

    conditions = manager->getConditions();

    foreach (Condition* condition, conditions)
    {
        widgets.push_back(condition->getCreateConditionWidget(this));
    }

    associationConditionDlg->registerCreators(widgets);
    Condition *c = associationConditionDlg->newCondition();
    delete associationConditionDlg;
    if (c)
      m_compoundAssociationCondition->addCondition(c);

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
      m_compoundAssociationCondition->removeCondition(condition);
      ui.lvConditions->setCurrentIndex(m_conditionsProxy->index(m_conditionsProxy->rowCount()-1, 0));
      m_compoundAssociationCondition->addCondition(c);
    }
    delete editCondition;
}

void CreateOrConditionAssociationWidget::deleteAssociationCondition()
{
    Condition* condition = getCurrentCondition();

    if (!condition)
        return;

    if (KMessageBox::questionYesNoCancel(this, i18nc("%1 is the conditions name", 
                                         "Are you sure that you want to irreversibly remove the condition \"%1\"?", condition->name()), i18n("Remove Condition")) == KMessageBox::Yes)
    {
        if (!m_compoundAssociationCondition->removeCondition(condition))
        {
            qDebug() << "Error removing condition!";
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
