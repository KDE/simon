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

#include "lipdetectioncondition.h"
#include <KDebug>
#include "createlipdetectionconditionwidget.h"
#include <QWidget>
#include "simonvision/lipanalyzer.h"
#include <highgui.h>
#include <cv.h>
K_PLUGIN_FACTORY(LipDetectionPluginFactory,
                 registerPlugin< LipDetectionCondition >();
                )

K_EXPORT_PLUGIN(LipDetectionPluginFactory("simonlipdetectioncondition"))

LipDetectionCondition::LipDetectionCondition(QObject *parent, const QVariantList &args) :
    Condition(parent, args)
{
  thresholdValue=0;
  m_pluginName = "simonlipdetectionconditionplugin.desktop";
}

CreateConditionWidget* LipDetectionCondition::getCreateConditionWidget(QWidget* parent)
{
  return new CreateLipDetectionConditionWidget(parent);
}

QDomElement LipDetectionCondition::privateSerialize(QDomDocument *doc, QDomElement elem)
{
 QDomElement thresholdValueElem = doc->createElement("thresholdvalue");
 thresholdValueElem.appendChild(doc->createTextNode(QString::number(thresholdValue)));

 elem.appendChild(thresholdValueElem);

  return elem;
}

QString LipDetectionCondition::name()
{
  if (!isInverted())
    return i18nc("Detecting Whether the user is speaking or not by detecting lip movements", "User is speaking");
  else
    return i18nc("Detecting Whether the user is speaking or not by detecting lip movements", "User is not speaking");
}

bool LipDetectionCondition::privateDeSerialize(QDomElement elem)
{
  QDomElement thresholdValueElement;

  thresholdValueElement = elem.firstChildElement("thresholdvalue");
  if (thresholdValueElement.isNull())
  {
    kDebug() << "Threhold value not specified!  Deserialization failure!";
    return false;
  }
  thresholdValue = thresholdValueElement.text().toInt();

  analyzer = new LipAnalyzer(thresholdValue);
  connect(analyzer,SIGNAL(lipMovementChanged(bool,int)),this,SLOT(manageConditionState(bool,int)));
  return true;
}

void LipDetectionCondition::manageConditionState(bool isSpeaking, int thresholdValue)
{
  Q_UNUSED(thresholdValue);
  if (isSpeaking)
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


LipDetectionCondition::~LipDetectionCondition()
{
  delete analyzer;

}
