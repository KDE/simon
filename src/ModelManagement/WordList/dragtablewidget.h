//
// C++ Interface: dragtablewidget
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DRAGTABLEWIDGET_H
#define DRAGTABLEWIDGET_H

#include <QTableWidget>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include "../../SimonLib/SimonInfo/simoninfo.h"


/**
 *	@class DragTableWidget
 *	@brief Extends the QTableWidget with Drag Functions
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
class DragTableWidget : public QTableWidget
{
	Q_OBJECT
private:
	QPoint mousePos; //!< Holds the mouse position where the mouse was pressed
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void startDrag(Qt::DropActions supportedActions);
public:
    DragTableWidget(QWidget *parent);

    ~DragTableWidget();

};

#endif
