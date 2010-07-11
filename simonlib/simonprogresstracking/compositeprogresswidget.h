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
#ifndef SIMON_COMPOSITEPROGRESSWIDGET_H_F41F1BB39FF5470EA4B07C5FEB1F1414
#define SIMON_COMPOSITEPROGRESSWIDGET_H_F41F1BB39FF5470EA4B07C5FEB1F1414

#include <QWidget>

#include "operation.h"

class QLabel;
class QFrame;
class QProgressBar;
class ProgressWidget;
class KPushButton;
class QMoveEvent;

/**
\class CompositeProgressWidget
\version 0.1
\date 9.11.2008
\brief Displays several Operations in a nice way
\author Peter Grasch

*/
class CompositeProgressWidget : public QWidget
{
  Q_OBJECT

    private:
    QWidget *designatedParent;

    QLabel *statusLabel;
    QProgressBar *bar;
    QFrame *popupWidget;
    KPushButton *togglePopup;
    QList<ProgressWidget*> progressWidgets;

  protected:
    void moveEvent(QMoveEvent *event);

  public:
    CompositeProgressWidget(QWidget* parent=0);

    ~CompositeProgressWidget();

  private slots:
    void showDetails(bool show);

  public slots:
    void display(OperationList operations);

};
#endif
