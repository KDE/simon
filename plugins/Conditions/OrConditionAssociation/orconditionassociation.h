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

#ifndef ORCONDITIONASSOCIATION_H
#define ORCONDITIONASSOCIATION_H

/** \file orconditionassociation.h
 * \brief The file containing the OrConditionAssociation baseclass header.
 */

#include "simoncontextdetection/conditionassociation.h"
#include "simoncontextdetection/processinfo.h"
#include "simoncontextdetection/simoncontextdetection_export.h"

/**
 *	@class OrConditionAssociation
 *	@brief The OrConditionAssociation class is a condition plugin that monitors whether or not at least one of several other conditions is satisfied
 *
 *      The OrConditionAssociation class is a ConditionAssociation that is satisfied if one of its Condition objects (it monitors a group
 *      of other Condition objects) is satisfied.  In other words, its satisfaction is the logical or of all of its component Condition
 *      satisfactions.  Whenever the satisfaction of one of its component Condition objects changes, it reevaluates itself.
 *
 *      Just like any other condition, the OrConditionAssociation will accordingly update its \ref Condition::m_satisfied value and then emit its \ref Condition::conditionChanged()
 *      signal whenever it becomes newly satisfied or unsatisfied.
 *
 *      \sa Condition, ConditionAssociation, CreateOrConditionAssociationWidget, NewAssociationCondition
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class OrConditionAssociation : public ConditionAssociation
{
    Q_OBJECT
public:
    explicit OrConditionAssociation(QObject *parent, const QVariantList& args);

    virtual QString name();

    virtual CreateConditionWidget* getCreateConditionWidget(CompoundCondition *compoundCondition, QWidget *parent);

private:

signals:

protected slots:
    void evaluateConditions();
};

#endif // ORCONDITIONASSOCIATION_H
