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

#include "calculatorconfiguration.h"
#include "calculatorcommandmanager.h"
#include <QVariantList>
#include <KDELibs4Support/kgenericfactory.h>

K_PLUGIN_FACTORY_DECLARATION(CalculatorCommandPluginFactory)

CalculatorConfiguration::CalculatorConfiguration(Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent, "calculator", i18n( "Calculator" ),
"0.1", i18n("Calculate with your voice"),
"accessories-calculator")
{
  Q_UNUSED(args);
  ui.setupUi(this);

  connect(ui.cbControlMode, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
  connect(ui.rbOutputAsk, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
  connect(ui.rbOutputDefault, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
  connect(ui.rbAskAndDefault, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
  connect(ui.cbDefaultOutputMode, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
  connect(ui.sbOutputDefaultTimeout, SIGNAL(valueChanged(double)), this, SLOT(slotChanged()));
}


bool CalculatorConfiguration::deSerialize(const QDomElement& elem)
{
  bool ok;
  int outputModeSelection = elem.firstChildElement("outputModeSelection").text().toInt(&ok);
  if (!ok) outputModeSelection = 0;               //default;

  CalculatorConfiguration::OutputModeSelection modeSelection =
    (CalculatorConfiguration::OutputModeSelection) outputModeSelection;

  switch (modeSelection) {
    case CalculatorConfiguration::AlwaysAsk:
      ui.rbOutputAsk->setChecked(true);
      break;
    case CalculatorConfiguration::UseDefault:
      ui.rbOutputDefault->setChecked(true);
      break;
    case CalculatorConfiguration::AskButDefaultAfterTimeout:
      ui.rbAskAndDefault->setChecked(true);
      break;
  }

  ui.cbControlMode->setCurrentIndex(elem.firstChildElement("controlMode").text().toInt(&ok));

  int defaultOutputMode = elem.firstChildElement("defaultOutputMode").text().toInt(&ok);
  if (!ok) defaultOutputMode = 0;
  ui.cbDefaultOutputMode->setCurrentIndex(defaultOutputMode);

  int askTimeout = elem.firstChildElement("askTimeout").text().toFloat(&ok);
  if (!ok) askTimeout = 12;
  ui.sbOutputDefaultTimeout->setValue(askTimeout);

  emit changed(false);
  return true;
}


QDomElement CalculatorConfiguration::serialize(QDomDocument *doc)
{
  QDomElement configElem = doc->createElement("config");

  QDomElement controlModeElement = doc->createElement("controlMode");
  QDomElement outputModeSelectionElem = doc->createElement("outputModeSelection");
  QDomElement defaultOutputModeElem = doc->createElement("defaultOutputMode");
  QDomElement askTimeoutElem = doc->createElement("askTimeout");

  controlModeElement.appendChild(doc->createTextNode(QString::number(ui.cbControlMode->currentIndex())));
  outputModeSelectionElem.appendChild(doc->createTextNode(QString::number((int) outputModeSelection())));
  defaultOutputModeElem.appendChild(doc->createTextNode(QString::number((int) outputMode()-1)));
  askTimeoutElem.appendChild(doc->createTextNode(QString::number(ui.sbOutputDefaultTimeout->value())));

  configElem.appendChild(controlModeElement);
  configElem.appendChild(outputModeSelectionElem);
  configElem.appendChild(defaultOutputModeElem);
  configElem.appendChild(askTimeoutElem);

  return configElem;
}


void CalculatorConfiguration::defaults()
{
  ui.cbControlMode->setCurrentIndex(0);
  ui.rbOutputAsk->click();
  ui.sbOutputDefaultTimeout->setValue(12);
  ui.cbDefaultOutputMode->setCurrentIndex(0);
}


/**
 * \return timeout in milliseconds
 */
int CalculatorConfiguration::askTimeout()
{
  double timeoutD = ui.sbOutputDefaultTimeout->value();
  return qRound(timeoutD*1000.0f);
}


CalculatorConfiguration::ControlMode CalculatorConfiguration::controlMode()
{
  return (CalculatorConfiguration::ControlMode) (ui.cbControlMode->currentIndex()+1);
}


CalculatorConfiguration::OutputModeSelection CalculatorConfiguration::outputModeSelection()
{
  if (ui.rbOutputAsk->isChecked())
    return CalculatorConfiguration::AlwaysAsk;
  else if (ui.rbOutputDefault->isChecked())
    return CalculatorConfiguration::UseDefault;
  else
    return CalculatorConfiguration::AskButDefaultAfterTimeout;
}


CalculatorConfiguration::OutputMode CalculatorConfiguration::outputMode()
{
  return (CalculatorConfiguration::OutputMode) (ui.cbDefaultOutputMode->currentIndex()+1);
}


CalculatorConfiguration::~CalculatorConfiguration()
{

}
