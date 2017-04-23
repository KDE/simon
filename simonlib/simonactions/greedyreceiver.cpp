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

#include "greedyreceiver.h"
#include "actionmanager.h"
#include <simonscenarios/commandmanager.h>
#include <KDebug>

bool GreedyReceiver::greedyTriggerRawList(const RecognitionResultList &resultList)
{
  kDebug() << "Greedy triggering raw list";
  if (resultList.isEmpty()) return false;

  if (resultList.count() > 1) {
    kDebug() << "Presenting user with results...";
    ActionManager::getInstance()->presentUserWithResults(resultList);
    return true;
  } else {
    return greedyTriggerRaw(resultList.at(0));
  }
}


bool GreedyReceiver::greedyTriggerRaw(const RecognitionResult& result)
{
  return greedyTrigger(result.sentence());
}


bool GreedyReceiver::greedyTrigger(const QString& triggerResult)
{
  kDebug() << "Greedy triggering!";
  if (!m_manager) return false;

  return m_manager->trigger(triggerResult, false /* silent */);
}


void GreedyReceiver::startGreedy()
{
  ActionManager::getInstance()->registerGreedyReceiver(this);
  if (m_manager)
    m_manager->setGreedyStatus(true);
}


void GreedyReceiver::stopGreedy()
{
  ActionManager::getInstance()->deRegisterGreedyReceiver(this);
  if (m_manager)
    m_manager->setGreedyStatus(false);
}

GreedyReceiver::~GreedyReceiver()
{
  ActionManager::getInstance()->deRegisterGreedyReceiver(this);
}
