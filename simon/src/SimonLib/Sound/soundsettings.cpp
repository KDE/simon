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
#include <KMessageBox>
#include <KIcon>
#include "soundcontrol.h"
#include "coreconfiguration.h"


/**
 * \brief Constructor - inits the help text and the gui
 * \author Peter Grasch
 * @param parent the parent of the widget
 */
SoundSettings::SoundSettings(QWidget* parent):QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pbTest, SIGNAL(clicked()), this, SLOT(checkWithSuccessMessage()));
	connect(ui.pbReload, SIGNAL(clicked()), this, SLOT(init()));

	ui.pbReload->setIcon(KIcon("view-refresh"));
	ui.pbTest->setIcon(KIcon("help-hint"));

	this->sc= new SoundControl();
	init();
}


void SoundSettings::checkWithSuccessMessage()
{
	if (check())
		KMessageBox::information(this, i18n("Die Soundkonfiguration wurde erfolgreich getestet."));
}

/**
 * \author Peter Grasch
 * \brief Asks the SoundControl for the needed infos and inserts the data in the comboboxes
 * \return success
 */
bool SoundSettings::init()
{
	SoundDeviceList *in = sc->getInputDevices();

	ui.kcfg_SoundInputDevice->clear();
	for ( int i=0; i<in->count(); i++ )
	{
		int deviceid= in->at(i).getDeviceID();
		ui.kcfg_SoundInputDevice->addItem (in->at(i).getName(),deviceid );
	}
	int defindevice=CoreConfiguration::soundInputDevice();
	ui.kcfg_SoundInputDevice->setCurrentIndex(ui.kcfg_SoundInputDevice->findData(defindevice));



	SoundDeviceList *out = sc->getOutputDevices();

	ui.kcfg_SoundOutputDevice->clear();
	for ( int i=0; i<out->count(); i++ )
	{
		int deviceid= out->at (i).getDeviceID();
		ui.kcfg_SoundOutputDevice->addItem (out->at(i).getName(),deviceid );
	}
	int defoutdevice=CoreConfiguration::soundOutputDevice();
	ui.kcfg_SoundOutputDevice->setCurrentIndex(ui.kcfg_SoundOutputDevice->findData(defoutdevice));

	return true;
}

bool SoundSettings::check()
{
	int inputDevice = ui.kcfg_SoundInputDevice->itemData(ui.kcfg_SoundInputDevice->currentIndex()).toInt();
	int outputDevice = ui.kcfg_SoundOutputDevice->itemData(ui.kcfg_SoundOutputDevice->currentIndex()).toInt();
	int channels = ui.kcfg_SoundChannels->value();
	int samplerate = ui.kcfg_SoundSampleRate->value();

	bool ok = this->sc->checkDeviceSupport(inputDevice, outputDevice, channels, samplerate);

	if (!ok)
		KMessageBox::error(this, i18n("Die ausgewählte Soundkonfiguration wird von der Hardware nicht unterstützt.\n\nBitte korrigieren Sie die Werte.\n\nFalls notwendig, wenden Sie sich bitte an Ihren Soundkartenhersteller."));

	
	if ((channels != CoreConfiguration::soundChannels()) || 
			(samplerate != CoreConfiguration::soundSampleRate()))
	{
		KMessageBox::information(this, i18n("Die konfigurierten Kanäle / Samplerate der Trainingstexte passt nicht mit den Soundeinstellungen zusammen.\n\nBitte korrigieren Sie ihre Konfiguration hier oder definieren Sie entsprechende Nachbearbeitungsketten um den Unterschied auszugleichen.") );
		ok=false;
	}

	return ok;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
SoundSettings::~SoundSettings()
{
	if (sc) delete sc;
}


