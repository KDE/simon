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

#ifndef CONDITION_H
#define CONDITION_H

/** \file condition.h
 * \brief The file containing the Condition baseclass header.
 */

#include <QObject>
#include <QtXml/QDomElement>
#include <QVariant>
#include "simoncontextdetection_export.h"

class CreateConditionWidget;

/**
 *	@class Condition
 *	@brief The Condition class provides the abstract interface for condition plugins
 *
 *      Condition objects are used to monitor a condition that determines the context of the user.
 *      They monitor contexts such as whether or not a program is opened (see ProcessOpenedCondition)
 *      and send a signal whenever the monitored context changes.  This signal is sent to the CompoundCondition
 *      that holds the condition and the CompoundCondition is reevaluated.
 *
 *      Condition plugins must derive a class from Condition (it is the plugin interface class).
 *      Condition plugins must also derive a class from CreateConditionWidget (in which the user interface for creating the condition is established)
 *
 *      In addition to re-implementing the pure virtual methods of Condition, a Condition plugin must
 *      change its \ref m_satisfied variable and then emit the \ref conditionChanged() signal (in that order) whenever
 *      the context being monitored by the Condition plugin changes.
 *
 *      Conditions are managed by the ContextManager singleton class.
 *
 *	\sa CompoundCondition, ContextManager, CreateConditionWidget, ProcessOpenedCondition
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class SIMONCONTEXTDETECTION_EXPORT Condition : public QObject
{
    Q_OBJECT
public:
    /**
     * \brief Constructor
     *
     */
    explicit Condition(QObject *parent, const QVariantList &args);

    /**
     * \brief The current satisfaction of the Condition
     *
     * \return If the CompoundCondition is satisfied: True
     * \return Otherwise: False
     */
    bool isSatisfied();

    /// \return whether or not the condition is inverted
    bool isInverted() {return m_inverted;}

    /**
     * \brief Serializes the Condition
     *
     * This function serializes the Condition.  It uses the QDomDocument \var doc.
     *
     * \return A QDomElement specification of the Condition
     */
    QDomElement serialize(QDomDocument *doc);

    /**
     * \brief DeSerializes the Condition from a QDomElement specification
     *
     * DeSerializes the Condition from the QDomElement specification \var elem.
     *
     * \return If the Condition was successfully deSerialized: True
     * \return Otherwise: False
     */
    bool deSerialize(QDomElement elem);

    /**
     * \brief Abstract function for retrieving the CreateConditionWidget of a Condition plugin
     */
    virtual CreateConditionWidget* getCreateConditionWidget(QWidget *parent)=0;

    /**
     * \brief Abstract function for retrieving the name of a Condition plugin
     *
     *
     * \return The name of a specific Condition
     */
    virtual QString name()=0;

protected:
    /**
     * \brief Abstract function for DeSerializing the plugin-specific Condition characteristics from a QDomElement specification
     *
     * \param elem The QDomElement Condition specification that is to be deSerialized
     *
     * \return If the Condition was successfully deSerialized: True
     * \return Otherwise: False
     */
    virtual bool privateDeSerialize(QDomElement elem)=0;

    /**
     * \brief Abstract function for serializing the plugin-specific Condition characteristics to a QDomElement specification
     *
     * \param elem The general Condition QDomElement specification (ie. the non-plugin-specific specifications)
     * \param doc The QDomDocument used to create new elements
     *
     * \return If the Condition was successfully serialized: True
     * \return Otherwise: False
     */
    virtual QDomElement privateSerialize(QDomDocument *doc, QDomElement elem)=0;

    /// The boolean value that indicates whether or not the Condition is satisfied
    bool m_satisfied;

    /// The name of this Condition plugin
    QString m_pluginName;

private:
    /// The boolean value that indicates whether of not satisfaction of the Condition should be inverted
    bool m_inverted;

signals:
    /// Emitted when the satisfaction of the Condition changes
    void conditionChanged();

public slots:

};

#endif // CONDITION_H
