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

#ifndef SIMON_SCENARIOMANAGEMENTWIDGET_H_FA6CC60BFE674C5DA2F42DE2132BC4C0
#define SIMON_SCENARIOMANAGEMENTWIDGET_H_FA6CC60BFE674C5DA2F42DE2132BC4C0

#include <QWidget>
#include <QList>
#include <QModelIndex>

#include "simonscenarioui_export.h"

class Scenario;
class QTreeWidget;
class QTreeWidgetItem;

namespace Ui
{
  class ScenarioManagementDialog ;
}


class SIMONSCENARIOUI_EXPORT ScenarioManagementWidget : public QWidget
{
  Q_OBJECT

  private:
    Ui::ScenarioManagementDialog *ui;

    QModelIndex m_lastSelectedIndex;

    QString m_dataPrefix;
    bool m_dirty;

    void initDisplay();
    QTreeWidgetItem* displayScenario(Scenario *scenario, QTreeWidget* widget);
    void setupItemToScenario(QTreeWidgetItem *item, Scenario *scenario);

    Scenario* getCurrentlySelectedScenario();
    QStringList getChildScenarioIds(QTreeWidgetItem* parentItem);
    QStringList getAllLevelChildScenarioIds(QTreeWidgetItem* parentItem);

    void saveChildConfiguration(QTreeWidgetItem *parentItem);

    bool askExportFull(Scenario *s);

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
    void slotAdded();
    void slotMovedDown();
    void slotMovedUp();
    void slotRemoved();

    bool getNewScenarios();

  public:
    explicit ScenarioManagementWidget(const QString& dataPrefix, bool minimal, QWidget *parent = 0);
    QStringList getSelectedScenarioIds();
    void init();
    bool save();

    ~ScenarioManagementWidget();

};
#endif
