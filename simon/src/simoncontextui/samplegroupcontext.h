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

#ifndef SAMPLEGROUPCONTEXT_H
#define SAMPLEGROUPCONTEXT_H

#include <QWidget>
#include "simoncontextui_export.h"
#include "simoncontextdetection/contextmanager.h"
#include "simoncontextdetection/samplegroupconditionmodel.h"
#include "samplegroupitemdelegate.h"

namespace Ui {
    class SampleGroupContext;
}

class SIMONCONTEXTUI_EXPORT SampleGroupContext : public QWidget
{
    Q_OBJECT

public:
    explicit SampleGroupContext(QWidget *parent = 0);
    ~SampleGroupContext();

    Condition* getCurrentCondition();

private:
    Ui::SampleGroupContext *ui;

    SampleGroupConditionModel* m_model;
    SampleGroupItemDelegate* m_sampleGroupDelegate;

public slots:
    void addCondition();
    void editCondition();
    void removeCondition();
    void promoteCondition();
    void demoteCondition();
};

#endif // SAMPLEGROUPCONTEXT_H
