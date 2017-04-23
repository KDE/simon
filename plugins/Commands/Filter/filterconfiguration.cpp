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

#include "filterconfiguration.h"
#include "filtercommandmanager.h"
#include <QVariantList>
#include <KPluginFactory>

K_PLUGIN_FACTORY_DECLARATION(FilterPluginFactory)

FilterConfiguration::FilterConfiguration(Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent, "filter", i18n( "Filter" ),
"0.1", i18n("Filter recognition results"),
"view-filter")
{
  Q_UNUSED(args);
  ui.setupUi(this);

  QObject::connect(ui.leRegExp, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
}


QString FilterConfiguration::regExp() const
{
  return ui.leRegExp->text();
}

bool FilterConfiguration::relayStageOne() const
{
  return ui.cbRelayStageOne->isChecked();
}

bool FilterConfiguration::twoStage() const
{
  return ui.cbTwoStage->isChecked();
}

bool FilterConfiguration::autoLeaveStageOne() const
{
  return ui.cbLeaveStageOneAutomatically->isChecked();
}


int FilterConfiguration::autoLeaveStageOneTimeout() const
{
  return ui.sbAutoLeaveTimeout->value();
}


bool FilterConfiguration::deSerialize(const QDomElement& elem)
{
  QDomElement regExpElem = elem.firstChildElement("regExp");

  if (regExpElem.isNull())
    defaults();
  else
    ui.leRegExp->setText(regExpElem.text());

  ui.cbTwoStage->setChecked(elem.firstChildElement("twoStage").text() == "1");
  ui.cbRelayStageOne->setChecked(elem.firstChildElement("relayStageOne").text() == "1");
  ui.cbLeaveStageOneAutomatically->setChecked(elem.firstChildElement("autoLeaveStageOne").text() == "1");
  ui.sbAutoLeaveTimeout->setValue(elem.firstChildElement("autoLeaveStageOneTimeout").text().toInt());
  return true;
}


QDomElement FilterConfiguration::serialize(QDomDocument *doc)
{
  QDomElement configElem = doc->createElement("config");

  QDomElement regExpElem = doc->createElement("regExp");
  regExpElem.appendChild(doc->createTextNode(regExp()));

  QDomElement twoStageElem = doc->createElement("twoStage");
  twoStageElem.appendChild(doc->createTextNode(twoStage() ? "1" : "0"));

  QDomElement relayElem = doc->createElement("relayStageOne");
  relayElem.appendChild(doc->createTextNode(relayStageOne() ? "1" : "0"));

  QDomElement autoLeaveElem = doc->createElement("autoLeaveStageOne");
  autoLeaveElem.appendChild(doc->createTextNode(autoLeaveStageOne() ? "1" : "0"));

  QDomElement autoLeaveTimeoutElem = doc->createElement("autoLeaveStageOneTimeout");
  autoLeaveTimeoutElem.appendChild(doc->createTextNode(QString::number(autoLeaveStageOneTimeout())));

  configElem.appendChild(regExpElem);
  configElem.appendChild(twoStageElem);
  configElem.appendChild(relayElem);
  configElem.appendChild(autoLeaveElem);
  configElem.appendChild(autoLeaveTimeoutElem);

  return configElem;
}


void FilterConfiguration::defaults()
{
  ui.leRegExp->setText(i18n(".*"));
  ui.cbTwoStage->setChecked(false);
  ui.cbRelayStageOne->setChecked(false);
  ui.cbLeaveStageOneAutomatically->setChecked(false);
  ui.sbAutoLeaveTimeout->setValue(5000);
}


FilterConfiguration::~FilterConfiguration()
{

}
