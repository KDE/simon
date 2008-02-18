//
// C++ Implementation: dragtablewidget
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dragtablewidget.h"
#include <QApplication>

/**
 * @brief Constructor
 *
 * Empty constructor - calls the element initializer of the
 * QTableWidget
 *
 * @author Peter Grasch
 */
DragTableWidget::DragTableWidget(QWidget *parent)
	: QTableWidget(parent)
{	}


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
void DragTableWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		this->mousePos= event->pos();
	QTableWidget::mousePressEvent(event);
	
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
void DragTableWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (!(event->buttons() & Qt::LeftButton))
	{
		return;
	}
	if ((this->mousePos - event->pos()).manhattanLength() < QApplication::startDragDistance())
	{	
		return;
	}
	startDrag();
}

/**
 * @brief startDrag
 *
 * Utilizes the QDrag Object to store the data of the first
 * cell in the current row.
 * 
 * Displays a OSD-Message:
 * 	"Ziehe das Wort in die Liste rechts um es zu trainieren"
 *
 * It uses the text-plain mimetype
 *
 * @author Peter Grasch / Phillip Goriup
 */
void DragTableWidget::startDrag()
{
	QDrag *drag = new QDrag ( this );
	QMimeData *mimeData = new QMimeData();
	if(this->currentItem())
	{
		QString currentItem( this->item(this->currentRow(),0)->text() );
		
		mimeData->setText( currentItem );
		drag->setMimeData(mimeData);
	
		SimonInfo::showMessage( tr("Ziehe das Wort in die Liste rechts um es zu trainieren") , 2000 );
	
		drag->start(Qt::MoveAction);
	}
}



/**
 * @brief Destructor
 *
 * @author Peter Grasch
 */
DragTableWidget::~DragTableWidget()
{
}
