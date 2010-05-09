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


#include "devicesettings.h"
#include "soundconfig.h"
#include "soundserver.h"

#include <KMessageBox>
#include <QVBoxLayout>
#include <QAudioDeviceInfo>
#include <KIcon>

#include <KLocalizedString>
#include <KDebug>

#include "ui_deviceconfiguration.h"

/**
 * \brief Constructor - inits the help text and the gui
 * \author Peter Grasch
 * @param parent the parent of the widget
 */
DeviceSettings::DeviceSettings(QWidget* parent):
		QWidget(parent),
	enabled(true)
{
	ui = new Ui::DeviceConfiguration();

	ui->setupUi(this);
	ui->pbTest->setIcon(KIcon("help-hint"));
	ui->pbReload->setIcon(KIcon("view-refresh"));
	connect(ui->pbTest, SIGNAL(clicked()), this, SLOT(checkWithSuccessMessage()));
	connect(ui->pbReload, SIGNAL(clicked()), this, SLOT(load()));
	connect(ui->cbSoundInputDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
	connect(ui->cbSoundOutputDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
}



void DeviceSettings::checkWithSuccessMessage()
{
	if (check())
		KMessageBox::information(this, i18n("The soundconfiguration has been tested successfully."));
}

/**
 * \author Peter Grasch
 * \return success
 */
void DeviceSettings::load()
{
	ui->cbSoundInputDevice->clear();
	ui->cbSoundOutputDevice->clear();

	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
	{
		kDebug() << "Input device name: " << deviceInfo.deviceName();
		ui->cbSoundInputDevice->addItem(deviceInfo.deviceName());
	}
	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
	{
		kDebug() << "Output device name: " << deviceInfo.deviceName();
		ui->cbSoundOutputDevice->addItem(deviceInfo.deviceName());
	}

	//select
	QString configuredInputDevice = SoundConfiguration::soundInputDevice();
	QString configuredOutputDevice = SoundConfiguration::soundOutputDevice();

	ui->cbSoundInputDevice->setCurrentIndex(ui->cbSoundInputDevice->findText(configuredInputDevice));
	ui->cbSoundOutputDevice->setCurrentIndex(ui->cbSoundOutputDevice->findText(configuredOutputDevice));

	bool hasChanged=false;
	if ((!configuredOutputDevice.isEmpty()) && (!configuredInputDevice.isEmpty()) &&
		((ui->cbSoundOutputDevice->currentText() != configuredOutputDevice) ||
		 (ui->cbSoundInputDevice->currentText() != configuredInputDevice)))
	{
		if (KMessageBox::questionYesNoCancel(this, i18n("simon noticed that not all of the sound devices you selected to use previously are available.\n\nThis is perfectly normal if you are connected to simond or are otherwise using an application that blocks the device.\n\nDid you plug / unplug a device or otherwise change your systems audio setup?\n\nSelecting \"Yes\" will allow you to change your sound configuration, essentially deleting your previous configuration. Selecting \"No\" will temporarily deactivate the sound configuration in order to protect your previous configuration from being overwritten.")) == KMessageBox::Yes)
		{
			ui->cbSoundInputDevice->setCurrentIndex(
					ui->cbSoundInputDevice->findText(
						QAudioDeviceInfo::defaultInputDevice().deviceName()));
			ui->cbSoundOutputDevice->setCurrentIndex(
					ui->cbSoundOutputDevice->findText(
						QAudioDeviceInfo::defaultOutputDevice().deviceName()));
			hasChanged=true;
			KMessageBox::information(this, i18n("Please adjust your soundconfiguration accordingly."));
			enable();
		} else disable();

	} else enable();

	if (hasChanged) emit changed(true);
}

void DeviceSettings::enable()
{
	ui->lbInDevice->setEnabled(true);
	ui->cbSoundInputDevice->setEnabled(true);

	ui->lbOutDevice->setEnabled(true);
	ui->cbSoundOutputDevice->setEnabled(true);

	ui->kcfg_SoundChannels->setEnabled(true);
	ui->kcfg_SoundSampleRate->setEnabled(true);
	ui->lbHz->setEnabled(true);
	ui->lbChannels->setEnabled(true);
	ui->lbSamplerate->setEnabled(true);

	ui->pbTest->setEnabled(true);
	enabled=true;
}

void DeviceSettings::disable()
{
	ui->lbInDevice->setEnabled(false);
	ui->cbSoundInputDevice->setEnabled(false);

	ui->lbOutDevice->setEnabled(false);
	ui->cbSoundOutputDevice->setEnabled(false);

	ui->kcfg_SoundChannels->setEnabled(false);
	ui->kcfg_SoundSampleRate->setEnabled(false);
	ui->lbHz->setEnabled(false);
	ui->lbChannels->setEnabled(false);
	ui->lbSamplerate->setEnabled(false);

	ui->pbTest->setEnabled(false);
	enabled=false;
}


bool DeviceSettings::check()
{
	QString inputDevice = getSelectedInputDeviceId();
	QString outputDevice = getSelectedOutputDeviceId();
	int channels = ui->kcfg_SoundChannels->value();
	int samplerate = ui->kcfg_SoundSampleRate->value();

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

	if (!ok)
		KMessageBox::error(this, i18n("The selected sound configuration is not supported by your hardware.\n\nPlease double-check your configuration and, if necessairy, please contact your vendor."));

	return ok;
}

QString DeviceSettings::getSelectedInputDeviceId()
{
	return ui->cbSoundInputDevice->currentText();
}

QString DeviceSettings::getSelectedOutputDeviceId()
{
	return ui->cbSoundOutputDevice->currentText();
}

void DeviceSettings::save()
{
	//even when not enabled this call will be save
	//The sound input / output devices are not affected by this
	//and this way we store the configuration regarding the prompt font
	//and the postprocessing commands
	if (!enabled) return;
	check();

	SoundConfiguration::setSoundInputDevice(getSelectedInputDeviceId());
	SoundConfiguration::setSoundOutputDevice(getSelectedOutputDeviceId());

	KSharedConfig::Ptr config = KSharedConfig::openConfig("simonsoundrc");
	KConfigGroup group(config, "Devices");
	group.writeEntry("SoundInputDevice", getSelectedInputDeviceId());
	group.writeEntry("SoundOutputDevice", getSelectedOutputDeviceId());
	config->sync();

	SoundServer::getInstance()->reinitializeDevices();
}

void DeviceSettings::slotChanged()
{
	emit changed(true);
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
DeviceSettings::~DeviceSettings()
{
}


