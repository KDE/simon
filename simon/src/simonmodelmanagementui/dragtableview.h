/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_DRAGTABLEVIEW_H_AB453F4C9942440EA755D7A55A42ECB3
#define SIMON_DRAGTABLEVIEW_H_AB453F4C9942440EA755D7A55A42ECB3

#include <QTableView>
#include <QMouseEvent>
#include <QDrag>

/**
 *	@class DragTableView
 *	@brief Extends the QTableView with Drag Functions
 *
 *	Implements the Drag- (from Drag and Drop) functions:
 *		mousePressEvent(QMouseEvent)
 *		mouseMoveEvent(QMouseEvent)
 *		startDrag()
 *			Creates the QDrag Object and starts the
 *			dragging operation with it
 *
 *	@version 0.1
 *	@date 28.01.2006
 *	@author Peter Grasch
 */
class DragTableView : public QTableView
{
  Q_OBJECT
    private:
    QPoint mousePos;                              //!< Holds the mouse position where the mouse was pressed
  protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void startDrag(Qt::DropActions);
  public:
    DragTableView(QWidget *parent);

    ~DragTableView();

};
#endif
