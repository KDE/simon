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

#ifndef SIMON_COMMANDSETTINGS_H_92014DF656EC423699D2F493D77108BA
#define SIMON_COMMANDSETTINGS_H_92014DF656EC423699D2F493D77108BA

#include "ui_commandsettingsdlg.h"
#include "commandlistwidget.h"
#include <simonscenarios/commandlistelements.h>
#include <QStringList>
#include <QVariantList>
#include <QFont>
#include <QList>
#include <QHash>
#include <KCModule>

class QListWidgetItem;
class KPageWidget;
class KPageWidgetItem;
class Action;
class VoiceInterfaceCommand;

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
	void recognitionResultsFilterParametersChanged();

private:
	bool forceChangeFlag;
	static CommandSettings* instance;

	Ui::CommandSettingsDlg ui;
	KSharedConfig::Ptr config;
	bool isChanged;
	QFont storedFont;
	CommandListElements::Element getRowElementType(int row);
	void storeCurrentlyDisplayedElement(CommandListElements::Element type);
	void displayListElement(CommandListElements::Element type);

	QString getListSelectionId(CommandListElements::Element type);
	QString getListSelectionDescription(CommandListElements::Element type);
	QString getListDefaultTrigger(CommandListElements::Element type);
	QString getListDefaultVisibleTrigger(CommandListElements::Element type);
	bool getListDefaultShowIcon(CommandListElements::Element type);
	QString getListDefaultIcon(CommandListElements::Element type);

	QStringList getListTriggers(CommandListElements::Element type);
	bool getListShowIcon(CommandListElements::Element type);
	QString getListIcon(CommandListElements::Element type);
	QString getListVisibleTrigger(CommandListElements::Element type);

	QHash<CommandListElements::Element, VoiceInterfaceCommand*> listInterfaceCommands;

	void registerVoiceInterfaceCommand(CommandListElements::Element, 
			const QStringList& triggers, 
			const QString& visibleTrigger, bool showIcon, const QString& iconSrc);


public slots:
	virtual void save();
	virtual void load();
	virtual void defaults();
 
private slots:
	void slotChanged();
	void listActionsItemChanged(QListWidgetItem *newItem, QListWidgetItem *item);

public:
	QHash<CommandListElements::Element, VoiceInterfaceCommand*> getListInterfaceCommands();
	static CommandSettings* getInstance(QWidget *parent=0, const QVariantList& args=QVariantList())
	{
		if (!instance) return new CommandSettings(parent, args);
		return instance;
	}

	CommandSettings(QWidget* parent=0, const QVariantList& args=QVariantList());

	float minimumConfidence();
	bool useDYM();
	QFont pluginBaseFont();
	
	~CommandSettings();
};

#endif

