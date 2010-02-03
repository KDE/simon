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

#include "commandsettings.h"
#include "actionmanager.h"

#include <QListWidget>
#include <QVariant>
#include <QPointer>
#include <QListWidgetItem>

#include <KAboutData>
#include <KMessageBox>
#include <KService>
#include <KServiceTypeTrader>
#include <KPageWidget>
#include <simonscenarios/commandmanager.h>
#include <simonscenarios/scenariomanager.h>
#include <kgenericfactory.h>


K_PLUGIN_FACTORY( CommandSettingsFactory, 
			registerPlugin< CommandSettings >(); 
		)
        
K_EXPORT_PLUGIN( CommandSettingsFactory("simonlib") )

CommandSettings* CommandSettings::instance;
/**
 * \brief Constructor
 *
 * Initializes the Systemwidget by giving name, icon and description
 * 
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
CommandSettings::CommandSettings(QWidget* parent, const QVariantList& args): KCModule(KGlobal::mainComponent(), parent),
	forceChangeFlag(false),
	isChanged(false)
{
	Q_UNUSED(args)

	ui.setupUi(this);

	KAboutData *about = new KAboutData(
				"commandsettings", "", ki18n("Command Settings"),
				"0.1", ki18n("Configuration about the Commands and the Selection of Plugins to use"), KAboutData::License_GPL);
#if KDE_IS_VERSION(4,0,80)
	about->setProgramIconName("fork");
#endif
	setAboutData( about );


	config = KSharedConfig::openConfig(KGlobal::mainComponent(),
					"simoncommandrc");

	QObject::connect(ui.cbUseDYM, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	QObject::connect(ui.sbMinimumConfidence, SIGNAL(valueChanged(double)), this, SLOT(slotChanged()));
	QObject::connect(ui.fcFont, SIGNAL(fontSelected(const QFont&)), this, SLOT(slotChanged()));

	load();
}


void CommandSettings::save()
{
	KConfigGroup cg(config, "");
	cg.writeEntry("MinimumConfidence", ui.sbMinimumConfidence->value());
	cg.writeEntry("UseDYM", ui.cbUseDYM->isChecked());
	if (storedFont != ui.fcFont->font()) {
		storedFont = ui.fcFont->font();
		cg.writeEntry("PluginBaseFont", storedFont);
		ScenarioManager::getInstance()->setPluginFont(storedFont);
	}

	cg.sync();
	KCModule::save();
	emit changed(false);
//	emit recognitionResultsFilterParametersChanged();
	ActionManager::getInstance()->retrieveRecognitionResultFilteringParameters();
}

void CommandSettings::load()
{
	Q_ASSERT(config);

	KConfigGroup cg(config, "");

	float minimumConfidence = cg.readEntry("MinimumConfidence", 0.7f);
	ui.sbMinimumConfidence->setValue(minimumConfidence);

	ui.cbUseDYM->setChecked(cg.readEntry("UseDYM", true));
	storedFont = cg.readEntry("PluginBaseFont", QFont());
	ui.fcFont->setFont(storedFont);

	cg.sync();

	emit changed(false);
	KCModule::load();
}

float CommandSettings::minimumConfidence()
{
	return ui.sbMinimumConfidence->value();
}

bool CommandSettings::useDYM()
{
	return ui.cbUseDYM->isChecked();
}

QFont CommandSettings::pluginBaseFont()
{
	return storedFont;
}

void CommandSettings::defaults()
{
	KCModule::defaults();

	KConfigGroup cg(config, "");
	cg.writeEntry("MinimumConfidence", 0.7f);
	cg.writeEntry("UseDYM", true);
	cg.writeEntry("PluginBaseFont", QFont());
	ScenarioManager::getInstance()->setPluginFont(QFont());
	cg.sync();
	load();
	forceChangeFlag = true;
	save();
}


void CommandSettings::slotChanged()
{
	emit changed(true);
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
CommandSettings::~CommandSettings()
{
}


