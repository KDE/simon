//
// C++ Implementation: droplistwidget
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "droplistwidget.h"

/**
 * @brief Constructor
 *
 * Setzt acceptDrops auf true um die Drops zu erlauben
 *
 * @author Peter Grasch
 * @param QWidget *parent
 * Sets the parent of the QListWidget
 */
DropListWidget::DropListWidget(QWidget *parent)
 : QListWidget(parent)
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
void DropListWidget::dragEnterEvent(QDragEnterEvent *event)
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
void DropListWidget::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat("text/plain"))
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
void DropListWidget::dropEvent(QDropEvent *event)
{
	emit droppedText( event->mimeData()->text() );
}


/**
 * @brief Destructor
 *
 * @author Peter Grasch
 */
DropListWidget::~DropListWidget()
{
}


