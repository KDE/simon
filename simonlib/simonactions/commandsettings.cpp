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
#include <QListWidgetItem>

#include <KAboutData>
#include <KService>
#include <KServiceTypeTrader>
#include <KPageWidget>
#include <commandpluginbase/commandmanager.h>
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

	KPageWidgetItem *generalItem = pageWidget->addPage(baseWidget, i18n("General"));
	generalItem->setIcon(KIcon("fork"));
	generalItem->setHeader("");

	KAboutData *about = new KAboutData(
				"commandsettings", "", ki18n("Command Settings"),
				"0.1", ki18n("Configuration about the Commands and the Selection of Plugins to use"), KAboutData::License_GPL);
#if KDE_IS_VERSION(4,0,80)
	about->setProgramIconName("fork");
#endif
	setAboutData( about );


	config = KSharedConfig::openConfig(KGlobal::mainComponent(),
					"simoncommandrc");

	QObject::connect(ui.asCommandPlugins, SIGNAL(added(QListWidgetItem*)), this, SLOT(slotChanged()));
	QObject::connect(ui.asCommandPlugins, SIGNAL(added(QListWidgetItem*)), this, SLOT(initPluginListWidgetItem(QListWidgetItem*)));
	QObject::connect(ui.asCommandPlugins, SIGNAL(removed(QListWidgetItem*)), this, SLOT(slotChanged()));
	QObject::connect(ui.asCommandPlugins, SIGNAL(movedUp(QListWidgetItem*)), this, SLOT(slotChanged()));
	QObject::connect(ui.asCommandPlugins, SIGNAL(movedDown(QListWidgetItem*)), this, SLOT(slotChanged()));
	QObject::connect(ui.leTrigger, SIGNAL(textChanged(const QString&)), this, SLOT(currentTriggerChanged(const QString&)));
	QObject::connect(ui.leTrigger, SIGNAL(textChanged(const QString&)), this, SLOT(slotChanged()));
	QObject::connect(ui.pbApplyToAll, SIGNAL(clicked()), this, SLOT(applyToAllClicked()));
	
	QObject::connect(ui.asCommandPlugins->selectedListWidget(), SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
			  this, SLOT(activePluginSelectionChanged(QListWidgetItem*)));

	load();
	isChanged=false;

	ActionManager::getInstance()->setConfigurationDialog(this);
}

void CommandSettings::updatePluginListWidgetItem(QListWidgetItem *item, const QString& trigger)
{
	if (!item) return;
	QString displayName = item->data(Qt::UserRole).toString();
	kDebug() << displayName;
	
	if (!trigger.isEmpty())
		displayName += " ("+trigger+")";

	item->setData(Qt::UserRole+1, trigger);
	item->setText(displayName);
}

void CommandSettings::initPluginListWidgetItem(QListWidgetItem *item)
{
	if (!item) return;
	updatePluginListWidgetItem(item, item->data(Qt::UserRole+1).toString());
}

void CommandSettings::currentTriggerChanged(const QString& newTrigger)
{
	QListWidgetItem *pluginItem = ui.asCommandPlugins->selectedListWidget()->currentItem();
	if (!pluginItem) return;
	updatePluginListWidgetItem(pluginItem, newTrigger);
}

void CommandSettings::applyToAllClicked()
{
	QListWidget *selected = ui.asCommandPlugins->selectedListWidget();
	QString trigger = ui.leTrigger->text();
	for (int i=0; i < selected->count(); i++)
		updatePluginListWidgetItem(selected->item(i), trigger);
}

void CommandSettings::registerPlugIn(KCModule *plugin)
{
	if (!plugin) return;

	Q_ASSERT(plugin->aboutData());

	QString moduleName = plugin->aboutData()->programName();
#if KDE_IS_VERSION(4,0,80)
	QString moduleIcon = plugin->aboutData()->programIconName();
#endif
	KPageWidgetItem *newItem = pageWidget->addPage(plugin, moduleName);
#if KDE_IS_VERSION(4,0,80)
	newItem->setIcon(KIcon(moduleIcon));
#endif
	newItem->setHeader("");
	moduleHash.insert(plugin, newItem);

	connect(plugin, SIGNAL(changed(bool)), this, SLOT(pluginChanged(bool)));
}

void CommandSettings::pluginChanged(bool isChanged)
{
	if (isChanged) emit changed(true);
}

void CommandSettings::activePluginSelectionChanged(QListWidgetItem* activePluginItem)
{
	if (!activePluginItem)
	{
		ui.lbTrigger->setEnabled(false);
		ui.leTrigger->setEnabled(false);
		ui.pbApplyToAll->setEnabled(false);
		return;
	}
	
	ui.pbApplyToAll->setEnabled(true);
	ui.lbTrigger->setEnabled(true);
	ui.leTrigger->setEnabled(true);
	
	QString trigger = activePluginItem->data(Qt::UserRole+1).toString();
	ui.leTrigger->setText(trigger);
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
	QListWidget *selected = ui.asCommandPlugins->selectedListWidget();
	QStringList pluginsToLoad;
	QStringList newTrigger;
	for (int i=0; i < selected->count(); i++)
	{
		pluginsToLoad << selected->item(i)->data(Qt::UserRole).toString();
		newTrigger << selected->item(i)->data(Qt::UserRole+1).toString();
	}
	

	foreach (KCModule *module, moduleHash.keys())
	{
		module->save();
	}

	if (newTrigger != storedTrigger)
	{
		cg.writeEntry("Trigger", newTrigger);
		emit triggerChanged(newTrigger);
		this->storedTrigger = newTrigger;
	}
	
	if (pluginsToLoad != this->pluginsToLoad)
	{
		cg.writeEntry("SelectedPlugins", pluginsToLoad);
		emit pluginSelectionChanged(pluginsToLoad);
		this->pluginsToLoad = pluginsToLoad;
	}

	cg.sync();

	emit changed(false);
	KCModule::save();
}

void CommandSettings::load()
{
	Q_ASSERT(config);

	KConfigGroup cg(config, "");

	QStringList trigger = cg.readEntry("Trigger", QStringList());

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
			int index = pluginsToLoad.indexOf(pluginName);
			while (index != -1)
			{
				pluginsToLoad.removeAt(index);
				if (trigger.count() > index)
					trigger.removeAt(index);
				index = pluginsToLoad.indexOf(pluginName);
			}
			pluginsToLoad.removeAll(pluginName);
			continue;
		}
		
		notSelectedPlugins.removeAll(pluginName);
	}
	foreach (const QString pluginName, notSelectedPlugins)
	{
		QListWidgetItem *newItem = new QListWidgetItem(pluginName);
		newItem->setData(Qt::UserRole, pluginName);
		newItem->setData(Qt::UserRole+1, i18n("Computer"));
		available->addItem(newItem);
	}

	// ensure that trigger has the same amount of elements
	// as pluginsToLoad
	if (trigger.count() != pluginsToLoad.count())
	{
		for (int i=0; trigger.count() < pluginsToLoad.count(); i++)
			trigger << i18n("Computer");

		while (trigger.count() > pluginsToLoad.count())
			trigger.removeAt(trigger.count()-1);
	}
	storedTrigger = trigger;
	
	int i=0;
	foreach (QString pluginToLoad, pluginsToLoad)
	{
		QString name;
		if (trigger[i].isEmpty())
			name = pluginToLoad;
		else name = pluginToLoad+" ("+trigger[i]+")";
		
		QListWidgetItem *newItem = new QListWidgetItem(name);
		newItem->setData(Qt::UserRole, pluginToLoad);
		newItem->setData(Qt::UserRole+1, trigger[i]);
		selected->addItem(newItem);
		i++;
	}
	
	this->pluginsToLoad = pluginsToLoad;
	
	cg.sync();
	

	foreach (KCModule *module, moduleHash.keys())
	{
		module->load();
	}

	emit changed(false);
	KCModule::load();
}

QStringList CommandSettings::getPluginsToLoad()
{
	return pluginsToLoad;
}

QStringList CommandSettings::getTrigger()
{
	return storedTrigger;
}

void CommandSettings::defaults()
{
	QListWidget* available = ui.asCommandPlugins->availableListWidget();
	available->clear();
	available->addItems(availableCommandManagers());
	ui.asCommandPlugins->selectedListWidget()->clear();


	foreach (KCModule *module, moduleHash.keys())
	{
		module->defaults();
	}

	save();
	KCModule::defaults();
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


