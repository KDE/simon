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

DesktopGridConfiguration* DesktopGridConfiguration::instance;


DesktopGridConfiguration::DesktopGridConfiguration(QWidget *parent, const QVariantList &args)
		: CommandConfiguration("desktopgrid", ki18n( "Desktopgitter" ),
				      "0.1", ki18n("Sprachgesteuerte Mausklicks"),
				      "games-config-board",
				      DesktopGridPluginFactory::componentData(),
				      parent)
{
	Q_UNUSED(args);
	ui.setupUi(this);
	
	config = KSharedConfig::openConfig(DesktopGridPluginFactory::componentData(),
					"desktopgridrc");

	QObject::connect(ui.leTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	QObject::connect(ui.cbUseRealTransparency, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
}

QString DesktopGridConfiguration::trigger()
{
	KConfigGroup cg(config, "");
	return cg.readEntry("Trigger", i18n("Desktopgitter"));
}

bool DesktopGridConfiguration::useRealTransparency()
{
	KConfigGroup cg(config, "");
	return cg.readEntry("RealTransparency", false);
}

void DesktopGridConfiguration::save()
{
	Q_ASSERT(config);
	
	KConfigGroup cg(config, "");
	cg.writeEntry("Trigger", ui.leTrigger->text());
	cg.writeEntry("RealTransparency", ui.cbUseRealTransparency->isChecked());

	cg.sync();
	
	emit changed(false);
}

void DesktopGridConfiguration::destroy()
{
	deleteLater();
	instance=0;
}
 
void DesktopGridConfiguration::load()
{
	Q_ASSERT(config);

	KConfigGroup cg(config, "");
	ui.leTrigger->setText(cg.readEntry("Trigger", i18n("Desktopgitter")));
	ui.cbUseRealTransparency->setChecked(cg.readEntry("RealTransparency", false));

	cg.sync();
	
	emit changed(false);
}
 
void DesktopGridConfiguration::defaults()
{
	ui.leTrigger->setText(i18n("Desktopgitter"));
	ui.cbUseRealTransparency->setChecked(false);
 
	save();
}

DesktopGridConfiguration::~DesktopGridConfiguration()
{
	
}
