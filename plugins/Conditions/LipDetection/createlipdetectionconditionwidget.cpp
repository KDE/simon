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
#include "simonvision/lipanalyzer.h"


CreateLipDetectionConditionWidget::CreateLipDetectionConditionWidget(QWidget *parent) : CreateConditionWidget(parent)
{
  analyzer=0;
  thresholdValue=5000;
  ui.setupUi(this);
  setWindowTitle(i18n("Lip Detection"));
  setWindowIcon(KIcon(""));
  connect(ui.pbTraining,SIGNAL(clicked(bool)),this,SLOT(modify()));
  connect(ui.horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(displaySliderValue(int)));

}


void CreateLipDetectionConditionWidget::displaySliderValue(int value)
{
  ui.lblSilderValue->setText(QString::number(value));
}

void CreateLipDetectionConditionWidget::modify()
{
  if(ui.pbTraining->text()=="&Start Training"||ui.pbTraining->text()=="&Train Again")
  {
    ui.pbTraining->setText("Stop Training");
    emit completeChanged();
    count=0;
    analyzer = new LipAnalyzer(thresholdValue);
    connect(analyzer,SIGNAL(lipMovementChanged(bool,int)),this,SLOT(calculateThreshold(bool,int)));
    ui.lblNotification->setText("Failed to initialize video capture!");
  }
  else
  {
    if(analyzer)
    {
      delete analyzer;
      analyzer=0;
    }
    if(thresholdValue>5000)
      ui.horizontalSlider->setValue((thresholdValue-35000)/250);
    ui.pbTraining->setText("Train Again");
    emit completeChanged();
  }
}
void CreateLipDetectionConditionWidget::calculateThreshold(bool isSpeaking, int value)
{
  if(isSpeaking)
  {
    ui.lblNotification->setText("Lip detected!");
    thresholdValue=(thresholdValue*count+value)/(++count);
  }
  else
  {
    ui.lblNotification->setText("Lip not detected! Try adjusting yourself to the webcam");
  }
}

bool CreateLipDetectionConditionWidget::isComplete()
{
  if (ui.pbTraining->text()=="Train Again")
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
  if(analyzer)
  {
    delete analyzer;
  }
}
