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


#ifndef SIMON_DROPLISTVIEW_H_0F6FC9EE19EA4F1F99049E964AE7114F
#define SIMON_DROPLISTVIEW_H_0F6FC9EE19EA4F1F99049E964AE7114F

#include <QListView>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

/**
 *	@class DropListView
 *	@brief Extends the QListView with Drop functions
 *
 *	Implements the Drop- (from Drag and Drop) functions:
 *		dragEnterEvent(QDragEnterEvent)
 *		dropEvent(QDragEvent)
 *		dragMoveEvent(QDrapMoveEvent)
 *
 *	@version 0.1
 *	@date 28.01.2006
 *	@author Peter Grasch
 */
class DropListView : public QListView
{
	Q_OBJECT
signals:
	void droppedText(QString text);
protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
public:
    DropListView(QWidget *parent);

    ~DropListView();

};

#endif
