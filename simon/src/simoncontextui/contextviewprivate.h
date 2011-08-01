/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#ifndef SIMON_CONTEXTVIEWPRIVATE_H_819E2A6755D948FA917DEBAE89DA9F2D
#define SIMON_CONTEXTVIEWPRIVATE_H_819E2A6755D948FA917DEBAE89DA9F2D

/** \file contextviewprivate.h
 * \brief The file containing the ContextViewPrivate baseclass header.
 */

#include "ui_contextdialog.h"
#include <simoncontextdetection/condition.h>
#include <simonscenarios/scenariodisplay.h>
#include <KDE/KIcon>

class Condition;
class QSortFilterProxyModel;

/**
 *	@class ContextViewPrivate
 *	@brief The ContextViewPrivate class provides a graphical frontend for changing the context requirements of a scenario
 *
 *      Conditions that are required in order for a scenario to be active can be added, editted and deleted with
 *      the UI elements provided by ContextViewPrivate.
 *
 *      \sa ContextView, Scenario, Condition
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class ContextViewPrivate : public QWidget, public ScenarioDisplay
{
  Q_OBJECT

    signals:
  void actionsChanged();

  private:
    Ui::ContextDlg ui;

    Condition* getCurrentCondition();
    QSortFilterProxyModel *conditionsProxy;

    void updateInvalidChildList();
    void addChildScenarios(QTreeWidgetItem* parentItem, Scenario* parentScenario);

  private slots:
    void addCondition();
    void deleteCondition();
    void editCondition();
    void selectionChanged();

    //void managePlugIns();

  public:
    void displayScenarioPrivate(Scenario *scenario);

    ContextViewPrivate(QWidget *parent);
    ~ContextViewPrivate();

};
#endif
