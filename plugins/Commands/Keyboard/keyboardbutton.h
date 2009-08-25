/*
 *   Copyright (C) 2009 Mario Strametz strmam06@htl-kaindorf.ac.at
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

#ifndef KEYBOARDBUTTON_H
#define KEYBOARDBUTTON_H

#include <QString>
#include <KPushButton>

class KeyboardButton : public KPushButton
{
private:
QString triggerShown;
QString triggerReal;
short valueType; // 0 = text; 1 = shortcut; no bool cause scaleability
QString value;

public:
KeyboardButton(QString triggerShown, QString triggerReal, short valueType, QString value);
QString getTriggerReal();
short getValueType();
QString getValue();
};

#endif
