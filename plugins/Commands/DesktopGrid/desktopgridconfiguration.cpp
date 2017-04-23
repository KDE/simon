/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "desktopgridconfiguration.h"
#include "desktopgridcommandmanager.h"
#include <QVariantList>
#include <KPluginFactory>
#include <QDebug>

K_PLUGIN_FACTORY_DECLARATION(DesktopGridPluginFactory)

DesktopGridConfiguration::DesktopGridConfiguration(Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent, "desktopgrid", i18n( "Desktop Grid" ),
"0.1", i18n("Voice controlled mouse clicks"),
"games-config-board")
{
  Q_UNUSED(args);
  ui.setupUi(this);

  QObject::connect(ui.cbUseRealTransparency, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
  QObject::connect(ui.rbActionAsk, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
  QObject::connect(ui.rbActionDefault, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
  QObject::connect(ui.rbActionAskAndDefault, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
  QObject::connect(ui.sbActionDefaultTimeout, SIGNAL(valueChanged(double)), this, SLOT(slotChanged()));
  QObject::connect(ui.cbDefaultClickMode, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
}


bool DesktopGridConfiguration::useRealTransparency()
{
  return ui.cbUseRealTransparency->isChecked();
}


DesktopGridConfiguration::ActionSelection DesktopGridConfiguration::actionSelection()
{
  ActionSelection actionSel;
  if (ui.rbActionAsk->isChecked())
    actionSel = AlwaysAsk;
  else if (ui.rbActionDefault->isChecked())
    actionSel = UseDefault;
  else actionSel = AskButDefaultAfterTimeout;

  return actionSel;
}


EventSimulation::ClickMode DesktopGridConfiguration::clickMode()
{
  switch (ui.cbDefaultClickMode->currentIndex()) {
    case 0:
      return EventSimulation::LMB;
    case 1:
      return EventSimulation::LMBDouble;
    case 2:
      return EventSimulation::RMB;
    case 3:
      return EventSimulation::MMB;
  }
  //default
  return EventSimulation::LMB;
}


void DesktopGridConfiguration::setActionSelection(ActionSelection actionSel)
{
  switch (actionSel) {
    case AlwaysAsk:
      ui.rbActionAsk->click();
      break;
    case UseDefault:
      ui.rbActionDefault->click();
      break;
    case AskButDefaultAfterTimeout:
      ui.rbActionAskAndDefault->click();
      break;
  }
}


int DesktopGridConfiguration::askTimeout()
{
  double timeoutD = ui.sbActionDefaultTimeout->value();
  return qRound(timeoutD*1000.0f);
}


void DesktopGridConfiguration::setClickMode(EventSimulation::ClickMode actionM)
{
  switch (actionM) {
    case EventSimulation::LMB:
      ui.cbDefaultClickMode->setCurrentIndex(0);
      break;
    case EventSimulation::LMBDouble:
      ui.cbDefaultClickMode->setCurrentIndex(1);
      break;
    case EventSimulation::RMB:
      ui.cbDefaultClickMode->setCurrentIndex(2);
      break;
    case EventSimulation::MMB:
      ui.cbDefaultClickMode->setCurrentIndex(3);
      break;
    default:
      qDebug() << "Invalid default action mode: " << actionM;
  }
}


bool DesktopGridConfiguration::deSerialize(const QDomElement& elem)
{
  QDomElement realTransElem = elem.firstChildElement("realTransparency");
  bool ok;
  int realTransparency = realTransElem.text().toInt(&ok);
  if (!ok) {
    defaults();
    return true;
  }

  ui.cbUseRealTransparency->setChecked(realTransparency);

  QDomElement actionSelectElem = elem.firstChildElement("actionSelect");
  setActionSelection((ActionSelection) actionSelectElem.text().toInt());

  QDomElement actionSelectTimeoutElem = elem.firstChildElement("askAndDefaultTimeout");
  ui.sbActionDefaultTimeout->setValue(actionSelectTimeoutElem.text().toFloat());

  QDomElement clickModeElem = elem.firstChildElement("clickMode");
  EventSimulation::ClickMode actionM = (EventSimulation::ClickMode) clickModeElem.text().toInt();
  setClickMode(actionM);
  return true;
}


QDomElement DesktopGridConfiguration::serialize(QDomDocument *doc)
{
  QDomElement configElem = doc->createElement("config");
  QDomElement realTransElem = doc->createElement("realTransparency");
  realTransElem.appendChild(doc->createTextNode(QString::number(ui.cbUseRealTransparency->isChecked() ? 1 : 0)));
  configElem.appendChild(realTransElem);

  ActionSelection actionSel = actionSelection();
  QDomElement actionSelectElem = doc->createElement("actionSelect");
  actionSelectElem.appendChild(doc->createTextNode(QString::number((int) actionSel)));
  configElem.appendChild(actionSelectElem);

  QDomElement actionSelectTimeoutElem = doc->createElement("askAndDefaultTimeout");
  actionSelectTimeoutElem.appendChild(doc->createTextNode(QString::number(ui.sbActionDefaultTimeout->value())));
  configElem.appendChild(actionSelectTimeoutElem);

  QDomElement clickModeElem = doc->createElement("clickMode");
  clickModeElem.appendChild(doc->createTextNode(QString::number((int) clickMode())));
  configElem.appendChild(clickModeElem);

  return configElem;
}


void DesktopGridConfiguration::defaults()
{
  qDebug() << "Defaults...";
  ui.cbUseRealTransparency->setChecked(true);
  ui.rbActionDefault->click();
  ui.sbActionDefaultTimeout->setValue(12);
  ui.cbDefaultClickMode->setCurrentIndex(0);
}


DesktopGridConfiguration::~DesktopGridConfiguration()
{

}
