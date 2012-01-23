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

#ifndef SIMON_COMPOUNDCONDITIONMODEL_H_3EBF59D79A3C46A1B84DE19FA1269213
#define SIMON_COMPOUNDCONDITIONMODEL_H_3EBF59D79A3C46A1B84DE19FA1269213

/** \file compoundconditionmodel.h
 * \brief The file containing the CompoundConditionModel baseclass header.
 */

#include <QString>
#include <QAbstractItemModel>
#include "simoncontextdetection_export.h"

class CompoundCondition;
class Condition;


/**
 *	@class CompoundConditionModel
 *	@brief The CompoundConditionModel class provides a view-compatible model of the CompoundCondition class
 *
 *	Every CompoundCondition has a proxy CompoundConditionModel which provides a simple way of
 *      displaying the contents of the CompoundCondition in a view object such as a QListView.
 *
 *      The CompoundConditionModel is used in ContextViewPrivate in order to display the Conditions
 *      that are in a specified CompoundCondition.
 *
 *	\sa CompoundCondition, QAbstractItemModel, ContextViewPrivate
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */
class SIMONCONTEXTDETECTION_EXPORT CompoundConditionModel : public QAbstractItemModel
{

  protected:
    /**
     * \brief The list of conditions contained in the model's CompoundCondition
     *
     */
    QList<Condition*> m_conditions;

    /**
     * \brief The model's CompoundCondition
     *
     */
    CompoundCondition *m_ref;

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
     * Initializes the model's CompoundCondition (\ref m_ref) as \var base
     * and calls \ref update().
     *
     */
    CompoundConditionModel(CompoundCondition *base);

    /**
     * \brief Destructor
     *
     */
    ~CompoundConditionModel();

  public slots:
    /**
     * \brief Updates the model to be in sync with its CompoundCondition
     *
     */
    void update();

};
#endif
