//
// C++ Interface: returntablewidget
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RETURNTABLEWIDGET_H
#define RETURNTABLEWIDGET_H

#include <QTableWidget>

class QKeyEvent;
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ReturnTableWidget : public QTableWidget
{
    Q_OBJECT
public:
        ReturnTableWidget(QWidget* parent=0);

        ~ReturnTableWidget();

protected:
    virtual void keyPressEvent(QKeyEvent *e);

signals:
    void returnPressed();

};

#endif
