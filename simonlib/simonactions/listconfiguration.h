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

#ifndef SIMON_LISTCONFIGURATION_H_92014DF656EC423699D2F493D77108BA
#define SIMON_LISTCONFIGURATION_H_92014DF656EC423699D2F493D77108BA

#include "commandlistwidget.h"
#include "actionlib_export.h"
#include <simonscenarios/commandlistelements.h>
#include <QStringList>
#include <QList>
#include <QHash>
#include <QWidget>

namespace Ui
{
	class ListConfigurationDlg;
}

class QListWidgetItem;
class Action;
class VoiceInterfaceCommand;

class SIMONACTIONS_EXPORT ListConfiguration : public QWidget
{
Q_OBJECT

signals:
	void changed();

private:
	Ui::ListConfigurationDlg* ui;

	CommandListElements::Element getRowElementType(int row);
	void storeCurrentlyDisplayedElement(CommandListElements::Element type);
	void displayListElement(CommandListElements::Element type);

	QHash<CommandListElements::Element, VoiceInterfaceCommand*> listInterfaceCommands;

public slots:
	void prepareToSave();
	void prepareToLoad();
	void loadFinished();
 
private slots:
	void listActionsItemChanged(QListWidgetItem *newItem, QListWidgetItem *item);

public:
	ListConfiguration(QWidget* parent=0);
	QHash<CommandListElements::Element, VoiceInterfaceCommand*> getListInterfaceCommands();

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

	void registerVoiceInterfaceCommand(CommandListElements::Element, 
			const QStringList& triggers, 
			const QString& visibleTrigger, bool showIcon, const QString& iconSrc);
	~ListConfiguration();



};

#endif


