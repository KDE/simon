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

#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H

/** \file contextmanager.h
 * \brief The file containing the ContextManager baseclass header.
 */

#include <QObject>
#include <QtXml/QDomElement>
#include <QHash>
#include "compoundcondition.h"
#include "simoncontextdetection_export.h"

/**
 *	@class ContextManager
 *	@brief The ContextManager class provides a means for loading and managing all of the Conditions that are active in simon
 *
 *      The ContextManager is a factory for Condition objects.  If a Condition that has already been loaded
 *      is requested with getCondition, the ContextManager returns a pointer to the already instantiated Condition.
 *
 *      The ContextManager also contains a function that returns intantiations of every available Condition plugin.
 *      This allows, for example, the ContextViewPrivate class to create a CreateConditionWidget for every available
 *      Condition plugin.
 *
 *      ContextManager is a singleton.
 *
 *	\sa Condition, CreateConditionWidget
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class SIMONCONTEXTDETECTION_EXPORT ContextManager : public QObject
{
    Q_OBJECT
public:
    /**
     * \return The instance of the ContextManager
     */
    static ContextManager* instance();

    /**
     * \brief Destructor
     *
     * Deletes all conditions that have been created.
     */
    ~ContextManager();

    /**
     * \brief The factory method for creating Condition objects
     *
     * Returns a condition based on the specifications in \var elem.  If the
     * Condition with those specification already exists, the existing Condition
     * is returned, otherwise, the Condition is newly created.
     *
     * \return If \var elem is valid: The Condition specified by elem
     * \return Otherwise: A NULL pointer
     */
    Condition* getCondition(const QDomElement &elem);

    /**
     * \return The list of all available Condition plugins
     */
    QList<Condition*>* getConditions();

    QDomElement getEmptyCondition(const QString &pluginName);

private:
    explicit ContextManager(QObject *parent = 0);
    
    static ContextManager* m_instance;
  
    QHash<QString, Condition*> m_conditionLookup;
    QList<Condition*> m_conditions;

signals:

public slots:
};

#endif // CONTEXTMANAGER_H
