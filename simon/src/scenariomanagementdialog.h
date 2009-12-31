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


#ifndef SCENARIOMANAGEMENTDIALOG_H
#define SCENARIOMANAGEMENTDIALOG_H

#include <KDialog>
#include <QList>
#include <QModelIndex>

#include "ui_scenariomanagementdlg.h"

class Scenario;

class ScenarioManagementDialog : public KDialog    {
	Q_OBJECT

private:
	Ui::Dialog ui;

	QModelIndex m_lastSelectedIndex;

	bool m_dirty;

	void initDisplay();
	void displayScenario(Scenario *scenario, QListWidget* widget);
	void setupItemToScenario(QListWidgetItem *item, Scenario *scenario);

	Scenario* getCurrentlySelectedScenario();

	void save();

private slots:
	void availableScenarioSelected();
	void selectedScenarioSelected();

	void newScenario();
	void editScenario();
	void importScenario();
	void exportScenarioFile();
	void exportScenarioGHNS();
	void getNewScenarios();
	void deleteScenario();

	void updateLastSelectedIndex(const QModelIndex&);
	void slotAdded(QListWidgetItem*);
	void slotMovedDown(QListWidgetItem*);
	void slotMovedUp(QListWidgetItem*);
	void slotRemoved(QListWidgetItem*);
	
public slots:
	int exec();

public:
	explicit ScenarioManagementDialog(QWidget *parent = 0);

	~ScenarioManagementDialog();

};

#endif

