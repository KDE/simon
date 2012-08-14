/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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
#include <KLocalizedString>

RecognitionControl::RecognitionControl(const QString& user_name, QObject* parent) : QThread(parent),
  m_refCounter(0),
  username(user_name),
  m_startRequests(0),
  m_initialized(false),
  stopping(false),
  shouldBeRunning(false),
  recog(0)
{

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

QByteArray RecognitionControl::getBuildLog()
{
  return "<html><head /><body><p>"+recog->getLog().replace('\n', "<br />")+"</p></body></html>";
}

bool RecognitionControl::startRecognition()
{
  kDebug() << "Starting recognition" << ++m_startRequests;
  if (isInitialized() && (m_startRequests > 1))  {
    emit recognitionStarted();
    return true;
  }
  kDebug() << "Starting recognition: Continuing";
  return startRecognitionInternal();
}

void RecognitionControl::recognize(const QString& fileName)
{
  if (!shouldBeRunning) return;

  kDebug() << "Recognizing " << fileName;

  queueLock.lock();
  toRecognize.enqueue(fileName);
  queueLock.unlock();
}

void RecognitionControl::run()
{
  Q_ASSERT(recog);
  shouldBeRunning=true;

  RecognitionConfiguration *cfg = setupConfig();
  kDebug()<<"whee";
  bool success = recog->init(cfg);
  delete cfg;
  if (!success) {
    emitError(i18n("Failed to setup recognition: %1", recog->getLastError()));
    return;
  }

  m_initialized=true;

  while (shouldBeRunning) {
    QString file;
    queueLock.lock();
    if (!toRecognize.isEmpty())
      file = toRecognize.dequeue();
    queueLock.unlock();
    if (file.isNull()) {
      QThread::msleep(100);
    } else {
      emit recognitionResult(file, recog->recognize(file));
      emit recognitionDone(file);
    }
  }
}

bool RecognitionControl::startRecognitionInternal()
{
  start();

  emit recognitionStarted();
  return true;
}

bool RecognitionControl::stopInternal()
{
  shouldBeRunning=false;

  if (!isRunning()) return true;

  if (!wait(1000)) {
    while (isRunning()) {
      kDebug() << "Forcefully terminating";
      terminate();
      wait(500);
    }
  }
  m_lastModel = QString();

  return true;
}

bool RecognitionControl::suspend()
{
  bool res = stopInternal();
  if (!res) return false;
  return true;
}

void RecognitionControl::uninitialize()
{
  kDebug() << "Uninitializing recognition control";
  if (!m_initialized) return;

  recog->uninitialize();
  stopInternal();

  m_initialized=false;
}

bool RecognitionControl::stop()
{
  kDebug() << "Stopping recognition" << m_startRequests;
  if (--m_startRequests > 0)
    return true;

  if (m_startRequests < 0)
    m_startRequests = 0;

  kDebug() << "Stopping recognition: Continuing";
  return stopInternal();
}

RecognitionControl::~RecognitionControl()
{
  uninitialize();
}
