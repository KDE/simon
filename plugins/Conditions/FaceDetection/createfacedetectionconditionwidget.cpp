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

#include "createfacedetectionconditionwidget.h"
#include "facedetectioncondition.h"
#include "simoncontextdetection/contextmanager.h"



CreateFaceDetectionConditionWidget::CreateFaceDetectionConditionWidget(QWidget *parent) : CreateConditionWidget(parent)
{
  ui.setupUi(this);

  setWindowTitle(i18n("Face Detection"));
  setWindowIcon(KIcon(""));
  emit completeChanged();
  this->completeChanged();
  
  connect(ui.cbInverted, SIGNAL(stateChanged(int)), this, SIGNAL(completeChanged()));
}


bool CreateFaceDetectionConditionWidget::isComplete()
{
  return true;
}

bool CreateFaceDetectionConditionWidget::init(Condition *condition)
{
  Q_ASSERT(condition);

  FaceDetectionCondition *faceDetectionCondition = dynamic_cast<FaceDetectionCondition*>(condition);

  if (!faceDetectionCondition) return false;

  ui.cbInverted->setChecked(faceDetectionCondition->isInverted());

  return true;
}


Condition* CreateFaceDetectionConditionWidget::createCondition()
{
  kDebug() << "Creating Face Detection Condition";
  QDomDocument doc;
  QDomElement conditionElem = doc.createElement("condition");
  conditionElem.setAttribute("name", "simonfacedetectionconditionplugin.desktop");

  QDomElement invertElem = doc.createElement("inverted");
  invertElem.appendChild(doc.createTextNode(ui.cbInverted->isChecked() ? "1" : "0"));
  conditionElem.appendChild(invertElem);


  ContextManager* manager = ContextManager::instance();

  return manager->getCondition(conditionElem);
}


CreateFaceDetectionConditionWidget::~CreateFaceDetectionConditionWidget()
{
}
