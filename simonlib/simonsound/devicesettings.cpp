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

#include "devicesettings.h"
#include "soundconfig.h"
#include "soundserver.h"
#include "singledevicesettings.h"

#include <QVBoxLayout>
#include <KIcon>
#include <QTimer>

#include <KLocalizedString>
#include <KDebug>

#include "simonsound.h"
#include <simoncontextdetection/compoundcondition.h>
#include "ui_deviceconfiguration.h"

/**
 * \brief Constructor - inits  the gui
 * \author Peter Grasch
 * @param parent the parent of the widget
 */
DeviceSettings::DeviceSettings(QWidget* parent):
QWidget(parent),
enabled(true)
{
  ui = new Ui::DeviceConfiguration();

  ui->setupUi(this);
  ui->pbAddInput->setIcon(KIcon("list-add"));
  ui->pbAddOutput->setIcon(KIcon("list-add"));

  ui->pbReload1->setIcon(KIcon("view-refresh"));
  ui->pbReload2->setIcon(KIcon("view-refresh"));
  connect(ui->pbReload1, SIGNAL(clicked()), this, SLOT(refreshDevices()));
  connect(ui->pbReload2, SIGNAL(clicked()), this, SLOT(refreshDevices()));

  connect(ui->pbAddOutput, SIGNAL(clicked()), this, SLOT(addOutputDevice()));
  connect(ui->pbAddInput, SIGNAL(clicked()), this, SLOT(addInputDevice()));
}


void DeviceSettings::checkWithSuccessMessage()
{
  foreach (SingleDeviceSettings* dev, inputDevices)
    dev->checkWithSuccessMessage();
  foreach (SingleDeviceSettings* dev, outputDevices)
    dev->checkWithSuccessMessage();
}


bool DeviceSettings::check()
{
  bool check = true;
  foreach (SingleDeviceSettings* dev, inputDevices)
    check = dev->check() && check;
  foreach (SingleDeviceSettings* dev, outputDevices)
    check = dev->check() && check;
  return check;
}


void DeviceSettings::refreshDevices()
{
  foreach (SingleDeviceSettings* dev, inputDevices)
    dev->refreshDevices();
  foreach (SingleDeviceSettings* dev, outputDevices)
    dev->refreshDevices();
}


void DeviceSettings::addInputDevice()
{
  registerInputDevice(new SingleDeviceSettings(SimonSound::Input,
    SoundServer::defaultInputDevice(), 1, 16000, false, 16000,
    SimonSound::Training,
    (SimonSound::SoundDeviceUses) (SimonSound::Training|SimonSound::Recognition),
    SoundServer::defaultSampleGroup(),
    QString(),
    SimonSound::Removable,
    this));
  emit changed(true);
}


void DeviceSettings::addOutputDevice()
{
  registerOutputDevice(new SingleDeviceSettings(SimonSound::Output,
    SoundServer::defaultOutputDevice(), 1, 16000, false, 16000,
    SimonSound::Training,
    SimonSound::Training,
    QString(),
    QString(),
    SimonSound::Removable, this));
  emit changed(true);
}


void DeviceSettings::registerDevice(SingleDeviceSettings* dev, QScrollArea* scrollArea)
{
  QBoxLayout *lay = dynamic_cast<QVBoxLayout*>(scrollArea->widget()->layout());

  Q_ASSERT(lay);

  lay->insertWidget(lay->count()-1 /* leave spacer the last item */, dev);

  connect(dev, SIGNAL(changed(bool)), this, SLOT(slotChanged()));
  connect(dev, SIGNAL(requestRemove(SingleDeviceSettings*)), this, SLOT(removeDevice(SingleDeviceSettings*)));
}


void DeviceSettings::registerInputDevice(SingleDeviceSettings* dev)
{
  registerDevice(dev, ui->saInputDevices);
  inputDevices << dev;
}


void DeviceSettings::registerOutputDevice(SingleDeviceSettings* dev)
{
  registerDevice(dev, ui->saOutputDevices);
  dev->disableSampleGroup();
  outputDevices << dev;
}


SingleDeviceSettings* DeviceSettings::getDevice(int index, QScrollArea* scrollArea)
{
  SingleDeviceSettings *dev = dynamic_cast<SingleDeviceSettings*>(scrollArea->widget()->layout()->itemAt(index)->widget());
  Q_ASSERT(dev);
  return dev;
}


SingleDeviceSettings* DeviceSettings::getInputDevice(int index)
{
  return getDevice(index, ui->saInputDevices);
}


SingleDeviceSettings* DeviceSettings::getOutputDevice(int index)
{
  return getDevice(index, ui->saOutputDevices);
}


void DeviceSettings::removeDevice(SingleDeviceSettings* device)
{
  if (device->getType() == SimonSound::Input) {
    inputDevices.removeAll(device);
  }
  else {
    outputDevices.removeAll(device);
  }
  device->deleteLater();

  emit changed(true);
}


/**
 * \author Peter Grasch
 * \return success
 */
void DeviceSettings::load()
{
  qDeleteAll(inputDevices);
  qDeleteAll(outputDevices);
  inputDevices.clear();
  outputDevices.clear();

  QStringList soundInputDevices = SoundConfiguration::soundInputDevices();
  QList<int> soundInputChannels = SoundConfiguration::soundInputChannels();
  QList<int> soundInputSampleRates = SoundConfiguration::soundInputSampleRates();
  QList<int> soundInputUses = SoundConfiguration::soundInputUses();
  QList<int> soundInputResampleEnabled = SoundConfiguration::soundInputResampleEnabled();
  QList<int> soundInputResampleSampleRates = SoundConfiguration::soundInputResampleSampleRates();
  QStringList soundInputDefaultSampleGroups = SoundConfiguration::soundInputDefaultSampleGroups();
  QStringList soundInputConditions = SoundConfiguration::soundInputConditions();
  
  QStringList soundOutputDevices = SoundConfiguration::soundOutputDevices();
  QList<int> soundOutputChannels = SoundConfiguration::soundOutputChannels();
  QList<int> soundOutputSampleRates = SoundConfiguration::soundOutputSampleRates();
  QList<int> soundOutputUses = SoundConfiguration::soundOutputUses();
  QList<int> soundOutputResampleEnabled = SoundConfiguration::soundOutputResampleEnabled();
  QStringList soundOutputConditions = SoundConfiguration::soundOutputConditions();
  
  //making sure the splitting in separate lists didn't screw everything up...
  Q_ASSERT(soundInputDevices.count() == soundInputSampleRates.count());
  Q_ASSERT(soundInputSampleRates.count() == soundInputChannels.count());
  Q_ASSERT(soundInputSampleRates.count() == soundInputUses.count());
  Q_ASSERT(soundInputResampleEnabled.count() == soundInputUses.count());
  Q_ASSERT(soundInputResampleSampleRates.count() == soundInputUses.count());
  Q_ASSERT(soundInputResampleSampleRates.count() == soundInputConditions.count());
  Q_ASSERT(soundOutputDevices.count() == soundOutputSampleRates.count());
  Q_ASSERT(soundOutputSampleRates.count() == soundOutputChannels.count());
  Q_ASSERT(soundOutputSampleRates.count() == soundOutputUses.count());
  Q_ASSERT(soundOutputResampleEnabled.count() == soundOutputUses.count());
  Q_ASSERT(soundOutputResampleEnabled.count() == soundOutputConditions.count());

  if (soundInputDefaultSampleGroups.count() != soundInputDevices.count())
  {
      kDebug() << "sample group config is new or incorrect.  resetting entries.";
      soundInputDefaultSampleGroups.clear();

      for (int i=0; i<soundInputDevices.count(); i++)
      {
          soundInputDefaultSampleGroups.push_back(SoundServer::defaultSampleGroup());
      }
  }

  bool hasChanged = false;
  for (int i=0; i < soundInputDevices.count(); i++) {
    SingleDeviceSettings *s = new SingleDeviceSettings(SimonSound::Input, soundInputDevices[i],
      soundInputChannels[i], soundInputSampleRates[i], (soundInputResampleEnabled[i] == 1), 
      soundInputResampleSampleRates[i],
      (SimonSound::SoundDeviceUses) soundInputUses[i],
      (SimonSound::SoundDeviceUses) (SimonSound::Training|SimonSound::Recognition),
      soundInputDefaultSampleGroups[i],
      soundInputConditions[i],
      (i > 0) ? SimonSound::Removable : SimonSound::NoOptions,
      this);
    registerInputDevice(s);
    if (s->getHasChanged())
      hasChanged = true;
  }

  for (int i=0; i < soundOutputDevices.count(); i++) {
    SingleDeviceSettings *s = new SingleDeviceSettings(SimonSound::Output, soundOutputDevices[i],
      soundOutputChannels[i], soundOutputSampleRates[i], (soundOutputResampleEnabled[i] == 1), 
      0,
      (SimonSound::SoundDeviceUses) soundOutputUses[i],
      SimonSound::Training,
      "",
      soundOutputConditions[i],
      (i > 0) ? SimonSound::Removable : SimonSound::NoOptions,
      this);
    registerOutputDevice(s);
    if (s->getHasChanged())
      hasChanged = true;
  }
  if (hasChanged) {
    QTimer::singleShot(100, this, SLOT(slotChanged()));
  }
}


void DeviceSettings::save()
{
  check();

  QStringList soundInputDevices;
  QList<int> soundInputChannels;
  QList<int> soundInputSampleRates;
  QList<int> soundInputUses;
  QList<int> soundInputResampleEnabled;
  QList<int> soundInputResampleSampleRates;
  QStringList soundInputDefaultSampleGroups;
  QStringList soundInputConditions;

  QStringList soundOutputDevices;
  QList<int> soundOutputChannels;
  QList<int> soundOutputSampleRates;
  QList<int> soundOutputUses;
  QList<int> soundOutputResampleEnabled;
  QStringList soundOutputConditions;

  for (int i=0; i < inputDevices.count(); i++) {
    SingleDeviceSettings *dev = getInputDevice(i);

    Q_ASSERT(dev);

    soundInputDevices << dev->getSelectedDeviceId();
    soundInputChannels << dev->getChannels();
    soundInputSampleRates << dev->getSampleRate();
    soundInputResampleEnabled << ((dev->getResampleEnabled()) ? 1 : 0);
    soundInputResampleSampleRates << dev->getResampleSampleRate();
    soundInputUses << (int) dev->getUses();
    soundInputDefaultSampleGroups << dev->getDefaultSampleGroup();
    soundInputConditions << dev->getConditions();

    dev->stored();
  }

  for (int i=0; i < outputDevices.count(); i++) {
    SingleDeviceSettings *dev = getOutputDevice(i);

    Q_ASSERT(dev);

    soundOutputDevices << dev->getSelectedDeviceId();
    soundOutputChannels << dev->getChannels();
    soundOutputSampleRates << dev->getSampleRate();
    soundOutputResampleEnabled << ((dev->getResampleEnabled()) ? 1: 0);
    soundOutputUses << (int) dev->getUses();
    soundOutputConditions << dev->getConditions();

    dev->stored();
  }

  SoundConfiguration::setSoundInputDevices(soundInputDevices);
  SoundConfiguration::setSoundInputChannels(soundInputChannels);
  SoundConfiguration::setSoundInputSampleRates(soundInputSampleRates);
  SoundConfiguration::setSoundInputResampleEnabled(soundInputResampleEnabled);
  SoundConfiguration::setSoundInputResampleSampleRates(soundInputResampleSampleRates);
  SoundConfiguration::setSoundInputUses(soundInputUses);
  SoundConfiguration::setSoundInputDefaultSampleGroups(soundInputDefaultSampleGroups);
  SoundConfiguration::setSoundInputConditions(soundInputConditions);

  SoundConfiguration::setSoundOutputDevices(soundOutputDevices);
  SoundConfiguration::setSoundOutputChannels(soundOutputChannels);
  SoundConfiguration::setSoundOutputSampleRates(soundOutputSampleRates);
  SoundConfiguration::setSoundOutputResampleEnabled(soundOutputResampleEnabled);
  SoundConfiguration::setSoundOutputUses(soundOutputUses);
  SoundConfiguration::setSoundOutputConditions(soundOutputConditions);

  SoundConfiguration::self()->writeConfig();

  SoundServer::getInstance()->reinitializeDevices();
}


void DeviceSettings::slotChanged()
{
  emit changed(true);
}


DeviceSettings::~DeviceSettings()
{
  delete ui;
}
