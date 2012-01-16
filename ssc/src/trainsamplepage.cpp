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
#include "trainsamplepage.h"
#include "deviceinformationpage.h"
#include <simonsound/recwidget.h>

#include <QDateTime>
#include <QDir>
#include <QLabel>
#include <QSettings>
#include <QVBoxLayout>
#include <QVariant>

#include <KLocalizedString>
#include <KMessageBox>
#include <KDebug>

TrainSamplePage::TrainSamplePage(const QString& name, QString prompt_, int nowPage, int maxPage, const QString& directory,
QWidget* parent, const QString& forcedFileNameTemplate, DeviceInformationPage *forcedDevices) :
QWizardPage(parent),
m_name(name),
recorder(0),
m_forcedDevices(forcedDevices),
prompt(prompt_),
m_thisPage(nowPage),
m_maxPage(maxPage),
m_directory(directory)
{
  if (forcedFileNameTemplate.isEmpty()) {
    fileName = prompt_.replace(' ', '_').replace('/','_').remove('?').replace('\\','_')
      .remove('<').remove('>').remove('|').remove('"').left(100)
      + "_S"
      + QString::number(nowPage)
      + '_'
      + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
  }
  else {
    fileName = forcedFileNameTemplate;
  }
}


void TrainSamplePage::setupUi()
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  QString title = i18n("Page %1 of %2", m_thisPage, m_maxPage);
  setTitle(m_name+": "+title);

  QVBoxLayout *lay = new QVBoxLayout(this);
  QLabel *desc = new QLabel(i18n("Please record the Text below."
    "\n\nTip: Leave about one to two seconds \"silence\" before and after you read "
    "the text for best results!\n"), this);
  desc->setWordWrap(true);
  lay->addWidget(desc);

  if (recorder) recorder->deleteLater();

  QList<SimonSound::DeviceConfiguration> forcedDevs;
  if (m_forcedDevices) forcedDevs = m_forcedDevices->buildDeviceList();
  recorder = new RecWidget("", prompt,
    m_directory+fileName, false, this, forcedDevs);
  

  lay->addWidget(recorder);

  connect(recorder, SIGNAL(recording()), this, SIGNAL(completeChanged()));
  connect(recorder, SIGNAL(recordingFinished()), this, SIGNAL(completeChanged()));
  connect(recorder, SIGNAL(sampleDeleted()), this, SIGNAL(completeChanged()));
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
    if (KMessageBox::warningContinueCancel(this, i18n("simon detected that at least one of the just recorded sample might have some issues.\n\nSelect \"More information\" to find out how to fix this issue.\n\nDo you want to continue with your training without fixing this problem?"), QString(), KStandardGuiItem::cont(), KStandardGuiItem::cancel(), "ShowSampleWarning") != KMessageBox::Continue)
      cont = false;
  }

  return cont;
}


QStringList TrainSamplePage::getFileNames() const
{
  return recorder->getFileNames();
  /*
  QStringList fileNames = recorder->getFileNames();
  for (int i=0; i < fileNames.count(); i++)
  {
    QString fileName = fileNames[i];
    int last_sep = qMax(fileName.lastIndexOf("/"), fileName.lastIndexOf("\\"));

    fileName = fileName.mid(last_sep+1);
    fileName = fileName.left(fileName.lastIndexOf("."));
    fileNames.replace(i, fileName);
  }

  return  fileNames;
  */
}


QStringList TrainSamplePage::getDevices()
{
  return recorder->getDevices();
}


bool TrainSamplePage::submit()
{
  return true;
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
      KMessageBox::error(this, i18n("Could not remove samples \"%1\".", getFileNames().join("\", \"")));
  }

  return succ;
}


bool TrainSamplePage::isComplete() const
{
  Q_ASSERT(recorder);

  return true;
}


bool TrainSamplePage::serializeToStorage(QSettings& ini, const QString& directory) const
{
  bool succ = true;

  //copy files to target directory
  QStringList fileNames = getFileNames();
  foreach (const QString& fileName, fileNames) {
    QString onlyName = fileName.mid(qMax(fileName.lastIndexOf("/"), fileName.lastIndexOf(QDir::separator())));
    if (!QFile::copy(fileName, directory+QDir::separator()+onlyName)) {
      kDebug() << "could not copy " << fileName << " to " << directory+QDir::separator()+onlyName;
      fileNames.removeAll(fileName);

      succ = false;
    }
    else {
      if (!QFile::remove(fileName))
        kWarning() << "Could not remove original input file!";
    }
  }

  //store prompts
  ini.setValue("Prompt", prompt);
  ini.setValue("FilenameTemplate", fileName);
  return succ;
}


bool TrainSamplePage::deserializeFromStorage(QSettings& ini, const QString& directory)
{
  prompt = ini.value("Prompt").toString();
  fileName = ini.value("FilenameTemplate").toString();

  m_directory = directory;
  setupUi();
  return true;
}


TrainSamplePage::~TrainSamplePage()
{
  recorder->deleteLater();
}
