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

#ifndef COMMANDSETTINGS_H
#define COMMANDSETTINGS_H

#include "ui_commandsettingsdlg.h"
#include <KCModule>
#include <QStringList>
#include <QVariantList>
#include <QHash>

class KPageWidget;
class KPageWidgetItem;
/**
 * \class CommandSettings
 * \author Peter Grasch
 * \brief KCModule that manages some general options and what command-plugins to load
 * \date 13.08.2007
 * \version 0.1
 */
class CommandSettings : public KCModule
{
Q_OBJECT

signals:
	void pluginSelectionChanged(const QStringList& pluginsToLoad);

private:
	static CommandSettings* instance;

	Ui::CommandSettingsDlg ui;
	KSharedConfig::Ptr config;
	bool isChanged;

	KPageWidget *pageWidget;
	QHash<KCModule*, KPageWidgetItem*> moduleHash;

	QStringList pluginsToLoad;
// 	bool storedUseGlobalTrigger;
// 	QString storedGlobalTrigger;

	QStringList availableCommandManagers();


public slots:
	virtual void save();
	virtual void load();
	virtual void defaults();
 
private slots:
	void slotChanged();
	void pluginChanged(bool isChanged);

public:
	static CommandSettings* getInstance(QWidget *parent=0, const QVariantList& args=QVariantList())
	{
		if (!instance) instance = new CommandSettings(parent, args);
		return instance;
	}

	CommandSettings(QWidget* parent=0, const QVariantList& args=QVariantList());
	QStringList getPluginsToLoad();
	~CommandSettings();
	
// 	bool useGlobalTrigger() { return storedUseGlobalTrigger; }
// 	QString globalTrigger() { return storedGlobalTrigger; }

	void registerPlugIn(KCModule *plugin);
	void unregisterPlugIn(KCModule *plugin);

};

#endif
