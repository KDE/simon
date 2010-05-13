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
#include "singledevicesettings.h"

#include <KMessageBox>
#include <QVBoxLayout>
#include <QAudioDeviceInfo>
#include <KIcon>

#include <KLocalizedString>
#include <KDebug>

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
	ui->pbReload1->setIcon(KIcon("view-refresh"));
	ui->pbReload2->setIcon(KIcon("view-refresh"));
	connect(ui->pbReload1, SIGNAL(clicked()), this, SLOT(load()));
	connect(ui->pbReload2, SIGNAL(clicked()), this, SLOT(load()));

	connect(ui->pbAddOutput, SIGNAL(clicked()), this, SLOT(addOutputDevice()));
	connect(ui->pbAddInput, SIGNAL(clicked()), this, SLOT(addInputDevice()));
}



void DeviceSettings::checkWithSuccessMessage()
{
	foreach (SingleDeviceSettings* dev, devices)
		dev->checkWithSuccessMessage();
}

bool DeviceSettings::check()
{
	bool check = true;
	foreach (SingleDeviceSettings* dev, devices)
		check = dev->check() && check;
	return check;
}

void DeviceSettings::addInputDevice()
{

}

void DeviceSettings::addOutputDevice()
{

}


/**
 * \author Peter Grasch
 * \return success
 */
void DeviceSettings::load()
{
	//select
/*	QString configuredInputDevice = SoundConfiguration::soundInputDevice();
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
	*/
}

void DeviceSettings::save()
{
	check();

	/*
	SoundConfiguration::setSoundInputDevice(getSelectedInputDeviceId());
	SoundConfiguration::setSoundOutputDevice(getSelectedOutputDeviceId());

	KSharedConfig::Ptr config = KSharedConfig::openConfig("simonsoundrc");
	KConfigGroup group(config, "Devices");
	group.writeEntry("SoundInputDevice", getSelectedInputDeviceId());
	group.writeEntry("SoundOutputDevice", getSelectedOutputDeviceId());
	config->sync();

	*/
	SoundServer::getInstance()->reinitializeDevices();
}

void DeviceSettings::slotChanged()
{
	emit changed(true);
}


