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
#include "soundcontrol.h"
#include "settings.h"

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
	help = tr("Hier finden Sie alle Einstellungen zur Spracheingabe und Tonausgabe.\n\nWenn Sie aktivieren, dass Sie alle Aufnahmen speichern wollen, wird nach jeder Aufnahme eine Audiodatei mit Datum und Uhrzeit, sowie die erkannte Transkribtion im angegebenen Verzeichnis erstellt.\nBitte bedenken Sie, das dies sehr viel Platz in Anspruch nimmt.");
	
	this->sc= new SoundControl();
	
#ifdef linux
	ui.lbDirectX->setVisible ( false );
	ui.lbALSA->setVisible ( true );
#endif
#ifdef __WIN32
	ui.lbDirectX->setVisible ( true );
	ui.lbALSA->setVisible ( false );
#endif
	
	connect ( ui.cbInDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshDeviceCapabilitiesIn()));
	connect ( ui.cbOutDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshDeviceCapabilitiesOut()));

	connect ( ui.cbInDevice, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
	connect ( ui.cbOutDevice, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
	connect ( ui.cbChannelsIn, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
	connect ( ui.cbSampleRateIn, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
	connect ( ui.cbChannelsOut, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
	connect ( ui.cbSampleRateOut, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
}

/**
 * \author Peter Grasch
 * \brief Asks the SoundControl for the needed infos and inserts the data in the comboboxes
 * \return success
 */
bool SoundSettings::init()
{
	this->in = sc->getInputDevices();
	
	ui.cbInDevice->clear();
	int defindevice=Settings::get ( "Sound/InputDevice" ).toInt();
	for ( int i=0; i<in->count(); i++ )
	{
		int deviceid= ( ( SoundDevice ) in->at ( i ) ).getDeviceID();
		ui.cbInDevice->addItem ( ( ( SoundDevice ) in->at ( i ) ).getName(),deviceid );

		if (deviceid==defindevice) 
			ui.cbInDevice->setCurrentIndex(i);
	}


	out =sc->getOutputDevices();
	ui.cbOutDevice->clear();
	int defoutdevice=Settings::get ( "Sound/OutputDevice" ).toInt();
	for ( int i=0; i<out->count(); i++ )
	{
		int deviceid= ( ( SoundDevice ) out->at ( i ) ).getDeviceID();
		ui.cbOutDevice->addItem ( ( ( SoundDevice ) out->at ( i ) ).getName(),deviceid );

		if ( deviceid ==defoutdevice ) 
			ui.cbOutDevice->setCurrentIndex ( i );
	}

	ui.cbChannelsIn->setCurrentIndex(ui.cbChannelsIn->findData(Settings::get("Sound/ChannelsIn")));
	ui.cbSampleRateIn->setCurrentIndex(ui.cbSampleRateIn->findText(
					Settings::get("Sound/SamplerateIn").toString()));
	ui.cbChannelsOut->setCurrentIndex(ui.cbChannelsOut->findData(Settings::get("Sound/ChannelsOut")));
	ui.cbSampleRateOut->setCurrentIndex(ui.cbSampleRateOut->findText(
					Settings::get("Sound/SamplerateOut").toString()));
	return true;
}

void SoundSettings::refresh(SoundDevice sd, QComboBox *cbChannels, QComboBox *sampleRates)
{
	cbChannels->clear();
	sampleRates->clear();

	QList<int> channels = sd.getChannels();
	QList<int> samplerate = sd.getSamplerates();
	for (int i=0; i < channels.count(); i++)
	{
		QString channelstr;
		if (channels[i] == 1)
			channelstr = "Mono";
		else if (channels[i] == 2)
			channelstr = "Stereo";
		else channelstr = QString::number(channels[i]);

		cbChannels->addItem(channelstr, channels[i]);
	}
	for (int i=0; i < samplerate.count(); i++)
	{
		sampleRates->addItem(QString::number(samplerate[i]));
	}
}

void SoundSettings::refreshDeviceCapabilitiesIn()
{
	if ((ui.cbInDevice->currentIndex() == -1) || !in) return;

	refresh(in->at(ui.cbInDevice->currentIndex()), ui.cbChannelsIn, ui.cbSampleRateIn);
}

void SoundSettings::refreshDeviceCapabilitiesOut()
{
	if (!out || (ui.cbOutDevice->currentIndex() == -1)) return;
	refresh(out->at(ui.cbOutDevice->currentIndex()), ui.cbChannelsOut, ui.cbSampleRateOut);
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
	Settings::set("Sound/ChannelsIn", ui.cbChannelsIn->itemData(ui.cbChannelsIn->currentIndex()));
	Settings::set("Sound/SamplerateIn", ui.cbSampleRateIn->currentText());
	Settings::set("Sound/ChannelsOut", ui.cbChannelsOut->itemData(ui.cbChannelsOut->currentIndex()));
	Settings::set("Sound/SamplerateOut", ui.cbSampleRateOut->currentText());

	return true;
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
    delete sc;
    delete in;
    delete out;
}


