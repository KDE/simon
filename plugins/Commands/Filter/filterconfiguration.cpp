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

QPointer<FilterConfiguration> FilterConfiguration::instance;


FilterConfiguration::FilterConfiguration(QWidget *parent, const QVariantList &args)
		: CommandConfiguration("filter", ki18n( "Filter" ),
				      "0.1", ki18n("Filter recognition results"),
				      "view-filter",
				      FilterPluginFactory::componentData(),
				      parent)
{
	Q_UNUSED(args);
	ui.setupUi(this);
	
	config = KSharedConfig::openConfig(FilterPluginFactory::componentData(),
					"filterrc");

	QObject::connect(ui.leActivateTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	QObject::connect(ui.leDeactivateTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
}

QString FilterConfiguration::deactivateTrigger() const
{
	KConfigGroup cg(config, "");
	return cg.readEntry("DeactivateTrigger", i18n("Resume recognition"));
}

QString FilterConfiguration::activateTrigger() const
{
	KConfigGroup cg(config, "");
	return cg.readEntry("ActivateTrigger", i18n("Pause recognition"));
}

void FilterConfiguration::save()
{
	Q_ASSERT(config);
	
	KConfigGroup cg(config, "");
	cg.writeEntry("ActivateTrigger", ui.leActivateTrigger->text());
	cg.writeEntry("DeactivateTrigger", ui.leDeactivateTrigger->text());

	cg.sync();
	
	emit changed(false);
}

void FilterConfiguration::destroy()
{
	deleteLater();
	instance=0;
}
 
void FilterConfiguration::load()
{
	Q_ASSERT(config);

	KConfigGroup cg(config, "");
	ui.leActivateTrigger->setText(cg.readEntry("ActivateTrigger", i18n("Pause recognition")));
	ui.leDeactivateTrigger->setText(cg.readEntry("DeactivateTrigger", i18n("Resume recognition")));

	cg.sync();
	
	emit changed(false);
}
 
void FilterConfiguration::defaults()
{
	ui.leActivateTrigger->setText(i18n("Pause recognition"));
	ui.leDeactivateTrigger->setText(i18n("Resume recognition"));
 
	save();
}

FilterConfiguration::~FilterConfiguration()
{
	
}
