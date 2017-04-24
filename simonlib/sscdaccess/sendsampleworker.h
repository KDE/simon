/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SENDSAMPLEWORKER_H_7C93ADEB2453450C88A2A4C90F5D82F1
#define SIMON_SENDSAMPLEWORKER_H_7C93ADEB2453450C88A2A4C90F5D82F1

#include "sscdaccess_export.h"
#include <sscobjects/sample.h>
#include <QString>
#include <QFutureWatcher>

class SSCDAccess;
class Operation;
class AbstractSampleDataProvider;
class QVBoxLayout;
class ProgressWidget;
class Sample;

class SSCDACCESS_EXPORT SendSampleWorker : public QObject
{
  Q_OBJECT

  signals:
    void status(QString, int now, int max);
    void error(QString);
    void aborted();
    void finished();
    void sendSample(Sample *s);

  private:
    SSCDAccess *m_server;
    bool shouldAbort;
    bool shouldDelete;
    AbstractSampleDataProvider *m_dataProvider;
    bool m_isStored;
    QString m_storageDirectory;

  private slots:
    void abort() { shouldAbort = true; }

  public:
    SendSampleWorker(SSCDAccess *server, AbstractSampleDataProvider *dataProvider, 
		     bool isStored=false, const QString& storageDirectory=QString()) :
      m_server(server),
      shouldAbort(false),
      shouldDelete(false),
      m_dataProvider(dataProvider),
      m_isStored(isStored),
      m_storageDirectory(storageDirectory)
      {}
    ~SendSampleWorker();

    bool sendSamples();
    bool storeData();
    bool getShouldAbort() { return shouldAbort; }
    void deleteThatSometime() { shouldDelete = true; }
};

#endif
