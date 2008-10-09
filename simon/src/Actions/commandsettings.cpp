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
#include <KAboutData>
#include <KMessageBox>


/**
 * \brief Constructor
 *
 * Initializes the Systemwidget by giving name, icon and description
 * 
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
CommandSettings::CommandSettings(QWidget* parent): KCModule(KGlobal::mainComponent(), parent)
{
	ui.setupUi(this);

	KAboutData *about = new KAboutData(
				"commandsettings", "", ki18n("Kommandoeinstellungen"),
				"0.1", ki18n("Einstellungen rund um die Kommandos und Auswahl der zu verwendenden Plugins"), KAboutData::License_GPL);
	about->setProgramIconName("fork");
	setAboutData( about );


	config = KSharedConfig::openConfig(KGlobal::mainComponent(),
					"simoncommandrc");

	QObject::connect(ui.cbDictation, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	QObject::connect(ui.cbUseGlobalTrigger, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	QObject::connect(ui.leGlobalTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	QObject::connect(ui.asCommandPlugins, SIGNAL(added(QListWidgetItem*)), this, SLOT(slotChanged()));
	QObject::connect(ui.asCommandPlugins, SIGNAL(removed(QListWidgetItem*)), this, SLOT(slotChanged()));

	load();
	isChanged=false;
}


void CommandSettings::save()
{
	KConfigGroup cg(config, "");

	cg.writeEntry("Dictation", ui.cbDictation->isChecked());
	cg.writeEntry("UseGlobalTrigger", ui.cbUseGlobalTrigger->isChecked());
	cg.writeEntry("GlobalTrigger", ui.leGlobalTrigger->text());
	
	QListWidget *selected = ui.asCommandPlugins->selectedListWidget();
	QStringList pluginsToLoad;
	for (int i=0; i < selected->count(); i++)
	{
		pluginsToLoad << selected->item(i)->text();
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
	ui.cbDictation->setChecked(cg.readEntry("Dictation", false));
	ui.cbUseGlobalTrigger->setChecked(cg.readEntry("UseGlobalTrigger", true));
	ui.leGlobalTrigger->setText(cg.readEntry("GlobalTrigger", i18n("Computer")));


	QListWidget* available = ui.asCommandPlugins->availableListWidget();
	available->clear();
	QListWidget* selected = ui.asCommandPlugins->selectedListWidget();
	selected->clear();
	
	QStringList allPlugins = ActionManager::getInstance()->availableCommandManagers();
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
	
	emit changed(false);
}

QStringList CommandSettings::getPluginsToLoad()
{
	return pluginsToLoad;
}

void CommandSettings::defaults()
{
	ui.cbDictation->setChecked(false);
	ui.cbUseGlobalTrigger->setChecked(true);
	ui.leGlobalTrigger->setText(i18n("Computer"));

	QListWidget* available = ui.asCommandPlugins->availableListWidget();
	available->clear();
	available->addItems(ActionManager::getInstance()->availableCommandManagers());
	ui.asCommandPlugins->selectedListWidget()->clear();


	save();
}


void CommandSettings::slotChanged()
{
// 	KMessageBox::information(this, "wtf?");
	emit changed(true);
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
CommandSettings::~CommandSettings()
{
}


