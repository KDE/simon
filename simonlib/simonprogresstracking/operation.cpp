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
#include "operation.h"
#include "statusmanager.h"
#include <QTimer>

#include <KLocalizedString>
#include <QMutex>

Operation::Operation(QThread* thread, const QString& name, const QString& currentAction, int now, int max, bool isAtomic) : QObject(0),
m_thread(thread),
m_deletionLocker(new QMutex(QMutex::Recursive)),
m_deletionTimer(new QTimer(this)),
m_name(name),
m_currentAction(currentAction),
m_now(now),
m_max(max),
m_isAtomic(isAtomic),
m_status(Operation::Running)
{
  connect(m_deletionTimer, SIGNAL(timeout()), this, SLOT(deleteLater()));
  registerWith(StatusManager::global());
}


void Operation::registerWith(StatusManager *man)
{
  QMutexLocker l(m_deletionLocker);
  manager << man;
  man->registerOperation(this);
}


QString Operation::currentAction()
{
  QMutexLocker l(m_deletionLocker);
  switch (m_status) {
    case Finished:
      return i18nc("The operation has completed", "Finished");
    case Aborted:
      return i18nc("The operation has been canceled", "Aborted");
    case Aborting:
      return i18nc("Aborting the operation", "Aborting...");
    default:
      return m_currentAction;
  }
}


void Operation::update(const QString& currentAction, int newProgress, int newMaximum)
{
  QMutexLocker l(m_deletionLocker);
  m_currentAction = currentAction;
  m_now = newProgress;

  if (newMaximum != -1)
    m_max = newMaximum;

  pushUpdate();
}


void Operation::update(int newProgress, int newMaximum)
{
  QMutexLocker l(m_deletionLocker);
  m_now = newProgress;
  if (newMaximum != -1)
    m_max = newMaximum;
  pushUpdate();
}


void Operation::cancel()
{
  QMutexLocker l(m_deletionLocker);
  m_cancel=true;
  m_status = Aborting;
  emit aborting();
  pushUpdate();
}


void Operation::maxProgressBar()
{
  QMutexLocker l(m_deletionLocker);
  if (maxProgress() > 0)
    update(maxProgress(), maxProgress());
  else update(1,1);
}


void Operation::canceled()
{
  QMutexLocker l(m_deletionLocker);
  m_status = Aborted;
  maxProgressBar();
  pushUpdate();
  m_deletionTimer->start(3000);
}


void Operation::finished()
{
  QMutexLocker l(m_deletionLocker);
  m_status = Finished;

  maxProgressBar();

  pushUpdate();
  m_deletionTimer->start(3000);
}


void Operation::pushUpdate()
{
  QMutexLocker l(m_deletionLocker);
  foreach (StatusManager *man, manager)
    man->update();
}


Operation::~Operation()
{
  QMutexLocker l(m_deletionLocker);
  foreach (StatusManager *man, manager)
    man->removeOperation(this);
}
