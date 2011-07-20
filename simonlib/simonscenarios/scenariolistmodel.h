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

#ifndef SCENARIOLISTMODEL_H
#define SCENARIOLISTMODEL_H

/** \file scenariolistmodel.h
 * \brief The file containing the ScenarioListModel baseclass header.
 */

#include <QString>
#include <QAbstractItemModel>
#include "simonmodelmanagement_export.h"
#include "scenario.h"

/**
 *	@class ScenarioListModel
 *	@brief The ScenarioListModel class provides a view-compatible model of all the available scenarios
 *
 *      This class is very simple, if not useless right now, but it can be expanded to retrieve the list of
 *      scenarios that can be imported as well as those that are directly available.
 *
 *	\sa Scenario, QAbstractItemModel, ContextViewPrivate
 *
 *	@version 0.1
 *	@date 7.18.2011
 *	@author Adam Nash
 */

class MODELMANAGEMENT_EXPORT ScenarioListModel : public QAbstractItemModel
{

  protected:
    /**
     * \brief The list of available scenario IDs
     *
     */
    QStringList m_scenarioIds;

    /**
     *
     * \return The index of the item in the model with the given \var row, \var column, and \var parent.
     */
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;

    /**
     *
     * \return The item flags for the given \var index
     */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /**
     *
     * \return The data for the given \var role in the header with the specified \var orientation.
     */
    QVariant headerData(int, Qt::Orientation orientation,
      int role = Qt::DisplayRole) const;

    /**
     *
     * \return The index of the parent of the item with the given \var index
     */
    QModelIndex parent(const QModelIndex &index) const;

  protected:
    /**
     *
     * \return The number of rows under the given \var parent.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     *
     * \return The data stored under the given \var role for the item referred to by \var index.
     */
    virtual QVariant data(const QModelIndex &index, int role) const;

    /**
     *
     * \return The number of columns under the given \var parent.
     */
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

  public:
    /**
     * \brief Constructor
     *
     *
     */
    ScenarioListModel();

    /**
     * \brief Destructor
     *
     */
    ~ScenarioListModel();

    /**
     * \brief Updates the model to be in sync with ScenarioManager
     *
     */
    void update();

};

#endif // SCENARIOLISTMODEL_H
