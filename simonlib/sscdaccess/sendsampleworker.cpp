/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#include "sendsampleworker.h"
#include "abstractsampledataprovider.h"

#include <sscdaccess/sscdaccess.h>
#include <sscobjects/sample.h>
#include <simonprogresstracking/operation.h>
#include <simonprogresstracking/progresswidget.h>

#include <QDir>
#include <QLabel>
#include <QtConcurrentRun>

#include <QThread>
#include <QVBoxLayout>
#include <QFuture>
#include <QFutureWatcher>

#include <KMessageBox>
#include <KLocalizedString>
#include <KPushButton>
#include <KDebug>


/**
 * Does the actual server communication. This will take LONG. This is why this
 * function should be called from outside the GUI thread. It will communicate
 * with the rest of the world through the shared transmitOperation object.
 */
bool SendSampleWorker::sendSamples()
{
  shouldAbort = false;
  shouldDelete = false;
  bool successful = true;

  int i=0;
  int retryAmount = 0;

  if (!m_dataProvider->startTransmission()) {
    kDebug() << "Could not start transmission";

    emit error(i18n("Failed to start the sample transmission.\n\nMost likely this is caused by problems to send the information about the used input devices."));
    shouldAbort = true;
    successful = false;
  }
  kDebug() << "Transmission started...";

  int maxProgress = m_dataProvider->sampleToTransmitCount();

  while (!shouldAbort && (m_dataProvider->hasSamplesToTransmit())) {
    Sample *s = m_dataProvider->getSample();
    emit status(i18nc("%1 is a path", "Sending: %1", s->path()), i, maxProgress);

    //    QFile f(s->path());
    emit sendSample(s);

    kDebug() << "Emit signal";
    if (!m_server->processSampleAnswer() && (retryAmount < 3)) {
      kDebug() << "Error processing sample"  << retryAmount;
      if (!m_server->isConnected()) {
        shouldAbort = true;
        successful = false;
      }
      retryAmount++;
      i--;
    }
    else {
      if (retryAmount < 3)
        m_dataProvider->sampleTransmitted();
      else {
        emit error(i18nc("%1 is an error message", "Server could not process sample: %1",
          m_server->lastError()));
        shouldAbort = true;                       //m_dataProvider->skipSample();
      }
      retryAmount = 0;
    }
    kDebug() << "Done processing sample";

    //if sample could not be opened we probably skipped this sample; ignore that silently
    //    } else {
    //      kDebug() << "File not found";
    //      m_dataProvider->sampleTransmitted();
    //    }

    i++;
  }
  kDebug() << "Done";
  m_dataProvider->stopTransmission();
  if (!shouldAbort) {
    if (m_isStored) {
      //remove storage
      if (QFile::exists(m_storageDirectory+"/profile.ini") &&
        !QFile::remove(m_storageDirectory+"/profile.ini"))
        emit error(i18nc("%1 is a path", "Profile information could not be removed: %1.", m_storageDirectory+"/profile.ini"));
      QDir d(m_storageDirectory);
      if (d.exists(m_storageDirectory) && !d.rmdir(m_storageDirectory))
        emit error(i18nc("%1 is a path", "Storage folder could not be removed: %1.", m_storageDirectory));
    }
    emit finished();
  } else
  emit aborted();

  if (shouldDelete)
    deleteLater();

  return successful;
}


bool SendSampleWorker::storeData()
{
  shouldAbort = false;

  //storing data
  bool succ = true;
  if (!m_dataProvider->store()) {
    kDebug() << "bleh";
    emit error("Could not store samples correctly.");
    emit aborted();
    succ = false;
  }
  else {
    emit finished();
    kDebug() << "Finished";
  }
  return succ;
}


SendSampleWorker::~SendSampleWorker()
{
  delete m_dataProvider;
}
