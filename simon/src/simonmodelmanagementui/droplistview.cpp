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

#include "droplistview.h"

/**
 * @brief Constructor
 *
 * Setzt acceptDrops auf true um die Drops zu erlauben
 *
 * @author Peter Grasch
 * @param QView *parent
 * Sets the parent of the KListView
 */
DropListView::DropListView(QWidget *parent)
: QListView(parent)
{
  setAcceptDrops(true);
}


/**
 * @brief Drag-Enter-Events
 *
 * Is activated when the Object is entered and only if there is
 * an "open drag"
 * The event is accepted
 *
 * @author Peter Grasch
 * @param QDragEnterEvent *event
 * Accepts the event
 */
void DropListView::dragEnterEvent(QDragEnterEvent *event)
{
  event->accept();
}


/**
 * @brief Drag-Move-Event
 *
 * If the event is of the mime-type the drag is accepted
 *
 * @author Peter Grasch
 * @param QDragMoveEvent *event
 * If the event has the MIME-Type text/plain the event is accepted
 */
void DropListView::dragMoveEvent(QDragMoveEvent *event)
{
  if (event->mimeData()->hasFormat("application/x-simon"))
    event->accept();
}


/**
 * @brief Drop Event
 *
 * Adds the text of the event (parameter) as new item to the
 * QList (*this)
 *
 * @author Peter Grasch
 * @param QDropEvent *event
 * The event->mimeData contains the Dragged data
 */
void DropListView::dropEvent(QDropEvent *)
{
  emit droppedText();
}


/**
 * @brief Destructor
 *
 * @author Peter Grasch
 */
DropListView::~DropListView()
{
}
