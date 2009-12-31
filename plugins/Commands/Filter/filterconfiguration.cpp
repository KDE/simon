/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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
#include <kgenericfactory.h>
#include <KAboutData>
#include <KMessageBox>

K_PLUGIN_FACTORY_DECLARATION(FilterPluginFactory)


FilterConfiguration::FilterConfiguration(Scenario *parent, const QVariantList &args)
		: CommandConfiguration(parent, "filter", ki18n( "Filter" ),
				      "0.1", ki18n("Filter recognition results"),
				      "view-filter",
				      FilterPluginFactory::componentData())
{
	Q_UNUSED(args);
	ui.setupUi(this);
	
	QObject::connect(ui.leActivateTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	QObject::connect(ui.leDeactivateTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
}

QString FilterConfiguration::deactivateTrigger() const
{
	return ui.leDeactivateTrigger->text();
}

QString FilterConfiguration::activateTrigger() const
{
	return ui.leActivateTrigger->text();
}

bool FilterConfiguration::deSerialize(const QDomElement& elem)
{
	QDomElement activateTriggerElem = elem.firstChildElement("activateTrigger");
	QDomElement deActivateTriggerElem = elem.firstChildElement("deactivateTrigger");

	if (activateTriggerElem.isNull() || deActivateTriggerElem.isNull()) {
		defaults();
	} else {
		ui.leActivateTrigger->setText(activateTriggerElem.text());
		ui.leDeactivateTrigger->setText(deActivateTriggerElem.text());
	}
	return true;
}

QDomElement FilterConfiguration::serialize(QDomDocument *doc)
{
	QDomElement configElem = doc->createElement("config");

	QDomElement activateTriggerElem = doc->createElement("activateTrigger");
	activateTriggerElem.appendChild(doc->createTextNode(activateTrigger()));

	QDomElement deactivateTriggerElem = doc->createElement("deactivateTrigger");
	deactivateTriggerElem.appendChild(doc->createTextNode(deactivateTrigger()));

	configElem.appendChild(activateTriggerElem);
	configElem.appendChild(deactivateTriggerElem);

	return configElem;
}

void FilterConfiguration::defaults()
{
	ui.leActivateTrigger->setText(i18n("Pause recognition"));
	ui.leDeactivateTrigger->setText(i18n("Resume recognition"));
}

FilterConfiguration::~FilterConfiguration()
{
	
}
