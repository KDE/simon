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

#include "trainingswizard.h"
#include "trainsamplepage.h"
#include "deviceinformationpage.h"
#include "sscsampledataprovider.h"
#include "sendsamplespage.h"
#include "trainsampleintropage.h"
#include "sscconfig.h"

#include <simonsound/soundserver.h>
#include <simonsound/trainsamplevolumepage.h>

#include <QWizardPage>
#include <QStringList>

#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QCheckBox>
#include <QFile>
#include <QSettings>
#include <QDir>

#include <KLocalizedString>
#include <KMessageBox>
#include <KStandardDirs>
#include <KDebug>

#include <math.h>
#include <sscprotocol/sscprotocol.h>

TrainingsWizard::TrainingsWizard(QWidget *parent) : SimonWizard(parent)
{
  setBanner("training");
  addPage(createIntroPage());

  m_infoPage = createDeviceDescPage();
  addPage(m_infoPage);

  if (SoundServer::getCalibrateVolume())
    addPage(new TrainSampleVolumePage());

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}


DeviceInformationPage* TrainingsWizard::createDeviceDescPage()
{
  return new DeviceInformationPage(this);
}


bool TrainingsWizard::init(qint32 userId, const QString& path)
{
  QSettings ini(path+"/profile.ini", QSettings::IniFormat);
  Sample::SampleType type = (Sample::SampleType)
    ini.value("Type").toInt();
  QString name = ini.value("Name").toString();

  SSCSampleDataProvider *sampleDataProvider = new SSCSampleDataProvider(userId, type, name);
  m_infoPage->deserializeFromStorage(ini);
  sampleDataProvider->registerMicrophoneInfo(m_infoPage);

  ini.beginGroup("Samples");
  int size = ini.beginReadArray("Sample");
  kDebug() << "Size of read array: " << size;
  for (int i=0; i < size; i++) {
    ini.setArrayIndex(i);
    TrainSamplePage *page = new TrainSamplePage(name, ini.value("Prompt").toString(), i+1, size,
      path, this,
      ini.value("FileNameTemplate").toString(),
	  m_infoPage
      );
    page->setupUi();
    addPage(page);

    sampleDataProvider->registerDataProvider(page);
  }
  ini.endArray();
  ini.endGroup();

  addPage(new SendSamplePage(sampleDataProvider, true /* is stored */, path, this));
  addPage(createFinishedPage());

  return true;
}


bool TrainingsWizard::init(qint32 userId, Sample::SampleType type, const QStringList& prompts, const QString& name)
{
  setWindowTitle(name);

  if (prompts.isEmpty()) return false;

  SSCSampleDataProvider *sampleDataProvider = new SSCSampleDataProvider(userId, type, name);
  int nowPage=1;
  int maxPage=prompts.count();
  sampleDataProvider->registerMicrophoneInfo(m_infoPage);

  foreach (const QString& prompt, prompts) {
    TrainSamplePage *page = new TrainSamplePage(name, prompt, nowPage++, maxPage,
      SSCConfig::sampleDirectory(), this);
    page->setupUi();
    connect(this, SIGNAL(rejected()), page, SLOT(cleanUp()));
    addPage(page);

    sampleDataProvider->registerDataProvider(page);
  }

  addPage(new SendSamplePage(sampleDataProvider, false /* is stored */, SSCConfig::sampleDirectory(), this));
  addPage(createFinishedPage());
  return true;
}


QStringList TrainingsWizard::repeatPrompts()
{
  return parsePromptsFromFile(KStandardDirs::locate("appdata", "texts/repeat/repeat.prompts"));
}


QStringList TrainingsWizard::trainingPrompts()
{
  return parsePromptsFromFile(KStandardDirs::locate("appdata", "texts/training/training.prompts"));
}


QStringList TrainingsWizard::interviewQuestions()
{
  return parsePromptsFromFile(KStandardDirs::locate("appdata", "texts/interview/interview.prompts"));
}


QStringList TrainingsWizard::parsePromptsFromFile(const QString& path)
{
  QFile f(path);
  if (!f.open(QIODevice::ReadOnly)) return QStringList();

  QStringList prompts;

  while (!f.atEnd())
    prompts << QString::fromUtf8(f.readLine()).trimmed();

  return prompts;
}


int TrainingsWizard::collectSamples(Sample::SampleType type, qint32 userId)
{
  // 	if (!cleanUp())
  // 		return -1;
  QString name;
  QStringList prompts;
  switch (type) {
    case Sample::Repeating:
    {
      name = i18nc("Training type where the user is required to repeat what the recording facilitator tells him", "Repeating");
      prompts = repeatPrompts();
      break;
    }
    case Sample::Training:
    {
      name = i18nc("Training type where the user reads prompts off the screen", "Training");
      prompts = trainingPrompts();
      break;
    }
    case Sample::Interview:
    {
      name = i18nc("Training type where the facilitator asks general questions and the interviewee answers them", "Interview");
      prompts = interviewQuestions();
      break;
    }
  }
  if (!init(userId, type, prompts, name)) {
    KMessageBox::sorry(this, i18n("Invalid set of prompts"));
    return -1;
  }

  return exec();
}


QWizardPage* TrainingsWizard::createIntroPage()
{
  return new TrainSampleIntroPage(this);
}


QWizardPage* TrainingsWizard::createFinishedPage()
{
  QWizardPage *finished = new QWizardPage(this);
  finished->setTitle(i18n("Training completed"));
  QLabel *lbFinished = new QLabel(finished);
  lbFinished->setText(i18n("The samples have been collected. They will now be transferred to the server."));
  lbFinished->setWordWrap(true);
  QVBoxLayout *lay = new QVBoxLayout(finished);
  lay->addWidget(lbFinished);
  finished->setLayout(lay);
  return finished;
}


void TrainingsWizard::submit()
{
  int i=0;
  forever
  {
    QWizardPage *plainPage = page(i++);
    if (!plainPage) break;

    TrainSamplePage *page = dynamic_cast<TrainSamplePage*>(plainPage);
    if (!page)
      continue;

    page->submit();
  }
}


bool TrainingsWizard::cleanUp()
{
  //clean up the samples
  QDir d(SSCConfig::sampleDirectory());

  QStringList files = d.entryList(QDir::NoDotAndDotDot);
  bool all = true;
  QString sampleDirectory = SSCConfig::sampleDirectory();
  foreach (const QString& file, files) {
    if (!QFile::remove(sampleDirectory+QDir::separator()+file)) {
      all = false;
      kDebug() << sampleDirectory+QDir::separator()+file;
    }
  }
  if (!all)
    KMessageBox::sorry(this, i18nc("%1 is path", "Could not clean up sample folder %1.\n\nPlease remove all files manually", sampleDirectory));
  return all;
}


TrainingsWizard::~TrainingsWizard()
{

}
