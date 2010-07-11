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

#ifndef SIMON_STATUSMANAGER_H_12966FF9A57D48DC97C91B2E61643660
#define SIMON_STATUSMANAGER_H_12966FF9A57D48DC97C91B2E61643660

#include "simonprogresstracking_export.h"

#include "operation.h"

#include <QObject>
#include <QList>

class CompositeProgressWidget;

class SIMONPROGRESSTRACKING_EXPORT StatusManager : public QObject
{
  Q_OBJECT

    signals:
  void operationsChanged(const OperationList&);

  private:
    QList<CompositeProgressWidget*> ui;
    OperationList runningOperations;
    static StatusManager *globalInstance;

  protected:
    StatusManager(QObject *parent=0);

  public:
    static StatusManager* global(QObject *parent=0) {
      if (!globalInstance) globalInstance = new StatusManager(parent);
      return globalInstance;
    }

    CompositeProgressWidget* createWidget(QWidget *parent=0);

    void registerOperation(Operation* operation);
    void removeOperation(Operation* operation);

    ~StatusManager();

    void update();

};
#endif
