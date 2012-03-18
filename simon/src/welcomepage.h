/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SIMON_WELCOMEPAGE_H_47AEB5F97F5246E8BAC891DD0B65687A
#define SIMON_WELCOMEPAGE_H_47AEB5F97F5246E8BAC891DD0B65687A

#include <simonuicomponents/inlinewidget.h>
#include "ui_welcomepage.h"

class QShowEvent;
class QHideEvent;

class WelcomePage : public InlineWidget
{
public:
    WelcomePage(QWidget *parent=0);
    
protected:
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
    
private:
    Ui::WelcomePage ui;
};

#endif // WELCOMEPAGE_H
