/*   Copyright (C) 2010 Grasch Peter <peter.grasch@bedahr.org>
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

#ifndef TIMESELECTOR_H
#define TIMESELECTOR_H

#include <QWidget>
#include "akonadicommand.h"
#include "ui_timeselector.h"

class TimeSelector : public QWidget
{
private:
  Ui::Form ui;

public:
    explicit TimeSelector(QWidget* parent = 0, Qt::WindowFlags f = 0);
    int getTime() const;
    
    static void getRelativeTime(int seconds, AkonadiCommand::RelativeDurationDimension& dimension, int& value);
    void setTime(int value);
    void setTime(AkonadiCommand::RelativeDurationDimension dimension, int value);
};

#endif // TIMESELECTOR_H
