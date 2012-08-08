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

  setWindowTitle(i18n("Lip Detection"));
  setWindowIcon(KIcon(""));
  
  connect(ui.cbInverted, SIGNAL(stateChanged(int)), this, SIGNAL(completeChanged()));
}


bool CreateLipDetectionConditionWidget::isComplete()
{
  return true;
}

bool CreateLipDetectionConditionWidget::init(Condition *condition)
{
  Q_ASSERT(condition);

  LipDetectionCondition *lipDetectionCondition = dynamic_cast<LipDetectionCondition*>(condition);

  if (!lipDetectionCondition) return false;

  ui.cbInverted->setChecked(lipDetectionCondition->isInverted());

  return true;
}


Condition* CreateLipDetectionConditionWidget::createCondition()
{
  kDebug() << "Creating Lip Detection Condition";
  QDomDocument doc;
  QDomElement conditionElem = doc.createElement("condition");
  conditionElem.setAttribute("name", "simonlipdetectionconditionplugin.desktop");

  QDomElement invertElem = doc.createElement("inverted");
  invertElem.appendChild(doc.createTextNode(ui.cbInverted->isChecked() ? "1" : "0"));
  conditionElem.appendChild(invertElem);


  ContextManager* manager = ContextManager::instance();

  return manager->getCondition(conditionElem);
}


CreateLipDetectionConditionWidget::~CreateLipDetectionConditionWidget()
{
}
