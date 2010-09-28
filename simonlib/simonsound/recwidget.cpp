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

#include "recwidget.h"

#include "wavrecorderclient.h"
#include "soundconfig.h"
#include "wavplayerclient.h"
#include "simonsound.h"
#include "wavfilewidget.h"

#include <QGroupBox>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QChar>
#include <QFont>
#include <QPlainTextEdit>
#include <KInputDialog>

#include <KLocalizedString>
#include <KIcon>
#include <KMessageBox>
#include <KPushButton>
#include <KLocale>

#include "ui_recwidget.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param name
 * The name that is displayed in the title of the groupbox
 * @param fileTemplate
 * The fileTemplate to record to;
 * We will ressamble the file (existing or not) when we create the play/pause/delete handles
 * @param forceSimpleMode
 * If true, the recWidget will treat the fileTemplate as fileName and only record with a single device
 * no matter how many are configured
 * @param forcedDevices
 * Forces the recwidget to use these devices instead of simons global sound configuration
 * @param *parent
 * The parent of the object
 */
RecWidget::RecWidget(QString name, QString text, QString fileTemplate, bool forceSimpleMode, QWidget *parent, QList<SimonSound::DeviceConfiguration>* forcedDevices) : QWidget(parent),
statusTimer(new QTimer(this)),
ui(new Ui::RecWidgetUi()),
m_simpleMode(forceSimpleMode)
{
  this->fileTemplate = fileTemplate;

  ui->setupUi(this);
  ui->pbRecord->setIcon(KIcon("media-record"));
  ui->pbDeleteAll->setIcon(KIcon("edit-delete"));

  setTitle(name);

  if (m_simpleMode) ui->pbDeleteAll->hide();

  if (text.isEmpty())
    ui->tePrompt->hide();

  ui->tePrompt->setFont(SoundConfiguration::promptFont());
  ui->tePrompt->setPlainText(text);

  setupSignalsSlots();
  initialize(forcedDevices);
  
  if (!forcedDevices)
    connect(SoundServer::getInstance(), SIGNAL(devicesChanged()), this, SLOT(initialize()));

  hideActionPrompt();
  ui->lbPromptAction->setStyleSheet("color:white");
  connect(statusTimer, SIGNAL(timeout()), this, SLOT(showStartPrompt()));
}


void RecWidget::hideActionPrompt()
{
  ui->frmPromptAction->hide();
}


SimonSamples::SampleProblems RecWidget::sampleProblems()
{
  SimonSamples::SampleProblems problems = SimonSamples::None;
  foreach (WavFileWidget *wav, waves)
    problems |= wav->sampleProblems();

  return problems;
}


void RecWidget::showStartPrompt()
{
  ui->frmPromptAction->show();
  ui->frmPromptAction->setStyleSheet("background-color: green");
  ui->lbPromptAction->setText(i18n("Please speak..."));
  statusTimer->stop();
}


void RecWidget::showFinishPrompt()
{
  ui->frmPromptAction->show();
  ui->frmPromptAction->setStyleSheet("background-color: blue");
  ui->lbPromptAction->setText(i18nc("Waiting for the recording to finish...", "Idle..."));
}


void RecWidget::showWaitPrompt()
{
  ui->frmPromptAction->show();
  ui->frmPromptAction->setStyleSheet("background-color: red");
  ui->lbPromptAction->setText(i18n("Please wait..."));
}


void RecWidget::registerDevice(const QString& id, int channels, int sampleRate, const QString& filenameSuffix)
{
  kDebug() << "Wavfile: " << fileTemplate+filenameSuffix+".wav";
  WavFileWidget *wg = new WavFileWidget(id, channels, sampleRate, fileTemplate+filenameSuffix+".wav", this);

  QBoxLayout *lay = dynamic_cast<QVBoxLayout*>(ui->gbContainer->layout());

  Q_ASSERT(lay);

  lay->addWidget(wg);

  connect(wg, SIGNAL(sampleDeleted()), this, SLOT(slotSampleDeleted()));
  connect(wg, SIGNAL(playing()), this, SLOT(slotEnableDeleteAll()));
  connect(wg, SIGNAL(playbackFinished()), this, SLOT(slotEnableDeleteAll()));

  waves << wg;
}


bool RecWidget::isRecording()
{
  foreach (WavFileWidget *wav, waves)
    if (wav->getIsRecording())
    return true;
  return false;
}


QStringList RecWidget::getFileNames()
{
  QStringList fileNames;
  foreach (WavFileWidget *wav, waves)
    if (wav->hasRecordingReady())
    fileNames << wav->getFileName();

  return fileNames;
}


QStringList RecWidget::getDevices()
{
  QStringList devices;
  foreach (WavFileWidget *wav, waves)
    if (wav->hasRecordingReady())
    devices << wav->getDevice();

  return devices;
}

void RecWidget::initialize()
{
  initialize(0);
}

void RecWidget::initialize(QList<SimonSound::DeviceConfiguration>* forcedDevices)
{
  foreach (WavFileWidget *wg, waves)
    wg->deleteLater();
  waves.clear();

  QList<SimonSound::DeviceConfiguration> devices;
  if (!forcedDevices)
    devices = SoundServer::getTrainingInputDevices();
  else
    devices = *forcedDevices;
	
  if (m_simpleMode) {
    //which device?
    QStringList deviceNames;
    foreach (const SimonSound::DeviceConfiguration& dev, devices)
      deviceNames << i18nc("Sound device selection; First parameter is device name",
      "%1 (%2 channels, %3 Hz)", dev.name(), dev.channels(), dev.sampleRate());

    QString selected;
    if (deviceNames.count() == 1)
      selected = deviceNames[0];
    else
      selected = KInputDialog::getItem(i18n("Select input device"), i18n("Your sound configuration lists multiple input devices.\n\nThis function only allows you to use one of those devices.\n\nPlease select the sound device before you proceed."), deviceNames, 0, false);
    kDebug() << selected;
    if (!selected.isEmpty()) {
      SimonSound::DeviceConfiguration selectedDevice = devices.at(deviceNames.indexOf(selected));
      registerDevice(selectedDevice.name(), selectedDevice.channels(), selectedDevice.sampleRate(), "");
    }
  }
  else {
    for (int i=0; i < devices.count(); i++)
      registerDevice(devices.at(i).name(), devices.at(i).channels(), devices.at(i).sampleRate(), '.'+QString::number(i));
  }
  adjustButtonsToFile();
}


void RecWidget::displayError(const QString& error)
{
  KMessageBox::error(this, error);
}


void RecWidget::changePromptFont(const QFont& font)
{
  QString text = ui->tePrompt->toPlainText();
  ui->tePrompt->setFont(font);
  ui->tePrompt->setPlainText(text);
}


/**
 * \brief Returns true if there is a file at the assigned filename
 * \author Peter Grasch
 * @return File exists?
 */
bool RecWidget::hasRecordingReady()
{
  bool recordingReady = false;
  foreach (WavFileWidget *wav, waves)
    recordingReady |= wav->hasRecordingReady();

  return recordingReady;
}


/**
 * \brief Sets up the signal/slot connections
 * \author Peter Grasch
 */
void RecWidget::setupSignalsSlots()
{
  connect(ui->pbRecord, SIGNAL(clicked()), this, SLOT(record()));
  connect(ui->pbDeleteAll, SIGNAL(clicked()), this, SLOT(deleteAll()));
}


/**
 * \brief Sets the widgets title to the given title
 * \author Peter Grasch
 * @param newTitle The new title
 */
void RecWidget::setTitle(QString newTitle)
{
  ui->gbContainer->setTitle(newTitle);
}


/**
 * \brief Starts the recording
 * \author Peter Grasch
 */
void RecWidget::record()
{
  foreach (WavFileWidget *wav, waves)
    wav->record();

  bool someoneIsRecording = false;
  foreach (WavFileWidget *wav, waves)
    someoneIsRecording |= wav->getIsRecording();

  ui->pbRecord->setChecked(someoneIsRecording);
  disconnect(ui->pbRecord, SIGNAL(clicked()), this, SLOT(record()));
  connect(ui->pbRecord, SIGNAL(clicked()), this, SLOT(stopRecording()));

  emit recording();

  if (someoneIsRecording) {
    showWaitPrompt();
    statusTimer->start(600);
  }
}


/**
 * \brief Stops the recording
 * \author Peter Grasch
 */
void RecWidget::stopRecording()
{
  statusTimer->stop();
  foreach (WavFileWidget *wav, waves)
    wav->stopRecording();

  ui->pbRecord->setChecked(false);
  adjustButtonsToFile();

  disconnect(ui->pbRecord, SIGNAL(clicked()), this, SLOT(stopRecording()));
  connect(ui->pbRecord, SIGNAL(clicked()), this, SLOT(record()));
  emit recordingFinished();
  showWaitPrompt();
  hideActionPrompt();
}


void RecWidget::stopPlayback()
{
  foreach (WavFileWidget *wav, waves)
    wav->stopPlayback();
}


void RecWidget::adjustButtonsToFile()
{
  bool somethingHasSample = hasRecordingReady();

  ui->pbRecord->setEnabled(!somethingHasSample);
  ui->pbDeleteAll->setEnabled(somethingHasSample);
}


void RecWidget::slotSampleDeleted()
{
  adjustButtonsToFile();

  emit sampleDeleted();
}


void RecWidget::slotEnableDeleteAll()
{
  bool shouldEnableDelete = true;
  foreach (WavFileWidget *wav, waves) {
    shouldEnableDelete &= ! wav->getIsPlaying();
    kDebug() << wav->getIsPlaying();
  }
  kDebug() << "Updating enable button: " << shouldEnableDelete;
  ui->pbDeleteAll->setEnabled(shouldEnableDelete);
}


/**
 * \brief Deletes the file at fileTemplate (member)
 * \author Peter Grasch
 */
bool RecWidget::deleteAll()
{
  bool success = true;
  foreach (WavFileWidget *wav, waves)
    success = wav->deleteSample() && success;

  return success;
}

void RecWidget::checkFile()
{
  adjustButtonsToFile();
  foreach (WavFileWidget *wav, waves)
    wav->checkFile();
}

RecWidget::~RecWidget()
{
  delete ui;
}
