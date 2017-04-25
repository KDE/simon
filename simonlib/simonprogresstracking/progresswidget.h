/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_PROGRESSWIDGET_H_A90BC5523CFA444DB2AEA79A1C65920F
#define SIMON_PROGRESSWIDGET_H_A90BC5523CFA444DB2AEA79A1C65920F

#include <QWidget>
#include <QPointer>
#include <QSharedPointer>
#include "simonprogresstracking_export.h"

class Operation;
class QLabel;
class QPushButton;
class QProgressBar;
class QPushButton;

/**
  @author
*/
class SIMONPROGRESSTRACKING_EXPORT ProgressWidget : public QWidget
{
  Q_OBJECT
    private:
    QPointer<Operation> op;
    QLabel *name;
    QLabel *currentAction;
    QProgressBar *bar;
    QPushButton *cancelButton;

  public slots:
    void update();

  public:

    enum ProgressWidgetStyle
    {
      Compact=1,
      Large=2
    };

    explicit ProgressWidget(QPointer<Operation> op, ProgressWidgetStyle style = Compact, QWidget* parent=0);

    QPointer<Operation> operation() { return op; }

    ~ProgressWidget();

};
#endif
