/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
 *   Copyright (C) 2012 Peter Grasch <grasch@simon-listens.org>
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
#include <simoncontextdetection/contextmanager.h>
#include "samplegroupitemdelegate.h"

class SampleGroupCondition;

namespace Ui {
    class SampleGroupContext;
}

class SampleGroupContextSettings : public QWidget
{
    Q_OBJECT

public:
    explicit SampleGroupContextSettings(QWidget *parent = 0);
    ~SampleGroupContextSettings();

    Condition* getCurrentCondition();

private:
    Ui::SampleGroupContext *ui;

    SampleGroupCondition* m_editCopyOfSampleGroupCondition;
    SampleGroupItemDelegate* m_sampleGroupDelegate;

signals:
    void changed();

public slots:
    void addCondition();
    void editCondition();
    void removeCondition();

    void saveChanges();
};

#endif // SAMPLEGROUPCONTEXT_H
