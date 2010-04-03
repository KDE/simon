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


#ifndef SIMON_SCENARIOMANAGEMENTDIALOG_H_FA6CC60BFE674C5DA2F42DE2132BC4C0
#define SIMON_SCENARIOMANAGEMENTDIALOG_H_FA6CC60BFE674C5DA2F42DE2132BC4C0

#include <KDialog>
#include <QList>
#include <QModelIndex>

#include "simonscenarioui_export.h"

class Scenario;
class QListWidget;
class QListWidgetItem;

namespace Ui {
	class ScenarioManagementDialog ;
}

class SIMONSCENARIOUI_EXPORT ScenarioManagementDialog : public KDialog    {
	Q_OBJECT

private:
	Ui::ScenarioManagementDialog *ui;

	QModelIndex m_lastSelectedIndex;

	QString m_dataPrefix;
	bool m_dirty;

	void initDisplay();
	void displayScenario(Scenario *scenario, QListWidget* widget);
	void setupItemToScenario(QListWidgetItem *item, Scenario *scenario);

	Scenario* getCurrentlySelectedScenario();

	bool save();

private slots:
	void availableScenarioSelected();
	void selectedScenarioSelected();

	void newScenario();
	void editScenario();
	void importScenario();
	void exportScenarioFile();
	void exportScenarioGHNS();
	void deleteScenario();

	void updateLastSelectedIndex(const QModelIndex&);
	void slotAdded(QListWidgetItem*);
	void slotMovedDown(QListWidgetItem*);
	void slotMovedUp(QListWidgetItem*);
	void slotRemoved(QListWidgetItem*);
	
public slots:
	int exec();
	bool updateScenarioConfiguration();

public:
	bool getNewScenarios();
	void init();
	explicit ScenarioManagementDialog(const QString& dataPrefix, QWidget *parent = 0);
	QStringList getSelectedScenarioIds();

	~ScenarioManagementDialog();

};

#endif

