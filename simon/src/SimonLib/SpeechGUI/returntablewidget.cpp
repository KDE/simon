/*
 *   Copyright (C) 2008 Phillip Goriup <goriup@simon-listens.org>
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


#include "returntablewidget.h"
#include <QStringList>
#include <QKeyEvent>

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



