//
// C++ Interface: inlinewidget
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INLINEWIDGET_H
#define INLINEWIDGET_H

#include <QWidget>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class InlineWidget : public QWidget
{
public:
    InlineWidget(QWidget* parent, Qt::WindowFlags f);

    ~InlineWidget();

};

#endif
