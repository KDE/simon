/*
 *   Copyright (C) 2012 Yash Shah <mail@yashshah.com>
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
#include "simonvision/lipanalyzer.h"
#include <KDebug>


CreateLipDetectionConditionWidget::CreateLipDetectionConditionWidget(QWidget *parent) : CreateConditionWidget(parent)
{
  analyzer=0;
  ui.setupUi(this);
  setWindowTitle(i18n("Lip Detection"));
  ui.pbTraining->setText(i18n("&Start Training"));
  connect(ui.pbTraining,SIGNAL(clicked(bool)),this,SLOT(modify()));
  connect(ui.horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(displaySliderValue(int)));
}

bool CreateLipDetectionConditionWidget::isInstanceOfSameCondition(Condition* condition)
{
  return dynamic_cast<LipDetectionCondition*>(condition);
}

void CreateLipDetectionConditionWidget::displaySliderValue(int value)
{
  ui.lblSilderValue->setText(QString::number(value));
  if (analyzer)
    analyzer->setThreshold(value);
}

void CreateLipDetectionConditionWidget::modify()
{
  if(ui.pbTraining->text()==i18n("&Start Training"))
  {
    ui.pbTraining->setText("Stop Training");
    count=0;
    analyzer = new LipAnalyzer(ui.horizontalSlider->value());
    connect(analyzer,SIGNAL(lipMovementChanged(bool,int)),this,SLOT(calculateThreshold(bool,int)));
    ui.lblNotification->setText(i18n("Failed to initialize video capture."));
  }
  else
  {
    ui.pbTraining->setText(i18n("&Start Training"));
    LipAnalyzer *t = analyzer;
    analyzer=0;
    delete t;
  }
}

void CreateLipDetectionConditionWidget::calculateThreshold(bool isSpeaking, int value)
{
  Q_UNUSED(value);
  kDebug() << "Value before: " << ui.horizontalSlider->value();
  if(isSpeaking)
  {
    ui.lblNotification->setText(i18n("Speaking."));
//     thresholdValue=(thresholdValue * count + value)/(++count);
    ui.horizontalSlider->setValue(ui.horizontalSlider->value() + 500);
  }
  else
  {
    ui.lblNotification->setText(i18n("Not speaking."));
    // constant decay because we know that the user should still be speaking at this point
    ui.horizontalSlider->setValue(ui.horizontalSlider->value() - 1000);
  }
  kDebug() << "Value after: " << ui.horizontalSlider->value();
//   displaySliderValue(thresholdValue);
}

bool CreateLipDetectionConditionWidget::isComplete()
{
  return true; // training is not a requirement
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
  delete analyzer;
}
