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
#include "statusmanager.h"
#include "compositeprogresswidget.h"
#include "operation.h"


#include <KLocalizedString>

StatusManager* StatusManager::globalInstance;

StatusManager::StatusManager(QObject *parent) : QObject(parent)
{
  qRegisterMetaType<OperationList>("OperationList");
  qRegisterMetaType<OperationPtr>("OperationPtr");
}


CompositeProgressWidget* StatusManager::createWidget(QWidget *parent)
{
  CompositeProgressWidget* widget = new CompositeProgressWidget(parent);
  widget->display(runningOperations);
  connect(this, SIGNAL(operationsChanged(OperationList)),
    widget, SLOT(display(OperationList)));
  return widget;
}


void StatusManager::update()
{
  emit operationsChanged(runningOperations);
}


void StatusManager::registerOperation(Operation* operation)
{
  runningOperations.append(operation);
  emit operationsChanged(runningOperations);
}


void StatusManager::removeOperation(Operation* operation)
{
  runningOperations.removeAll(operation);
  emit operationsChanged(runningOperations);
}


StatusManager::~StatusManager()
{
}
