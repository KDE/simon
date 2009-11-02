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

#include "desktopgridconfiguration.h"
#include "desktopgridcommandmanager.h"
#include <QVariantList>
#include <kgenericfactory.h>
#include <KAboutData>
#include <KMessageBox>

K_PLUGIN_FACTORY_DECLARATION(DesktopGridPluginFactory)


DesktopGridConfiguration::DesktopGridConfiguration(Scenario *parent, const QVariantList &args)
		: CommandConfiguration(parent, "desktopgrid", ki18n( "Desktopgrid" ),
				      "0.1", ki18n("Voice controlled mouse clicks"),
				      "games-config-board",
				      DesktopGridPluginFactory::componentData())
{
	Q_UNUSED(args);
	ui.setupUi(this);
	
	QObject::connect(ui.cbUseRealTransparency, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
}

bool DesktopGridConfiguration::useRealTransparency()
{
	return ui.cbUseRealTransparency->isChecked();
}

bool DesktopGridConfiguration::deSerialize(const QDomElement& elem)
{
	QDomElement realTransElem = elem.firstChildElement("realTransparency");
	bool ok;
	int realTransparency = realTransElem.text().toInt(&ok);
	if (!ok) defaults();
	else 
		ui.cbUseRealTransparency->setChecked(realTransparency);

	return true;
}

QDomElement DesktopGridConfiguration::serialize(QDomDocument *doc)
{
	QDomElement configElem = doc->createElement("config");
	QDomElement realTransElem = doc->createElement("realTransparency");
	realTransElem.appendChild(doc->createTextNode(QString::number(ui.cbUseRealTransparency->isChecked() ? 1 : 0)));
	configElem.appendChild(realTransElem);
	return configElem;
}
 
void DesktopGridConfiguration::defaults()
{
	ui.cbUseRealTransparency->setChecked(true);
}

DesktopGridConfiguration::~DesktopGridConfiguration()
{
	
}
