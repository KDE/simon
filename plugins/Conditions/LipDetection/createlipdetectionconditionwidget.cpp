/*
 *   Copyright (C) 2012 Yash Shah <blazonware@gmail.com>
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

#include "createlipdetectionconditionwidget.h"
#include "lipdetectioncondition.h"
#include "simoncontextdetection/contextmanager.h"



CreateLipDetectionConditionWidget::CreateLipDetectionConditionWidget(QWidget *parent) : CreateConditionWidget(parent)
{
  ui.setupUi(this);
  ui.groupBox->setVisible(false);
  setWindowTitle(i18n("Lip Detection"));
  setWindowIcon(KIcon(""));
  connect(ui.pbOk,SIGNAL(clicked(bool)),this,SLOT(modify()));
  connect(ui.horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(displaySliderValue(int)));
  connect(ui.pbSelect,SIGNAL(clicked(bool)),this,SLOT(select()));
//  connect(ui.cbInverted, SIGNAL(stateChanged(int)), this, SIGNAL(completeChanged()));
}

void CreateLipDetectionConditionWidget::select()
{
  ui.groupBox->setVisible(true);

}

void CreateLipDetectionConditionWidget::displaySliderValue(int value)
{
  ui.lblSilderValue->setText(QString::number(value));
}

void CreateLipDetectionConditionWidget::modify()
{
  thresholdValue = ui.horizontalSlider->value();
  ui.pbSelect->setText("Train again");
  ui.groupBox->setVisible(false);
  emit completeChanged();
}


bool CreateLipDetectionConditionWidget::isComplete()
{
  if (ui.pbSelect->text()=="Train again")
    return true;
  else
    return false;
}

bool CreateLipDetectionConditionWidget::init(Condition *condition)
{
  Q_ASSERT(condition);

  LipDetectionCondition *lipDetectionCondition = dynamic_cast<LipDetectionCondition*>(condition);

  if (!lipDetectionCondition) return false;

  return true;
}


Condition* CreateLipDetectionConditionWidget::createCondition(QDomDocument* doc, QDomElement& conditionElem)
{
  conditionElem.setAttribute("name", "simonlipdetectionconditionplugin.desktop");

  QDomElement thresholdElem = doc->createElement("thresholdvalue");
  thresholdElem.appendChild(doc->createTextNode(QString::number(ui.horizontalSlider->value())));
  conditionElem.appendChild(thresholdElem);

  return ContextManager::instance()->getCondition(conditionElem);
}


CreateLipDetectionConditionWidget::~CreateLipDetectionConditionWidget()
{
}
