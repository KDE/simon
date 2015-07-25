/*
 *   Copyright (C) 2014 Peter Grasch <peter.grasch@bedahr.org>
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

#include "macprocessinfogatherer.h"
#include "macprocessinfogathererhelper.h"
#include <QDebug>

MacProcessInfoGatherer::MacProcessInfoGatherer(QObject *parent) :
    ProcessInfoGatherer(parent)
{
}

void MacProcessInfoGatherer::checkCurrentProcesses()
{
  QStringList processes = MacProcessInfoGathererHelper::getCurrentProcesses();
  m_currentlyRunningProcesses << processes;
}

void MacProcessInfoGatherer::checkActiveWindow()
{
  QString processName = MacProcessInfoGathererHelper::getActiveProcess();
  QString windowName = MacProcessInfoGathererHelper::getActiveWindow();

  if (m_currentActiveWindowTitle != windowName)
  {
    m_currentActiveWindowTitle = windowName;
    emit activeWindowTitleChanged(windowName);
  }
  if (m_currentActiveWindowProgram != processName)
  {
    m_currentActiveWindowProgram = processName;
    emit activeWindowProcessChanged(processName);
  }
}

