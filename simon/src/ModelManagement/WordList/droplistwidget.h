//
// C++ Interface: droplistwidget
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DROPLISTWIDGET_H
#define DROPLISTWIDGET_H

#include <KListWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include "../../SimonLib/SimonInfo/simoninfo.h"

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
