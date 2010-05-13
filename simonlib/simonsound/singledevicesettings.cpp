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

#include "singledevicesettings.h"

#include <KMessageBox>
#include <QAudioDeviceInfo>
#include <KIcon>

#include <KLocalizedString>
#include <KDebug>

#include "ui_singledeviceconfiguration.h"



SingleDeviceSettings::SingleDeviceSettings(SoundDeviceType type, QString deviceName, int channels, 
			int sampleRate, SoundDeviceOptions options, QWidget* parent):
		QWidget(parent),
	enabled(true),
	m_type(type),
	m_deviceName(deviceName),
	m_options(options)
{
	ui = new Ui::SingleDeviceConfiguration();

	ui->setupUi(this);

	ui->sbChannels->setValue(channels);
	ui->sbSampleRate->setValue(sampleRate);

	ui->pbTest->setIcon(KIcon("help-hint"));
	
	if (!(options & Removable))
		ui->pbRemove->hide();
	
	connect(ui->pbTest, SIGNAL(clicked()), this, SLOT(checkWithSuccessMessage()));
	connect(ui->cbSoundDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
}



void SingleDeviceSettings::checkWithSuccessMessage()
{
	if (!enabled) return;
	if (check())
		KMessageBox::information(this, i18n("The soundconfiguration has been tested successfully."));
}

/**
 * \author Peter Grasch
 * \return success
 */
void SingleDeviceSettings::load(QString deviceName, int channels, 
			int sampleRate)
{
	ui->cbSoundDevice->clear();

	foreach(const QAudioDeviceInfo &deviceInfo, (m_type == Input) ? QAudioDeviceInfo::availableDevices(QAudio::AudioInput) :
			QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
	{
		kDebug() << "Device name: " << deviceInfo.deviceName();
		ui->cbSoundDevice->addItem(deviceInfo.deviceName());
	}

	//select
	ui->cbSoundDevice->setCurrentIndex(ui->cbSoundDevice->findText(deviceName));

	ui->sbChannels->setValue(channels);
	ui->sbSampleRate->setValue(sampleRate);

	bool hasChanged=false;
	if ((!deviceName.isEmpty()) &&
		(ui->cbSoundDevice->currentText() != deviceName))
	{
		if (KMessageBox::questionYesNoCancel(this, i18n("simon noticed that the sound device \"%1\" is no longer available.\n\nThis is perfectly normal if you are connected to simond or are otherwise using an application that blocks the device.\n\nDid you plug / unplug a device or otherwise change your systems audio setup?\n\nSelecting \"Yes\" will allow you to change your sound configuration, essentially deleting your previous configuration. Selecting \"No\" will temporarily deactivate the sound configuration in order to protect your previous configuration from being overwritten.", deviceName)) == KMessageBox::Yes)
		{
			ui->cbSoundDevice->setCurrentIndex(
					ui->cbSoundDevice->findText(
						(m_type == Input) ? QAudioDeviceInfo::defaultInputDevice().deviceName() :
						QAudioDeviceInfo::defaultOutputDevice().deviceName()));
			hasChanged=true;
			KMessageBox::information(this, i18n("Please adjust your soundconfiguration accordingly."));
			enable();
		} else disable();

	} else enable();

	if (hasChanged) emit changed(true);
}

void SingleDeviceSettings::enable()
{
	ui->frmDevice->setEnabled(true);
	enabled=true;
}

void SingleDeviceSettings::disable()
{
	ui->frmDevice->setEnabled(false);
	enabled=false;
}


bool SingleDeviceSettings::check()
{
	if (!enabled) return true;

	QString device = getSelectedDeviceId();
	int channels = getChannels();
	int samplerate = getSampleRate();

	bool ok = true;
	QAudioFormat format;
	format.setFrequency(samplerate);
	format.setChannels(channels);
	format.setSampleSize(16); // 16 bit
	format.setSampleType(QAudioFormat::SignedInt); // SignedInt currently
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setCodec("audio/pcm");

	foreach(const QAudioDeviceInfo &deviceInfo, (m_type == Input) ? QAudioDeviceInfo::availableDevices(QAudio::AudioInput) :
			QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
	{
		if (deviceInfo.deviceName() == device)
		{
			if (!deviceInfo.isFormatSupported(format))
			{
				ok = false;
				break;
			}
		}
	}

	if (!ok)
		KMessageBox::error(this, i18n("The selected sound configuration is not supported by the following device:\n%1\n\nPlease double-check your configuration and, if necessairy, please contact your vendor.", device));

	return ok;
}

QString SingleDeviceSettings::getSelectedDeviceId()
{
	return ui->cbSoundDevice->currentText();
}

int SingleDeviceSettings::getSampleRate()
{
	return ui->sbSampleRate->value();
}

int SingleDeviceSettings::getChannels()
{
	return ui->sbChannels->value();
}


void SingleDeviceSettings::slotChanged()
{
	emit changed(true);
}

