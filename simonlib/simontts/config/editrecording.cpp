/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "editrecording.h"
#include <simonsound/recwidget.h>
#include <QWidget>
#include <QFormLayout>
#include <QFile>
#include <KStandardDirs>

EditRecording::EditRecording(QWidget *parent) : KDialog(parent), recorder(0)
{
  QWidget *widget = new QWidget( this );
  ui.setupUi(widget);

  setMainWidget( widget );
  setCaption( i18n("Recording") );

  QFormLayout *layout = static_cast<QFormLayout*>(widget->layout());
  recorder = new RecWidget("", "", KStandardDirs::locateLocal("tmp", "simontts/editrecording"),
        true, this);
  layout->addRow(i18n("Recording:"), recorder);

  connect(ui.teText, SIGNAL(textChanged()), this, SLOT(checkComplete()));
  connect(recorder, SIGNAL(sampleDeleted()), this, SLOT(checkComplete()));
  connect(recorder, SIGNAL(recordingFinished()), this, SLOT(checkComplete()));
  checkComplete();
}

EditRecording::~EditRecording()
{
  recorder->deleteAll();
  recorder->deleteLater();
}

bool EditRecording::addRecording(QString& text, QString& path)
{
  ui.teText->setReadOnly(false);
  ui.pbLastTextToSpeechRequests->setEnabled(true);
  recorder->deleteAll();
  if (exec())
  {
    text = ui.teText->toPlainText();
    QStringList files = recorder->getFileNames();
    if (files.count() != 1) return false;
    path = files[0];
    return true;
  }
  return false;
}

bool EditRecording::editRecording(const QString& text, QString& path)
{
  ui.teText->setReadOnly(true);
  ui.pbLastTextToSpeechRequests->setEnabled(false);
  QString tmpPath = KStandardDirs::locateLocal("tmp", "simontts/editrecording.wav");
  if ((!recorder->deleteAll()) || (!QFile::copy(path, tmpPath)))
    return false;
  recorder->checkFile();

  ui.teText->setPlainText(text);
  if (exec())
  {
    QStringList files = recorder->getFileNames();
    if (files.count() != 1) return false;
    path = files[0];
    return true;
  }
  return false;
}

void EditRecording::checkComplete()
{
  enableButton(KDialog::Ok, (!ui.teText->toPlainText().isEmpty() && recorder->hasRecordingReady()));
}

