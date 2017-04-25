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
#include "trainsamplepage.h"
#include <simonsound/recwidget.h>
#include <simonscenarios/trainingmanager.h>

#include <QDateTime>
#include <QDir>
#include <QLabel>
#include <QVBoxLayout>
#include <QFile>
#include <QUuid>
#include <KMessageBox>

#include <KLocalizedString>

TrainSamplePage::TrainSamplePage(QString prompt_, int nowPage, int maxPage, const QString name, QWidget* parent) : QWizardPage(parent),
prompt(prompt_),
fileName(QFile::encodeName(QUuid::createUuid().toString())
    + '_'
    + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss"))
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  QString title = i18nc("%1 is current page number, %2 is total page count", "Page %1 of %2", nowPage, maxPage);
  setTitle(name+": "+title);

  QVBoxLayout *lay = new QVBoxLayout(this);
  QLabel *desc = new QLabel(i18n("Please record the Text below."
    "\n\nTip: Leave about one to two seconds \"silence\" before and after you read "
    "the text for best results.\n"), this);
  desc->setWordWrap(true);
  recorder = new RecWidget("", prompt,
    TrainingManager::getInstance()->getTrainingDir()+

  //not needed because getTrainingDir() ensures that
  //the path already ends with a separator
  //				  +QDir::separator()+
    fileName, false, this);
  lay->addWidget(desc);
  lay->addWidget(recorder);

  connect(recorder, SIGNAL(recording()), this, SIGNAL(completeChanged()));
  connect(recorder, SIGNAL(recordingFinished()), this, SIGNAL(completeChanged()));
  connect(recorder, SIGNAL(sampleDeleted()), this, SIGNAL(completeChanged()));
  connect(recorder, SIGNAL(speakingStopped()), this, SLOT(speakingStopped()));
}

void TrainSamplePage::initializePage()
{
  if (field("powerRecording").toBool()) {
    if (!recorder->hasRecordingReady())
      recorder->record();
  }
}


bool TrainSamplePage::validatePage()
{
  recorder->stopRecording();
  recorder->stopPlayback();

  bool cont = true;
  if (recorder->sampleProblems() != SimonSamples::None) {
    if (KMessageBox::warningContinueCancel(this, i18n("Simon detected that at least one of the just recorded Sample might have some issues.\n\nSelect \"More information\" to find out how to fix this issue.\n\nDo you want to continue with your training without fixing this problem?"), QString(), KStandardGuiItem::cont(), KStandardGuiItem::cancel(), "ShowSampleWarning") != KMessageBox::Continue)
      cont = false;
  }

  return cont;
}

PromptsTable TrainSamplePage::getPrompts() const
{
  PromptsTable t;
  QStringList fileNames = getFileNames();
  QStringList sampleGroups = recorder->getSampleGroups();
  Q_ASSERT(fileNames.count() == sampleGroups.count());

  for (int i=0; i < fileNames.count(); i++)
    t.insert(fileNames[i], sampleGroups[i], getPrompt().toUpper());
  return t;
}

QStringList TrainSamplePage::getFileNames() const
{
  QStringList fileNames = recorder->getFileNames();
  for (int i=0; i < fileNames.count(); i++) {
    QString fileName = QDir::toNativeSeparators(fileNames[i]);
    fileName = fileName.mid(fileName.lastIndexOf(QDir::separator())+1);
    fileName = fileName.left(fileName.lastIndexOf("."));
    fileNames.replace(i, fileName);
  }

  return  fileNames;
}

bool TrainSamplePage::submit()
{
  bool succ = true;
  succ = TrainingManager::getInstance()->mergePrompts(getPrompts());

  if (!succ) {
    KMessageBox::error(this, i18n("Could not add samples to the corpus.\n\nThis indicates internal data corruption."));
    cleanUp();
  }

  return succ;
}


void TrainSamplePage::cleanupPage()
{
  recorder->stopRecording();
  recorder->stopPlayback();
}


bool TrainSamplePage::cleanUp()
{
  bool succ=true;
  if (recorder->hasRecordingReady()) {
    succ = recorder->deleteAll();
    if (!succ)
      KMessageBox::error(this, i18nc("%1 is list of filenames", "Could not remove samples \"%1\".", getFileNames().join("\", \"")));
  }

  return succ;
}


bool TrainSamplePage::isComplete() const
{
  Q_ASSERT(recorder);

  if (field("powerRecording").toBool())
    return (recorder->hasRecordingReady() || recorder->isRecording());
  else
    return recorder->hasRecordingReady();
}

void TrainSamplePage::speakingStopped()
{
  qDebug() << "Speaking stopped";
  if (field("powerRecording").toBool())
    wizard()->next();
}

TrainSamplePage::~TrainSamplePage()
{
  recorder->deleteLater();
}
