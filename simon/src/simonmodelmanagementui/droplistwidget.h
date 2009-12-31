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


#ifndef SIMON_DROPLISTWIDGET_H_6141C4CF79E6474E97A308426C551198
#define SIMON_DROPLISTWIDGET_H_6141C4CF79E6474E97A308426C551198

#include <KListWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

/**
 *	@class DropListWidget
 *	@brief Extends the QListWidget with Drop functions
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
class DropListWidget : public KListWidget
{
	Q_OBJECT
signals:
	void droppedText(QString text);
protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
public:
    DropListWidget(QWidget *parent);

    ~DropListWidget();

};

#endif
