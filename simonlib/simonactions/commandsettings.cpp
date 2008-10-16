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
#include <QListWidget>
#include <KAboutData>
#include <KService>
#include <KServiceTypeTrader>
#include <KPageWidget>
#include <commandpluginbase/commandmanager.h>
#include <kgenericfactory.h>
#include <QDebug>



K_PLUGIN_FACTORY( CommandSettingsFactory, 
			registerPlugin< CommandSettings >(); 
		)
        
K_EXPORT_PLUGIN( CommandSettingsFactory("CommandSettings") )
/**
 * \brief Constructor
 *
 * Initializes the Systemwidget by giving name, icon and description
 * 
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
CommandSettings::CommandSettings(QWidget* parent, const QVariantList& args): KCModule(KGlobal::mainComponent(), parent)
{
	Q_UNUSED(args)

	QWidget *baseWidget = new QWidget(this);
	ui.setupUi(baseWidget);

	QVBoxLayout *lay=new QVBoxLayout(this);
	pageWidget = new KPageWidget(this);
	lay->addWidget(pageWidget);


	if (args.count() == 1)
		pageWidget->setFaceType(KPageView::Tabbed);

	KPageWidgetItem *generalItem = pageWidget->addPage(baseWidget, i18n("Allgemein"));
	generalItem->setIcon(KIcon("fork"));
	generalItem->setHeader("");

	KAboutData *about = new KAboutData(
				"commandsettings", "", ki18n("Kommandoeinstellungen"),
				"0.1", ki18n("Einstellungen rund um die Kommandos und Auswahl der zu verwendenden Plugins"), KAboutData::License_GPL);
	about->setProgramIconName("fork");
	setAboutData( about );


	config = KSharedConfig::openConfig(KGlobal::mainComponent(),
					"simoncommandrc");

	QObject::connect(ui.cbUseGlobalTrigger, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	QObject::connect(ui.leGlobalTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	QObject::connect(ui.asCommandPlugins, SIGNAL(added(QListWidgetItem*)), this, SLOT(slotChanged()));
	QObject::connect(ui.asCommandPlugins, SIGNAL(removed(QListWidgetItem*)), this, SLOT(slotChanged()));
	QObject::connect(ui.asCommandPlugins, SIGNAL(movedUp(QListWidgetItem*)), this, SLOT(slotChanged()));
	QObject::connect(ui.asCommandPlugins, SIGNAL(movedDown(QListWidgetItem*)), this, SLOT(slotChanged()));

	load();
	isChanged=false;
}

void CommandSettings::registerPlugIn(KCModule *plugin)
{
	if (!plugin) return;

	Q_ASSERT(plugin->aboutData());

	QString moduleName = plugin->aboutData()->programName();
	QString moduleIcon = plugin->aboutData()->programIconName();
	KPageWidgetItem *newItem = pageWidget->addPage(plugin, moduleName);
	newItem->setIcon(KIcon(moduleIcon));
	newItem->setHeader("");
	moduleHash.insert(plugin, newItem);

	connect(plugin, SIGNAL(changed(bool)), this, SLOT(pluginChanged(bool)));
}

void CommandSettings::pluginChanged(bool isChanged)
{
	if (isChanged) emit changed(true);
}


void CommandSettings::unregisterPlugIn(KCModule *plugin)
{
	pageWidget->removePage(moduleHash.value(plugin));
	
	moduleHash.remove(plugin);
}

QStringList CommandSettings::availableCommandManagers()
{
	QStringList commandManagers;
	
	KService::List services;
	KServiceTypeTrader* trader = KServiceTypeTrader::self();

	services = trader->query("simon/CommandPlugin");
	
	foreach (KService::Ptr service, services) {
		KPluginFactory *factory = KPluginLoader(service->library()).factory();
		if (!factory) continue;
		
		CommandManager *man = factory->create<CommandManager>(this);
		if (man) {
			commandManagers << man->name();
			man->deleteLater();
		}
	}
	return commandManagers;
}


void CommandSettings::save()
{
	KConfigGroup cg(config, "");

	cg.writeEntry("UseGlobalTrigger", ui.cbUseGlobalTrigger->isChecked());
	cg.writeEntry("GlobalTrigger", ui.leGlobalTrigger->text());
	
	QListWidget *selected = ui.asCommandPlugins->selectedListWidget();
	QStringList pluginsToLoad;
	for (int i=0; i < selected->count(); i++)
	{
		pluginsToLoad << selected->item(i)->text();
	}
	

	foreach (KCModule *module, moduleHash.keys())
	{
		module->save();
	}
	
	if (pluginsToLoad != this->pluginsToLoad)
	{
		cg.writeEntry("SelectedPlugins", pluginsToLoad);
		emit pluginSelectionChanged(pluginsToLoad);
		this->pluginsToLoad = pluginsToLoad;
	}

	cg.sync();

	emit changed(false);
}

void CommandSettings::load()
{
	Q_ASSERT(config);

	KConfigGroup cg(config, "");
	
	storedUseGlobalTrigger = cg.readEntry("UseGlobalTrigger", true);
	storedGlobalTrigger = cg.readEntry("GlobalTrigger", i18n("Computer"));
	
	ui.cbUseGlobalTrigger->setChecked(storedUseGlobalTrigger);
	ui.leGlobalTrigger->setText(storedGlobalTrigger);


	QListWidget* available = ui.asCommandPlugins->availableListWidget();
	available->clear();
	QListWidget* selected = ui.asCommandPlugins->selectedListWidget();
	selected->clear();
	
	QStringList allPlugins = availableCommandManagers();
	QStringList notSelectedPlugins = allPlugins;
	
	QStringList pluginsToLoad = cg.readEntry("SelectedPlugins", QStringList());
	foreach (const QString& pluginName, pluginsToLoad)
	{
		if (!allPlugins.contains(pluginName))
		{
			pluginsToLoad.removeAll(pluginName);
			continue;
		}
		
		notSelectedPlugins.removeAll(pluginName);
	}
	available->addItems(notSelectedPlugins);
	selected->addItems(pluginsToLoad);
	
	this->pluginsToLoad = pluginsToLoad;
	
	cg.sync();
	

	foreach (KCModule *module, moduleHash.keys())
	{
		module->load();
	}

	emit changed(false);
}

QStringList CommandSettings::getPluginsToLoad()
{
	return pluginsToLoad;
}

void CommandSettings::defaults()
{
	ui.cbUseGlobalTrigger->setChecked(true);
	ui.leGlobalTrigger->setText(i18n("Computer"));

	QListWidget* available = ui.asCommandPlugins->availableListWidget();
	available->clear();
	available->addItems(availableCommandManagers());
	ui.asCommandPlugins->selectedListWidget()->clear();


	foreach (KCModule *module, moduleHash.keys())
	{
		module->defaults();
	}

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


