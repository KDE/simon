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


#include "soundsettings.h"
#include "soundconfig.h"

#include <KMessageBox>
#include <QVBoxLayout>
#include <KIcon>

#include <KLocalizedString>
#include <KAboutData>
#include <KPageWidgetItem>
#include <KPageWidget>
#include <KDebug>
#include <kgenericfactory.h>

K_PLUGIN_FACTORY( SoundSettingsFactory, 
			registerPlugin< SoundSettings >(); 
		)
        
K_EXPORT_PLUGIN( SoundSettingsFactory("simonlib") )

/**
 * \brief Constructor - inits the help text and the gui
 * \author Peter Grasch
 * @param parent the parent of the widget
 */
SoundSettings::SoundSettings(QWidget* parent, const QVariantList& args):
		KCModule(KGlobal::mainComponent(), parent),
	enabled(true)
{
	Q_UNUSED(args);

	QVBoxLayout *lay = new QVBoxLayout(this);
	KPageWidget *pageWidget = new KPageWidget(this);
	lay->addWidget(pageWidget);

	if (args.count() == 1)
		pageWidget->setFaceType(KPageView::Tabbed);
	
	QWidget *coreConfig = new QWidget(this);
	deviceUi.setupUi(coreConfig);
	deviceUi.pbTest->setIcon(KIcon("help-hint"));
	deviceUi.pbReload->setIcon(KIcon("view-refresh"));
	connect(deviceUi.pbTest, SIGNAL(clicked()), this, SLOT(checkWithSuccessMessage()));
	connect(deviceUi.pbReload, SIGNAL(clicked()), this, SLOT(load()));
	connect(deviceUi.cbSoundInputDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
	connect(deviceUi.cbSoundOutputDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));

	QWidget *vadConfig = new QWidget(this);
	vadUi.setupUi(vadConfig);
	
	QWidget *postProcessingConfig = new QWidget(this);
	postProcUi.setupUi(postProcessingConfig);

	QWidget *promptConfig = new QWidget(this);
	promptUi.setupUi(promptConfig);

	KPageWidgetItem *deviceConfItem = pageWidget->addPage(coreConfig, i18n("Device Configuration"));
	KPageWidgetItem *vadConfItem = pageWidget->addPage(vadConfig, i18n("Voice Activity Detection"));
	KPageWidgetItem *promptConfItem = pageWidget->addPage(promptConfig, i18n("Prompt Font"));
	KPageWidgetItem *postProcConfItem = pageWidget->addPage(postProcessingConfig, i18n("Post-Processing"));

	deviceConfItem->setIcon(KIcon("audio-card"));
	vadConfItem->setIcon(KIcon("media-playback-start"));
	promptConfItem->setIcon(KIcon("draw-text"));
	postProcConfItem->setIcon(KIcon("applications-other"));

	deviceConfItem->setHeader("");
	vadConfItem->setHeader("");
	promptConfItem->setHeader("");
	postProcConfItem->setHeader("");
	
	KAboutData *about = new KAboutData(
				"soundsettings", "", ki18n("Recordings"),
				"0.1", ki18n("Configuration for the Recording and Playback of sounds"), KAboutData::License_GPL);
#if KDE_IS_VERSION(4,0,80)
	about->setProgramIconName("preferences-desktop-sound");
#endif
	setAboutData( about );

	addConfig(SoundConfiguration::self(), this);
//#ifdef USE_WITH_SIMON
//	connect(AdinStreamer::getInstance(), SIGNAL(audioDeviceError()), this, SLOT(show()));
//#endif
}



void SoundSettings::checkWithSuccessMessage()
{
	if (check())
		KMessageBox::information(this, i18n("The soundconfiguration has been tested successfully."));
}

/**
 * \author Peter Grasch
 * \return success
 */
void SoundSettings::load()
{
//#ifdef Q_OS_LINUX
//#ifdef USE_WITH_SIMON
//	AdinStreamer::getInstance()->stopSoundStream();
//#endif
//#endif
	deviceUi.cbSoundInputDevice->clear();
	deviceUi.cbSoundOutputDevice->clear();

	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
	{
		kDebug() << "Input device name: " << deviceInfo.deviceName();
		deviceUi.cbSoundInputDevice->addItem(deviceInfo.deviceName());
	}
	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
	{
		kDebug() << "Output device name: " << deviceInfo.deviceName();
		deviceUi.cbSoundOutputDevice->addItem(deviceInfo.deviceName());
	}

	//select
	QString configuredInputDevice = SoundConfiguration::soundInputDevice();
	QString configuredOutputDevice = SoundConfiguration::soundOutputDevice();

	deviceUi.cbSoundInputDevice->setCurrentIndex(deviceUi.cbSoundInputDevice->findText(configuredInputDevice));
	deviceUi.cbSoundOutputDevice->setCurrentIndex(deviceUi.cbSoundOutputDevice->findText(configuredOutputDevice));

	bool hasChanged=false;
	if ((!configuredOutputDevice.isEmpty()) && (!configuredInputDevice.isEmpty()) &&
		((deviceUi.cbSoundOutputDevice->currentText() != configuredOutputDevice) ||
		 (deviceUi.cbSoundInputDevice->currentText() != configuredInputDevice)))
	{
		if (KMessageBox::questionYesNoCancel(this, i18n("simon noticed that not all of the sound devices you selected to use previously are available.\n\nThis is perfectly normal if you are connected to simond or are otherwise using an application that blocks the device.\n\nDid you plug / unplug a device or otherwise change your systems audio setup?\n\nSelecting \"Yes\" will allow you to change your sound configuration, essentially deleting your previous configuration. Selecting \"No\" will temporarily deactivate the sound configuration in order to protect your previous configuration from being overwritten.")) == KMessageBox::Yes)
		{
			deviceUi.cbSoundInputDevice->setCurrentIndex(
					deviceUi.cbSoundInputDevice->findText(
						QAudioDeviceInfo::defaultInputDevice().deviceName()));
			deviceUi.cbSoundOutputDevice->setCurrentIndex(
					deviceUi.cbSoundOutputDevice->findText(
						QAudioDeviceInfo::defaultOutputDevice().deviceName()));
			hasChanged=true;
			KMessageBox::information(this, i18n("Please adjust your soundconfiguration accordingly."));
			enable();
		} else disable();

	} else enable();

	KCModule::load();

	if (hasChanged) emit changed(true);
	
//#ifdef USE_WITH_SIMON
//	AdinStreamer::getInstance()->restartSoundStream();
//#endif
}

void SoundSettings::enable()
{
	deviceUi.lbInDevice->setEnabled(true);
	deviceUi.cbSoundInputDevice->setEnabled(true);

	deviceUi.lbOutDevice->setEnabled(true);
	deviceUi.cbSoundOutputDevice->setEnabled(true);

	deviceUi.kcfg_SoundChannels->setEnabled(true);
	deviceUi.kcfg_SoundSampleRate->setEnabled(true);
	deviceUi.lbHz->setEnabled(true);
	deviceUi.lbChannels->setEnabled(true);
	deviceUi.lbSamplerate->setEnabled(true);

	deviceUi.pbTest->setEnabled(true);
	enabled=true;
}

void SoundSettings::disable()
{
	deviceUi.lbInDevice->setEnabled(false);
	deviceUi.cbSoundInputDevice->setEnabled(false);

	deviceUi.lbOutDevice->setEnabled(false);
	deviceUi.cbSoundOutputDevice->setEnabled(false);

	deviceUi.kcfg_SoundChannels->setEnabled(false);
	deviceUi.kcfg_SoundSampleRate->setEnabled(false);
	deviceUi.lbHz->setEnabled(false);
	deviceUi.lbChannels->setEnabled(false);
	deviceUi.lbSamplerate->setEnabled(false);

	deviceUi.pbTest->setEnabled(false);
	enabled=false;
}


bool SoundSettings::check()
{
	QString inputDevice = getSelectedInputDeviceId();
	QString outputDevice = getSelectedOutputDeviceId();
	int channels = deviceUi.kcfg_SoundChannels->value();
	int samplerate = deviceUi.kcfg_SoundSampleRate->value();

	bool ok = true;
	QAudioFormat format;
	format.setFrequency(samplerate);
	format.setChannels(channels);
	format.setSampleSize(16); // 16 bit
	format.setSampleType(QAudioFormat::SignedInt); // SignedInt currently
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setCodec("audio/pcm");

	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
	{
		if ((deviceInfo.deviceName() == inputDevice) ||
			(deviceInfo.deviceName() == outputDevice))
		{
			if (!deviceInfo.isFormatSupported(format))
			{
				ok = false;
				break;
			}
		}
	}
	/*
	 *     qWarning()<<"default format not supported try to use nearest";
	 *            format = info.nearestFormat(format
	 * */

	if (!ok)
		KMessageBox::error(this, i18n("The selected sound configuration is not supported by your hardware.\n\nPlease double-check your configuration and, if necessairy, please contact your vendor."));

	return ok;
}

QString SoundSettings::getSelectedInputDeviceId()
{
	return deviceUi.cbSoundInputDevice->currentText();
}

QString SoundSettings::getSelectedOutputDeviceId()
{
	return deviceUi.cbSoundOutputDevice->currentText();
}

void SoundSettings::save()
{
	//even when not enabled this call will be save
	//The sound input / output devices are not affected by this
	//and this way we store the configuration regarding the prompt font
	//and the postprocessing commands
	KCModule::save();

	if (!enabled) return;

//#ifdef USE_WITH_SIMON
//	if (AdinStreamer::getInstance()->isRunning())
//		check();
//#else
	check();
//#endif

	SoundConfiguration::setSoundInputDevice(getSelectedInputDeviceId());
	SoundConfiguration::setSoundOutputDevice(getSelectedOutputDeviceId());

	KSharedConfig::Ptr config = KSharedConfig::openConfig("simonsoundrc");
	KConfigGroup group(config, "Devices");
	group.writeEntry("SoundInputDevice", getSelectedInputDeviceId());
	group.writeEntry("SoundOutputDevice", getSelectedOutputDeviceId());
	config->sync();

//#ifdef USE_WITH_SIMON
//	AdinStreamer::getInstance()->stopSoundStream();
//	AdinStreamer::getInstance()->restartSoundStream();
//#endif
}

void SoundSettings::slotChanged()
{
	emit changed(true);
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
SoundSettings::~SoundSettings()
{
}


