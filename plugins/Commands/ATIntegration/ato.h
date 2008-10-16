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

#ifndef ATO_H
#define ATO_H
#include <QMetaType>

 struct ATOLocation
 {
     qint32 x;
     qint32 y;
     qint32 width;
     qint32 height;
 };
Q_DECLARE_METATYPE(ATOLocation)
 
 struct ATOPosition
 {
     qint32 x;
     qint32 y;
 };
Q_DECLARE_METATYPE(ATOPosition)

enum ATOMenuType {
	None=0,
	MenuBar=1,
	ToolBar=2,
 	Entry=3
};
struct ATOMenu {
	QString title;
	ATOMenuType type;
	ATOLocation geometry;
	QList<ATOMenu*> actions;
};

#endif

