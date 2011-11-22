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
#include "deviceinformationpage.h"
#include "deviceinformationwidget.h"
#include "sscconfig.h"
#include "sscdaccess.h"

#include <simonsound/soundserver.h>
#include <sscobjects/microphone.h>
#include <sscobjects/soundcard.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QSettings>

#include <KLocalizedString>
#include <KMessageBox>

DeviceInformationPage::DeviceInformationPage(QWidget *parent) : QWizardPage(parent),
scrollWidget(new QScrollArea(this))
{
  setTitle(i18n("Device information"));

  QVBoxLayout *lay = new QVBoxLayout(this);

  QLabel *lbIntro = new QLabel(this);
  lbIntro->setWordWrap(true);
  lbIntro->setText(i18n("Please provide some info about your recording devices."));

  QWidget *scrollChildWidget = new QWidget(scrollWidget);
  QVBoxLayout *childLay = new QVBoxLayout(scrollChildWidget);
  scrollChildWidget->setLayout(childLay);

  scrollWidget->setWidget(scrollChildWidget);
  scrollWidget->setWidgetResizable(true);

  lay->addWidget(lbIntro);
  lay->addWidget(scrollWidget);

  setLayout(lay);
}


bool DeviceInformationPage::serializeToStorage(QSettings& ini) const
{
  ini.beginGroup("Recording");
  ini.beginWriteArray("Devices");

  int i=0;
  foreach (DeviceInformationWidget *wg, informationWidgets) {
    ini.setArrayIndex(i);

    wg->serialize(ini);

    i++;
  }

  ini.endArray();
  ini.endGroup();
  return true;
}


void DeviceInformationPage::registerInformationWidget(DeviceInformationWidget* wg)
{
  connect(wg, SIGNAL(completeChanged()), this, SIGNAL(completeChanged()));
  informationWidgets << wg;
  QVBoxLayout *layout = dynamic_cast<QVBoxLayout*>(scrollWidget->widget()->layout());
  if (!layout) return;
  layout->addWidget(wg);
  wg->show();
}


bool DeviceInformationPage::deserializeFromStorage(QSettings& ini)
{
  ini.beginGroup("Recording");

  int size = ini.beginReadArray("Devices");
  qDeleteAll(informationWidgets);
  informationWidgets.clear();
  for (int i=0; i < size; i++) {
    ini.setArrayIndex(i);

    DeviceInformationWidget *wg = new DeviceInformationWidget(this);
    wg->deserialize(ini);

    registerInformationWidget(wg);
  }
  ini.endArray();
  ini.endGroup();
  return true;
}


QHash<QString, Microphone*> DeviceInformationPage::buildMicrophoneMappings(bool &ok)
{
  ok = true;
  QHash<QString, Microphone*> microphones;
  foreach (DeviceInformationWidget *wg, informationWidgets) {
    fprintf (stderr, "Processing widget during processing mics...\n");
    kDebug() << "processing widget...";
    QString device = wg->getDeviceName();
    QString micModel = wg->getMicModel();
    QString micType = wg->getMicType();

    Microphone *mic = new Microphone(0, micModel, micType);
    bool succ = true;
    kDebug() << "Calling server to get or create mic";
    qint32 id = SSCDAccess::getInstance()->getOrCreateMicrophone(mic, &succ);

    if (!succ) {
      fprintf(stderr, "failed to get / create microphone\n");
      kDebug() << "Failed...";
      ok = false;
    }

    kDebug() << "Got id for mic: " << id;

    microphones.insert(device, mic);
  }

  return microphones;
}


QHash<QString, SoundCard*> DeviceInformationPage::buildSoundCardMappings(bool &ok)
{
  QHash<QString, SoundCard*> soundCards;
  foreach (DeviceInformationWidget *wg, informationWidgets) {
    fprintf (stderr, "Processing widget during processing soundcards...\n");
    QString device = wg->getDeviceName();
    QString model = wg->getModel();
    QString type = wg->getType();

    SoundCard *soundCard = new SoundCard(0, model, type);
    bool succ = true;
    qint32 id = SSCDAccess::getInstance()->getOrCreateSoundCard(soundCard, &succ);

    if (!succ) {
      fprintf(stderr, "failed to get / create soundcard\n");
      ok = false;
    }

    kDebug() << "Got id for sound card: " << id;

    soundCards.insert(device, soundCard);
  }
  fprintf(stderr, "Done processing sound cards\n");

  return soundCards;
}


void DeviceInformationPage::initializePage()
{
  if (informationWidgets.count()) return;

  QList<SimonSound::DeviceConfiguration> devices = SoundServer::getTrainingInputDevices();
  foreach (const SimonSound::DeviceConfiguration& device, devices) {
    DeviceInformationWidget *wg = new DeviceInformationWidget(this);
    wg->setup(device);

    registerInformationWidget(wg);
  }
}

QList<SimonSound::DeviceConfiguration> DeviceInformationPage::buildDeviceList()
{
  QList<SimonSound::DeviceConfiguration> devices;
  foreach (DeviceInformationWidget *wg, informationWidgets)
    devices.append(wg->buildDevice());
  return devices;
}


bool DeviceInformationPage::isComplete() const
{
  bool complete = true;
  foreach (DeviceInformationWidget *wg, informationWidgets)
    complete &= wg->isComplete();
  return complete;
}


bool DeviceInformationPage::validatePage()
{
  foreach (DeviceInformationWidget *wg, informationWidgets)
    wg->storeConfig();

  SSCConfig::self()->writeConfig();
  return true;
}


DeviceInformationPage::~DeviceInformationPage()
{
}
