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

#include "recognitioncontrol.h"
#include <KDateTime>
#include <KDebug>

RecognitionControl::RecognitionControl(const QString& user_name, QObject* parent) : QThread(parent),
m_refCounter(0),
username(user_name),
m_startRequests(0)
{
  connect(this, SIGNAL(recognitionError(QString,QByteArray)), this, SLOT(touchLastFailedStart()));
}

bool RecognitionControl::isEmpty() const
{
  return (m_refCounter == 0);
}

void RecognitionControl::push()
{
  ++m_refCounter;
}

void RecognitionControl::pop()
{
  --m_refCounter;
}

bool RecognitionControl::recognitionRunning()
{
  kDebug() << m_startRequests;
  return (m_startRequests > 0);
}

void RecognitionControl::touchLastSuccessfulStart()
{
  m_lastSuccessfulStart = KDateTime::currentUtcDateTime().dateTime();
}

void RecognitionControl::touchLastFailedStart()
{
  m_lastFailedStart = KDateTime::currentUtcDateTime().dateTime();
}

bool RecognitionControl::shouldTryToStart(const QDateTime& activeModelDate)
{
  QDateTime utcActiveModelDate(activeModelDate);
  utcActiveModelDate.setTimeSpec(Qt::UTC);
  kWarning() << "Last successful start: " << m_lastSuccessfulStart;
  kWarning() << "Last failed start: " << m_lastFailedStart << m_lastFailedStart.isNull();
  kWarning() << "Active model: " << utcActiveModelDate << (utcActiveModelDate > m_lastSuccessfulStart);
  bool start = ((m_lastFailedStart.isNull() || (utcActiveModelDate > m_lastFailedStart)) && 
                (m_lastSuccessfulStart.isNull() || (utcActiveModelDate > m_lastSuccessfulStart)));
  kWarning() << "Start: " << start;
  return start;
}


RecognitionControl::~RecognitionControl()
{

}
