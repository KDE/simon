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

#include "facedetectioncondition.h"
#include <KDebug>
#include <KPluginFactory>
#include "createfacedetectionconditionwidget.h"
#include <QWidget>
#include "simonvision/faceanalyzer.h"
#include <highgui.h>
#include <cv.h>
K_PLUGIN_FACTORY(FaceDetectionPluginFactory,
                 registerPlugin< FaceDetectionCondition >();
                )

K_EXPORT_PLUGIN(FaceDetectionPluginFactory("simonfacedetectioncondition"))

FaceDetectionCondition::FaceDetectionCondition(QObject *parent, const QVariantList &args) :
    Condition(parent, args)
{
  m_pluginName = "simonfacedetectionconditionplugin.desktop";
}

CreateConditionWidget* FaceDetectionCondition::getCreateConditionWidget(QWidget* parent)
{
  return new CreateFaceDetectionConditionWidget(parent);
}

QDomElement FaceDetectionCondition::privateSerialize(QDomDocument *doc, QDomElement elem)
{
  Q_UNUSED(doc);

  return elem;
}

QString FaceDetectionCondition::name()
{
  if (!isInverted())
    return i18nc("Detecting the presence of user from the webcam", "Presence of user");
  else
    return i18nc("Detecting the presence of user from the webcam", "Absence of user");
}

bool FaceDetectionCondition::privateDeSerialize(QDomElement elem)
{
  Q_UNUSED(elem);
  analyzer = new FaceAnalyzer();
  connect(analyzer,SIGNAL(facePresenceChanged(bool)),this,SLOT(manageConditionState(bool)));
  return true;
}

void FaceDetectionCondition::manageConditionState(bool hasFace)
{
  if (hasFace)
  {
    if(!m_satisfied)
    {
      m_satisfied = true;
      kDebug() << name() + " is true!";
      emit conditionChanged();
    }

  }
  else
  {
    if(m_satisfied)
    {
      m_satisfied = false;
      kDebug() << name() + " is false!";
      emit conditionChanged();
    }
  }

}  

FaceDetectionCondition::~FaceDetectionCondition()
{
  delete analyzer;
}
