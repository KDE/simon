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

	forceChangeFlag = false;

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
	QObject::connect(ui.asCommandPlugins->availableListWidget(), SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
			  this, SLOT(availablePluginSelectionChanged(QListWidgetItem*)));

	load();
	isChanged=false;

	ActionManager::getInstance()->setConfigurationDialog(this);
}

void CommandSettings::updatePluginListWidgetItem(QListWidgetItem *item, const QString& trigger)
{
	if (!item) return;
	QString displayName = item->data(Qt::UserRole).toString();
	
	if (!trigger.isEmpty())
		displayName += " ("+trigger+")";

	item->setData(Qt::UserRole+2, trigger);
	item->setText(displayName);
}

void CommandSettings::initPluginListWidgetItem(QListWidgetItem *item)
{
	if (!item) return;
	updatePluginListWidgetItem(item, item->data(Qt::UserRole+2).toString());
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
	ui.asCommandPlugins->setButtonsEnabled();

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
	
	QString trigger = activePluginItem->data(Qt::UserRole+2).toString();
	ui.leTrigger->setText(trigger);
}

void CommandSettings::availablePluginSelectionChanged(QListWidgetItem* availablePluginItem)
{
	Q_UNUSED(availablePluginItem);
	ui.asCommandPlugins->setButtonsEnabled();
}

void CommandSettings::unregisterPlugIn(KCModule *plugin)
{
	pageWidget->removePage(moduleHash.value(plugin));
	
	moduleHash.remove(plugin);
}

QList<Action::Ptr> CommandSettings::availableCommandManagers()
{
	QList<Action::Ptr> actions;
	
	KService::List services;
	KServiceTypeTrader* trader = KServiceTypeTrader::self();

	services = trader->query("simon/CommandPlugin");
	
	foreach (KService::Ptr service, services)
		actions.append(QPointer<Action>(new Action(service->storageId())));

	return actions;
}

void CommandSettings::save()
{
	KConfigGroup cg(config, "");
	QListWidget *selected = ui.asCommandPlugins->selectedListWidget();
	QStringList pluginsToLoad;
	QStringList newTrigger;
	for (int i=0; i < selected->count(); i++)
	{
		pluginsToLoad << selected->item(i)->data(Qt::UserRole+1).toString();
		newTrigger << selected->item(i)->data(Qt::UserRole+2).toString();
	}
	

	foreach (KCModule *module, moduleHash.keys())
	{
		module->save();
	}

	bool isChanged=false;
	QStringList storedTrigger = cg.readEntry("Trigger", QStringList());
	QStringList storedPluginsToLoad = cg.readEntry("SelectedPlugins", QStringList());
	if (newTrigger != storedTrigger)
	{
		cg.writeEntry("Trigger", newTrigger);
		isChanged=true;
	}
	
	if (pluginsToLoad != storedPluginsToLoad)
	{
		cg.writeEntry("SelectedPlugins", pluginsToLoad);
		isChanged=true;
	}

	if (forceChangeFlag) isChanged = true;

	QList<Action::Ptr> newActions;
	if (isChanged)
	{
		//build action list
		for (int i=0; i < pluginsToLoad.count(); i++) {
			bool found = false;
			for (int j=0; (j<actions.count()) && (!found); j++) {
				if (actions[j]->source() == pluginsToLoad[i]) {
					Action::Ptr action = actions.takeAt(j);
					action->setTrigger(newTrigger[i]);
					newActions << action;
					found=true;
				}
			}
			if (!found) {
				newActions <<  QPointer<Action>(new Action(pluginsToLoad[i], newTrigger[i]));
			}
		}
	}
	//cleaning de-selected actions
	qDeleteAll(actions);
	actions.clear();

	actions = newActions;
	emit actionsChanged(actions);

	cg.sync();

	KCModule::save();
	emit changed(false);
}

void CommandSettings::displayList(QListWidget *listWidget, QList<Action::Ptr> actions)
{
	if (!listWidget) return;
	listWidget->clear();
	foreach (Action::Ptr action, actions)
	{
		if (!action || !action->manager()) continue;

		QString decorativeName;
		if (!action->trigger().isEmpty()) {
			decorativeName = action->manager()->name()+" ("+action->trigger()+")";
		} else {
			decorativeName = action->manager()->name();
		}

		QListWidgetItem *newItem = new QListWidgetItem(decorativeName);
		newItem->setData(Qt::UserRole, action->manager()->name());
		newItem->setData(Qt::UserRole+1, action->source());
		newItem->setData(Qt::UserRole+2, action->trigger());
		listWidget->addItem(newItem);
	}
}

QStringList CommandSettings::findDefaultPlugins(const QList<Action::Ptr>& actions)
{
	QStringList defaultPluginList;
	foreach (Action::Ptr action, actions) {
		if (action && action->enabledByDefault())
			defaultPluginList << action->source();
	}
	return defaultPluginList;
}

void CommandSettings::load()
{
	Q_ASSERT(config);

	KConfigGroup cg(config, "");

	QList<Action::Ptr> allPlugins = availableCommandManagers();
	
	QStringList trigger = cg.readEntry("Trigger", QStringList());

	QStringList defaultPluginList=findDefaultPlugins(allPlugins);
	QStringList pluginsToLoad = cg.readEntry("SelectedPlugins", defaultPluginList);

	// ensure that trigger has the same amount of elements
	// as pluginsToLoad
	while (trigger.count() < pluginsToLoad.count())
		trigger << QString();
	while (trigger.count() > pluginsToLoad.count())
		trigger.removeAt(trigger.count()-1);

	QList<Action::Ptr> notSelectedPlugins;
	QList<Action::Ptr> selectedPlugins;
	Action::Ptr selectedPluginsArr[pluginsToLoad.count()];

	int loadedCount=0;
	while (!allPlugins.isEmpty())
	{
		Action *currentAction = allPlugins.takeAt(0); //take the first

		//skip if faulty
		//if the manager could not be loaded from the given storageid it is set to NULL
		if (!currentAction)
			continue;

		if (pluginsToLoad.contains(currentAction->source())) { 
			//if this is in the list to load or we never started before and the
		        //load-by-default flag is set for this plugin
			
			int indexInList;

			indexInList = pluginsToLoad.indexOf(currentAction->source());
			if (!trigger[indexInList].isNull()) {
				currentAction->setTrigger(trigger[indexInList]);
			}
			selectedPluginsArr[indexInList] = currentAction;
			loadedCount++;
		} else {
			notSelectedPlugins.append(currentAction);
		}
	}

	for (int i=0; i < loadedCount; i++) {
		selectedPlugins << selectedPluginsArr[i];
	}

	displayList(ui.asCommandPlugins->availableListWidget(), notSelectedPlugins);
	displayList(ui.asCommandPlugins->selectedListWidget(), selectedPlugins);

	this->actions = selectedPlugins;
	
	cg.sync();

	foreach (KCModule *module, moduleHash.keys()) {
		module->load();
	}

	emit changed(false);
	KCModule::load();
}

QList<Action::Ptr> CommandSettings::getActivePlugins()
{
	return actions;
}


void CommandSettings::defaults()
{
	foreach (KCModule *module, moduleHash.keys()) {
		module->defaults();
	}
	KCModule::defaults();

	KConfigGroup cg(config, "");
	cg.writeEntry("Trigger", QStringList());
	cg.writeEntry("SelectedPlugins", findDefaultPlugins(availableCommandManagers()));
	cg.sync();
	load();
	forceChangeFlag = true;
	save();
}


void CommandSettings::slotChanged()
{
	ui.asCommandPlugins->setButtonsEnabled();
	emit changed(true);
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
CommandSettings::~CommandSettings()
{
}


