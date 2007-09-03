//
// C++ Implementation: returntablewidget
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "returntablewidget.h"
#include <QStringList>
#include <QMessageBox>

ReturnTableWidget::ReturnTableWidget(QWidget* parent): QTableWidget(parent)
{
    this->setSelectionMode(QAbstractItemView::SingleSelection);
}

ReturnTableWidget::~ReturnTableWidget()
{}

void ReturnTableWidget::keyPressEvent(QKeyEvent *e)
{
  if (e->key()==Qt::Key_Return)        //i have no notion, why i have to add 1 to the e->key(), that it gaves the same value as Qt::Key_Enter
    {
        emit returnPressed();
    }
}



