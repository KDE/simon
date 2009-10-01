/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_RUNCOMMANDVIEW_H_D0DB76CE9BB745EDBDBF8BB975742138
#define SIMON_RUNCOMMANDVIEW_H_D0DB76CE9BB745EDBDBF8BB975742138

#include "inlinewidget.h"
#include "simonactionsui_export.h"

class RunCommandViewPrivate;

class SIMONACTIONSUI_EXPORT RunCommandView : public InlineWidget {
	Q_OBJECT

private:
	RunCommandViewPrivate *d;

public slots:
// 	void loadCommands();

public:
    RunCommandView(QWidget *parent);

    ~RunCommandView();

};

#endif
