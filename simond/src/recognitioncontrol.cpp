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
username(user_name)
{
  connect(this, SIGNAL(recognitionError(const QString&, const QByteArray&)), this, SLOT(touchLastFailedStart()));
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
  kDebug() << "Last successful start: " << m_lastSuccessfulStart;
  kDebug() << "Last failed start: " << m_lastFailedStart;
  kDebug() << "Active model: " << activeModelDate;
  bool start = ((m_lastFailedStart.isNull() || (activeModelDate > m_lastFailedStart)) && 
                (m_lastSuccessfulStart.isNull() || (activeModelDate > m_lastSuccessfulStart)));
  kDebug() << "Start: " << start;
  return start;
}


RecognitionControl::~RecognitionControl()
{

}
