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

#include "newassociationcondition.h"

#include <simoncontextdetection/condition.h>
#include <simoncontextdetection/createconditionwidget.h>

#include <KDE/KUrl>
#include <KDE/KMessageBox>
#include <KDE/KKeySequenceWidget>
#include <KDE/KDialogButtonBox>

NewAssociationCondition::NewAssociationCondition(QWidget* parent) : KDialog(parent),
m_conditionCreators(0)
{
  QWidget *widget = new QWidget( this );
  ui.setupUi(widget);

  this->move(this->x() + 10, this->y() + 10);

  ui.swConditionCreators->removeWidget(ui.swConditionCreators->currentWidget());

  setMainWidget( widget );
  setCaption( i18n("Association Condition") );

  checkIfComplete();
}


void NewAssociationCondition::deleteLater()
{
  QObject::deleteLater();
}


bool NewAssociationCondition::registerCreators(QList<CreateConditionWidget *> *conditionCreators)
{
  if (m_conditionCreators) {
    qDeleteAll(*m_conditionCreators);
    delete m_conditionCreators;
  }

  foreach (CreateConditionWidget *widget, *conditionCreators) {
    ui.cbType->addItem(widget->windowIcon(), widget->windowTitle());
    ui.swConditionCreators->addWidget(widget);
    connect(widget, SIGNAL(completeChanged()), this, SLOT(checkIfComplete()));
  }

  m_conditionCreators = conditionCreators;
  return true;
}


void NewAssociationCondition::conditionSuggested(Condition *condition)
{
  if (!condition) return;
  init(condition);
  delete condition;
}

void NewAssociationCondition::init(Condition *condition)
{
  if (!condition) return;

  bool found=false;
  int i=0;
  foreach (CreateConditionWidget *widget, *m_conditionCreators) {
    if (widget->isInstanceOfSameCondition(condition)) {
      widget->init(condition);
      found=true;
      ui.cbType->setCurrentIndex(i);
      break;
    }
    i++;
  }
  if (!found)
    KMessageBox::error(this, i18n("Could not determine Condition Type"));

  checkIfComplete();
}


void NewAssociationCondition::checkIfComplete()
{
  CreateConditionWidget *creator = dynamic_cast<CreateConditionWidget*>(ui.swConditionCreators->currentWidget());
  Q_ASSERT(creator);

  bool complete;
  if (!creator)
    complete = false;
  else
    complete = creator->isComplete();

  enableButtonOk(complete);
}

bool NewAssociationCondition::newAssociationCondition()
{
  if (ui.swConditionCreators->count() == 0) {
    KMessageBox::information(this, i18n("No condition plugins loaded that provide condition engines."));
    return false;
  }

  if (KDialog::exec()) {
    //creating
    CreateConditionWidget *creator = dynamic_cast<CreateConditionWidget*>(ui.swConditionCreators->currentWidget());
    kDebug() << "Creating with creator: " << creator;
    Q_ASSERT(creator);

    if (!creator) return false;

    return creator->addCondition();
  }
  return false;
}


NewAssociationCondition::~NewAssociationCondition()
{
  if (m_conditionCreators) {
    qDeleteAll(*m_conditionCreators);
    delete m_conditionCreators;
  }
}
