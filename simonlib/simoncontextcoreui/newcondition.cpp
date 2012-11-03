/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#include "newcondition.h"
#include "ui_modifycondition.h"

#include <simoncontextdetection/condition.h>
#include <simoncontextdetection/createconditionwidget.h>

#include <KDE/KUrl>
#include <KDE/KMessageBox>
#include <KDE/KKeySequenceWidget>
#include <KDE/KDialogButtonBox>

NewCondition::NewCondition(QWidget* parent) : KDialog(parent), ui(new Ui::DlgModifyCondition)
{
  QWidget *widget = new QWidget( this );
  ui->setupUi(widget);

  ui->swConditionCreators->removeWidget(ui->swConditionCreators->currentWidget());

  setMainWidget( widget );
  setCaption( i18n("Condition") );
  
  connect(ui->cbType, SIGNAL(currentIndexChanged(int)), this, SLOT(checkIfComplete()));
}


void NewCondition::deleteLater()
{
  QObject::deleteLater();
}

bool sortCreateConditionHelper(const CreateConditionWidget* a, const CreateConditionWidget* b)
{
  return a->windowTitle() < b->windowTitle();
}

bool NewCondition::registerCreators(QList<CreateConditionWidget *> conditionCreators)
{
  qDeleteAll(m_conditionCreators);
 
  qSort(conditionCreators.begin(), conditionCreators.end(), sortCreateConditionHelper);
  
  foreach (CreateConditionWidget *widget, conditionCreators) {
    ui->cbType->addItem(widget->windowIcon(), widget->windowTitle());
    ui->swConditionCreators->addWidget(widget);
    connect(widget, SIGNAL(completeChanged()), this, SLOT(checkIfComplete()));
  }

  m_conditionCreators = conditionCreators;
  checkIfComplete();
  return true;
}


void NewCondition::conditionSuggested(Condition *condition)
{
  if (!condition) return;
  
  init(condition);
  delete condition;
}

void NewCondition::init(Condition *condition)
{
  if (!condition) return;

  ui->cbInverted->setChecked(condition->isInverted());
  
  bool found=false;
  int i=0;
  foreach (CreateConditionWidget *widget, m_conditionCreators) {
    if (widget->init(condition)) {
      found=true;
      ui->cbType->setCurrentIndex(i);
//       break; Init to all conditions that match
    }
    i++;
  }
  if (!found)
    KMessageBox::error(this, i18n("Could not determine condition Type"));

  checkIfComplete();
}


void NewCondition::checkIfComplete()
{
  CreateConditionWidget *creator = dynamic_cast<CreateConditionWidget*>(ui->swConditionCreators->currentWidget());

  bool complete;
  if (!creator)
    complete = false;
  else
    complete = creator->isComplete();

  enableButtonOk(complete);
}

Condition* NewCondition::newCondition()
{
  if (ui->swConditionCreators->count() == 0) {
    KMessageBox::information(this, i18n("No condition plugins loaded that provide condition engines."));
    return 0;
  }

  if (KDialog::exec()) {
    //creating
    CreateConditionWidget *creator = dynamic_cast<CreateConditionWidget*>(ui->swConditionCreators->currentWidget());
    kDebug() << "Creating with creator: " << creator;
    Q_ASSERT(creator);

    if (!creator) return 0;

    
    QDomDocument doc;
    QDomElement conditionElem = doc.createElement("condition");

    QDomElement invertElem = doc.createElement("inverted");
    invertElem.appendChild(doc.createTextNode(ui->cbInverted->isChecked() ? "1" : "0"));
    conditionElem.appendChild(invertElem);
    
    return creator->createCondition(&doc, conditionElem);
  }
  return 0;
}


NewCondition::~NewCondition()
{
  qDeleteAll(m_conditionCreators);
}
