/*
 *   Copyright (C) 2011 Alessandro Buggin <alessandrobuggin@gmail.com>
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMONSAMPLESHARE_H
#define SIMONSAMPLESHARE_H

#include <KDialog>
#include <KDebug>
#include <QPointer>
#include "simonsampleshareui_export.h"

template<class T >
class QFutureWatcher;
class ProgressWidget;

class SendSampleWorker;
namespace Ui {
  class SampleShareDlg;
}

class SSCDAccess;
class Operation;
class KProgressDialog;
class Sample;

class SIMONSAMPLESHARE_EXPORT SampleShare : public KDialog{
  Q_OBJECT
  
private:
  Ui::SampleShareDlg *ui;
  SSCDAccess *server;
  SendSampleWorker *worker;
  QPointer<Operation> transmissionOperation;
  ProgressWidget *progressWidget;
  KProgressDialog *connectionProgressDialog;
  QFutureWatcher<bool> *futureWatcher;
  
  void connectToServer();
  void startTransmission();
  void cleanup();
  
private slots:
  void transmissionError(const QString&);
  void transmissionWarning(const QString&);
  void transmissionStatus(const QString&);
  void connected();
  void disconnected();
  void listDialects();
  void checkCompletion();
  void transmissionFinished();
  void sendSample(Sample *s);
  void displayStatus(QString message, int now, int max);
  void displayError(QString error);
  
public:
  SampleShare( QWidget *parent = 0 );
  ~SampleShare();

private slots:
    void slotButtonClicked(int button);
};
#endif