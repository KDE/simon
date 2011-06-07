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

#include "createconditionwidget.h"
#include "condition.h"
#include "compoundcondition.h"

bool CreateConditionWidget::addCondition()
{
    return m_compoundCondition->addCondition(createCondition());
}

bool CreateConditionWidget::isInstanceOfSameCondition(Condition *condition)
{
    CreateConditionWidget* widget = condition->getCreateConditionWidget(0, 0);

    if (widget->windowTitle() == this->windowTitle())
    {
        delete widget;
        return true;
    }
    else
    {
        delete widget;
        return false;
    }
}
