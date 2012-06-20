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

#ifndef CONDITIONASSOCIATION_H
#define CONDITIONASSOCIATION_H

/** \file conditionassociation.h
 * \brief The file containing the ConditionAssociation baseclass header.
 */

#include "condition.h"
#include "simoncontextdetection_export.h"

/**
 *	@class ConditionAssociation
 *	@brief The ConditionAssociation class provides the abstract interface for condition plugins that monitor a logical assocaition of several conditions
 *
 *      ConditionAssociation is a convenient base class for creating condition plugins that base their
 *      state on a logical association of several other conditions.  For example, the OrConditionAssociation
 *      plugin monitors a group of conditions, and if any of the conditions evaluates to true, the OrConditionAssociation
 *      will evaluate to true.  Likewise, the AndConditionAssociation plugin will only evaluate to true if all
 *      of the conditions that it monitors evaluate to true.
 *
 *	\sa Condition, CreateConditionWidget, OrConditionAssociation
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */
class SIMONCONTEXTDETECTION_EXPORT ConditionAssociation : public Condition
{
    Q_OBJECT
public:
    /**
     * \brief Constructor
     *
     */
    explicit ConditionAssociation(QObject *parent, const QVariantList &args);
    ~ConditionAssociation();

    /**
     *
     * \return A list of pointers to the ConditionAssociation's component Condition objects
     */
    QList<Condition*> getConditions() { return m_conditions; } ;

protected:
    /// A list of pointers to the ConditionAssociation's component Condition objects
    QList<Condition*> m_conditions;

    /**
     * \brief Deserializes the ConditionAssociation-specific elements of a Condition
     *
     * This provides a privateDeSerialize implementation for all ConditionAssociation Condition plugins.
     * The plugin-specific specification of ConditionAssociation Condition plugins is simply a list
     * of their component Condition objects.  This deSerializes those componenent Condition objects.
     *
     * \return If the Condition was successfully deSerialized: True
     * \return Otherwise: False
     */
    bool privateDeSerialize(QDomElement elem);

    /**
     * \brief Serializes the ConditionAssociation-specific elements of a Condition
     *
     * This provides a privateSerialize implementation for all ConditionAssociation Condition plugins.
     * The plugin-specific specification of ConditionAssociation Condition plugins is simply a list
     * of their component Condition objects.  This serializes those componenent Condition objects.
     *
     * \return If the Condition was successfully serialized: True
     * \return Otherwise: False
     */
    QDomElement privateSerialize(QDomDocument *doc, QDomElement elem);

signals:

public slots:

    /**
     * \brief Abstract slot for evaluating the ConditionAssociation
     *
     */
    virtual void evaluateConditions()=0;
};

#endif // CONDITIONASSOCIATION_H
