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
SoundSettings::SoundSettings(QWidget* parent): SystemWidget(tr("Soundeinstellungen"), QIcon(":/images/icons/gnome-settings-sound.svg"), tr("Konfigurieren Sie hier ihre Audiogeräte und legen Einstellungen für dessen Verwendung fest"), parent)
{
	ui.setupUi(this);
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
	
	
	connect ( ui.cbInDevice, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( refreshDeviceCapabilities() ) );
}

/**
 * \author Peter Grasch
 * \brief Asks the SoundControl for the needed infos and inserts the data in the comboboxes
 * \return success
 */
bool SoundSettings::init()
{
	SoundDeviceList *sd=sc->getInputDevices();
	ui.cbInDevice->clear();
	QString defindevice=Settings::get ( "Sound/InputDevice" ).toString();
	for ( int i=0; i<sd->count(); i++ )
	{
		QString deviceid= ( ( SoundDevice ) sd->at ( i ) ).getDeviceID();
		ui.cbInDevice->addItem ( ( ( SoundDevice ) sd->at ( i ) ).getName(),deviceid );

		if (deviceid==defindevice) 
			ui.cbInDevice->setCurrentIndex(i);
	}

	sd=sc->getOutputDevices();
	ui.cbOutDevice->clear();
	QString defoutdevice=Settings::get ( "Sound/OutputDevice" ).toString();
	for ( int i=0; i<sd->count(); i++ )
	{
		QString deviceid= ( ( SoundDevice ) sd->at ( i ) ).getDeviceID();
		ui.cbOutDevice->addItem ( ( ( SoundDevice ) sd->at ( i ) ).getName(),deviceid );

		if ( deviceid ==defoutdevice ) 
			ui.cbOutDevice->setCurrentIndex ( i );
	}

	ui.cbChannels->setCurrentIndex(ui.cbChannels->findData(Settings::get("Sound/Channels")));
	ui.cbSampleRate->setCurrentIndex(ui.cbSampleRate->findText(
					Settings::get("Sound/Samplerate").toString()));
	
	ui.hsMic->setValue(Settings::get("Sound/InputVolume").toInt());
	return true;
}

/**
 * \brief Refreshes the capabilities of the available devices
 * \author Peter Grasch
 */
void SoundSettings::refreshDeviceCapabilities()
{
	ui.cbChannels->clear();
	ui.cbSampleRate->clear();
	if ( sc->getChannel ( ui.cbInDevice->currentText() ) >1 ) 
		ui.cbChannels->addItem ( "Stereo", 2 );

	if ( sc->getChannel ( ui.cbInDevice->currentText() ) >0 ) 
		ui.cbChannels->addItem ( "Mono", 1 );

	QList<int>* samplerates;
	samplerates=sc->getSamplerate ( ui.cbInDevice->currentText() );
	if ( !samplerates ) return;
	qSort ( samplerates->begin(), samplerates->end(),  qGreater<int>() );
	for ( int i=0; i<samplerates->count(); i++ )
	{
		ui.cbSampleRate->addItem ( QString::number ( samplerates->at ( i ) ) );
	}
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
	Settings::set("Sound/Samplerate", ui.cbSampleRate->currentText());
	Settings::set("Sound/InputVolume", ui.hsMic->value());
	
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
}


