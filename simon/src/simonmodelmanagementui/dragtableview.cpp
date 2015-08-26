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

#include "dragtableview.h"
#include <QApplication>
#include <KI18n/klocalizedstring.h>
#include <simoninfo/simoninfo.h>
#include <QMimeData>

/**
 * @brief Constructor
 *
 * Empty constructor - calls the element initializer of the
 * QTableView
 *
 * @author Peter Grasch
 */
DragTableView::DragTableView(QWidget *parent)
: QTableView(parent)
{ }

/**
 * @brief MousePress-Event
 *
 * Overrides the default behavior of the mousePressEvent().
 *
 * Stores the Position where the mouse was pressed in the member
 * mousePos
 *
 * @author Peter Grasch
 * @param QMouseEvent *event
 * Is used to determine the button that was pressed
 */
void DragTableView::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    this->mousePos= event->pos();
  QTableView::mousePressEvent(event);

}


/**
 * @brief MouseMove-Event
 *
 * Overrides the default behavior of the mouseMoveEvent().
 * Calculates the length Difference between the member mousePos
 * and the actual position (the Position of the event parameter)
 *
 * If the difference is larger than QApplication::startDragDistance()
 * if calls startDrag()
 *
 * @author Peter Grasch
 * @param QMouseEvent *event
 * Is used to determine the button that was pressed
 */
void DragTableView::mouseMoveEvent(QMouseEvent *event)
{
  if (!(event->buttons() & Qt::LeftButton)) {
    return;
  }
  if ((this->mousePos - event->pos()).manhattanLength() < QApplication::startDragDistance()) {
    return;
  }
  startDrag(Qt::CopyAction);
}


/**
 * @brief startDrag
 *
 * Utilizes the QDrag Object to store the data of the first
 * cell in the current row.
 *
 * Displays a OSD-Message:
 * 	"Drag the Word to the List on the right to train it"
 *
 * It uses the text-plain mimetype
 *
 * @author Peter Grasch, Phillip Goriup
 */
void DragTableView::startDrag(Qt::DropActions actions)
{
  QDrag *drag = new QDrag ( this );
  QMimeData *mimeData = new QMimeData();

  mimeData->setData( "application/x-simon-word", QByteArray() );
  drag->setMimeData(mimeData);

  SimonInfo::showMessage( i18n("Drag the word to the list on the right to train it") , 2000 ); // krazy:exclude=qmethods

  drag->exec(actions);
}


/**
 * @brief Destructor
 *
 * @author Peter Grasch
 */
DragTableView::~DragTableView()
{
}
