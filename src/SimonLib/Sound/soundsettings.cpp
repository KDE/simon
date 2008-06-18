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
#include "../SimonLib/Sound/soundcontrol.h"
#include "../SimonLib/Settings/settings.h"

/**
 * \brief Constructor - inits the help text and the gui
 * \author Peter Grasch
 * @param parent the parent of the widget
 */
SoundSettings::SoundSettings(QWidget* parent): SystemWidget(tr("Soundeinstellungen"), QIcon(":/images/icons/preferences-desktop-sound.svg"), tr("Konfigurieren Sie hier ihre Audiogeräte und legen Einstellungen für dessen Verwendung fest"), parent)
{
	ui.setupUi(this);
	guessChildTriggers(this);
	hide();
	//set help
	help = tr("Hier finden Sie alle Einstellungen zur Sound Ein- und Ausgabe.");

	//set up the channels
	ui.cbChannels->clear();
	ui.cbChannels->addItem(tr("Mono"), 1);
	ui.cbChannels->addItem(tr("Stereo"), 2);
	
	this->sc= new SoundControl();
	this->in = 0;
	this->out = 0;

	connect ( ui.cbInDevice, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
	connect ( ui.cbOutDevice, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
	connect ( ui.cbChannels, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
	connect ( ui.sbSamplerate, SIGNAL(valueChanged(int)), this, SIGNAL(changed()));

	connect(ui.pbTest, SIGNAL(clicked()), this, SLOT(check()));
	connect(ui.pbReload, SIGNAL(clicked()), this, SLOT(init()));
}

/**
 * \author Peter Grasch
 * \brief Asks the SoundControl for the needed infos and inserts the data in the comboboxes
 * \return success
 */
bool SoundSettings::init()
{
	if (in) delete in;
	if (out) delete out;


	this->in = sc->getInputDevices();

	ui.cbInDevice->clear();
	for ( int i=0; i<in->count(); i++ )
	{
		int deviceid= in->at(i).getDeviceID();
		ui.cbInDevice->addItem (in->at(i).getName(),deviceid );
	}
	int defindevice=Settings::getI ( "Sound/InputDevice" );
	ui.cbInDevice->setCurrentIndex(ui.cbInDevice->findData(defindevice));



	this->out = sc->getOutputDevices();

	ui.cbOutDevice->clear();
	for ( int i=0; i<out->count(); i++ )
	{
		int deviceid= out->at (i).getDeviceID();
		ui.cbOutDevice->addItem (out->at(i).getName(),deviceid );
	}
	int defoutdevice=Settings::getI ( "Sound/OutputDevice" );
	ui.cbOutDevice->setCurrentIndex(ui.cbOutDevice->findData(defoutdevice));

	ui.cbChannels->setCurrentIndex(ui.cbChannels->findData(Settings::get("Sound/Channels")));
	ui.sbSamplerate->setValue(Settings::getI("Sound/Samplerate"));
	return true;
}

bool SoundSettings::check()
{
	int inputDevice = ui.cbInDevice->itemData(ui.cbInDevice->currentIndex()).toInt();
	int outputDevice = ui.cbOutDevice->itemData(ui.cbOutDevice->currentIndex()).toInt();
	int channels = ui.cbChannels->itemData(ui.cbChannels->currentIndex()).toInt();
	int samplerate = ui.sbSamplerate->value();

	bool ok = this->sc->checkDeviceSupport(inputDevice, outputDevice, channels, samplerate);

	if (!ok)
		QMessageBox::critical(this, tr("Soundkonfiguration nicht unterstützt"), tr("Die ausgewählte Soundkonfiguration wird von der Hardware nicht unterstützt.\n\nBitte korrigieren Sie die Werte.\n\nFalls notwendig, wenden Sie sich bitte an Ihren Soundkartenhersteller."));

	
	if ((channels != Settings::getI("Model/Channels")) || 
			(samplerate != Settings::getI("Model/Samplerate")))
		QMessageBox::warning(this, tr("Modell- und Soundkonfiguration abweichend"), tr("Die konfigurierten Kanäle / Samplerate der Trainingstexte passt nicht mit den Soundeinstellungen zusammen.\n\nBitte korrigieren Sie ihre Konfiguration hier oder definieren Sie entsprechende Nachbearbeitungsketten um den Unterschied auszugleichen."));

	return ok;
}


/**
 * \brief As there _must_ be an audio device selected  it might as well be the correct one - return true
 * \author Peter Grasch
 * @return always true
 */
bool SoundSettings::isComplete()
{
	return true;
}

/**
 * \brief Applys the changes
 * \author Peter Grasch
 * @return success
 */
bool SoundSettings::apply()
{
	Settings::set("Sound/InputDevice", ui.cbInDevice->itemData(ui.cbInDevice->currentIndex()));
	Settings::set("Sound/OutputDevice", ui.cbOutDevice->itemData(
		      ui.cbOutDevice->currentIndex()));
	Settings::set("Sound/Channels", ui.cbChannels->itemData(ui.cbChannels->currentIndex()));
	Settings::set("Sound/Samplerate", ui.sbSamplerate->value());

	return check();
}

/**
 * \brief Resets the cxhanges
 * \author Peter Grasch
 * @return success
 */
bool SoundSettings::reset()
{
	return init();
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
SoundSettings::~SoundSettings()
{
	if (sc) delete sc;
	if (in) delete in;
	if (out) delete out;
}


