//
// C++ Implementation: soundsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "soundsettings.h"
#include <KMessageBox>
#include <KIcon>
#include "soundcontrol.h"
#include "../Settings/settings.h"

/**
 * \brief Constructor - inits the help text and the gui
 * \author Peter Grasch
 * @param parent the parent of the widget
 */
SoundSettings::SoundSettings(QWidget* parent):QWidget(parent)// SystemWidget(i18n("Soundeinstellungen"), KIcon("preferences-desktop-sound"), i18n("Konfigurieren Sie hier ihre Audiogeräte und legen Einstellungen für dessen Verwendung fest"), parent)
{
	ui.setupUi(this);
// 	guessChildTriggers(this);
	//set help
// 	help = i18n("Hier finden Sie alle Einstellungen zur Sound Ein- und Ausgabe.");

// 	connect ( ui.kcfg_SoundInputDevice, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
// 	connect ( ui.kcfg_SoundOutputDevice, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
// 	connect ( ui.cbChannels, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
// 	connect ( ui.sbSamplerate, SIGNAL(valueChanged(int)), this, SIGNAL(changed()));

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
	int defindevice=Settings::getI ( "Sound/InputDevice" );
	ui.kcfg_SoundInputDevice->setCurrentIndex(ui.kcfg_SoundInputDevice->findData(defindevice));



	SoundDeviceList *out = sc->getOutputDevices();

	ui.kcfg_SoundOutputDevice->clear();
	for ( int i=0; i<out->count(); i++ )
	{
		int deviceid= out->at (i).getDeviceID();
		ui.kcfg_SoundOutputDevice->addItem (out->at(i).getName(),deviceid );
	}
	int defoutdevice=Settings::getI ( "Sound/OutputDevice" );
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

	
	if ((channels != Settings::getI("Model/Channels")) || 
			(samplerate != Settings::getI("Model/Samplerate")))
	{
		KMessageBox::information(this, i18n("Die konfigurierten Kanäle / Samplerate der Trainingstexte passt nicht mit den Soundeinstellungen zusammen.\n\nBitte korrigieren Sie ihre Konfiguration hier oder definieren Sie entsprechende Nachbearbeitungsketten um den Unterschied auszugleichen.") );
		ok=false;
	}

	return ok;
}


/**
 * \brief As there _must_ be an audio device selected  it might as well be the correct one - return true
 * \author Peter Grasch
 * @return always true
 */
// bool SoundSettings::isComplete()
// {
// 	return true;
// }

/**
 * \brief Applys the changes
 * \author Peter Grasch
 * @return success
 */
// bool SoundSettings::apply()
// {
// 	Settings::set("Sound/InputDevice", ui.kcfg_SoundInputDevice->itemData(ui.kcfg_SoundInputDevice->currentIndex()));
// 	Settings::set("Sound/OutputDevice", ui.kcfg_SoundOutputDevice->itemData(
// 		      ui.kcfg_SoundOutputDevice->currentIndex()));
// 	Settings::set("Sound/Channels", ui.cbChannels->itemData(ui.cbChannels->currentIndex()));
// 	Settings::set("Sound/Samplerate", ui.sbSamplerate->value());
// 
// 	return check();
// }

/**
 * \brief Resets the cxhanges
 * \author Peter Grasch
 * @return success
 */
// bool SoundSettings::reset()
// {
// 	return init();
// }

/**
 * \brief Destructor
 * \author Peter Grasch
 */
SoundSettings::~SoundSettings()
{
	if (sc) delete sc;
}


