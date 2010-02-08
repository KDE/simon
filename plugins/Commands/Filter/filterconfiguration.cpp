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
	
	QObject::connect(ui.leRegExp, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
}

QString FilterConfiguration::regExp() const
{
	return ui.leRegExp->text();
}

bool FilterConfiguration::deSerialize(const QDomElement& elem)
{
	QDomElement regExpElem = elem.firstChildElement("regExp");

	if (regExpElem.isNull()) 
		defaults();
	else
		ui.leRegExp->setText(regExpElem.text());

	return true;
}

QDomElement FilterConfiguration::serialize(QDomDocument *doc)
{
	QDomElement configElem = doc->createElement("config");

	QDomElement regExpElem = doc->createElement("regExp");
	regExpElem.appendChild(doc->createTextNode(regExp()));

	configElem.appendChild(regExpElem);

	return configElem;
}

void FilterConfiguration::defaults()
{
	ui.leRegExp->setText(i18n(".*"));
}

FilterConfiguration::~FilterConfiguration()
{
	
}
