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
#include "soundcontrol.h"

#include "simoncombobox.h"
#include "simonspinbox.h"
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
		KCModule(KGlobal::mainComponent(), parent)
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
	
	QWidget *postProcessingConfig = new QWidget(this);
	postProcUi.setupUi(postProcessingConfig);

	QWidget *promptConfig = new QWidget(this);
	promptUi.setupUi(promptConfig);

	KPageWidgetItem *deviceConfItem = pageWidget->addPage(coreConfig, i18n("Device Configuration"));
	KPageWidgetItem *promptConfItem = pageWidget->addPage(promptConfig, i18n("Prompt Font"));
	KPageWidgetItem *postProcConfItem = pageWidget->addPage(postProcessingConfig, i18n("Post-Processing"));

	deviceConfItem->setIcon(KIcon("audio-card"));
	promptConfItem->setIcon(KIcon("draw-text"));
	postProcConfItem->setIcon(KIcon("applications-other"));

	deviceConfItem->setHeader("");
	promptConfItem->setHeader("");
	postProcConfItem->setHeader("");
	
	KAboutData *about = new KAboutData(
				"soundsettings", "", ki18n("Recordings"),
				"0.1", ki18n("Configuration for the Recording and Playback of sounds"), KAboutData::License_GPL);
	about->setProgramIconName("preferences-desktop-sound");
	setAboutData( about );

	this->sc= new SoundControl();

	addConfig(SoundConfiguration::self(), this);
}


void SoundSettings::checkWithSuccessMessage()
{
	if (check())
		KMessageBox::information(this, i18n("The soundconfiguration has been tested successfully."));
}

/**
 * \author Peter Grasch
 * \brief Asks the SoundControl for the needed infos and inserts the data in the comboboxes
 * \return success
 */
void SoundSettings::load()
{
	deviceUi.cbSoundInputDevice->clear();
	deviceUi.cbSoundOutputDevice->clear();

	SoundDeviceList *in = sc->getInputDevices();

	deviceUi.cbSoundInputDevice->clear();
	for ( int i=0; i<in->count(); i++ )
	{
		int deviceid= in->at(i).getDeviceID();
		deviceUi.cbSoundInputDevice->addItem (in->at(i).getName(),deviceid );
	}
	deviceUi.cbSoundInputDevice->setCurrentIndex(
			deviceUi.cbSoundInputDevice->findData(SoundConfiguration::soundInputDevice()));



	SoundDeviceList *out = sc->getOutputDevices();
	deviceUi.cbSoundOutputDevice->clear();
	for ( int i=0; i<out->count(); i++ )
	{
		int deviceid= out->at (i).getDeviceID();
		deviceUi.cbSoundOutputDevice->addItem (out->at(i).getName(),deviceid );
	}
	deviceUi.cbSoundOutputDevice->setCurrentIndex(deviceUi.cbSoundOutputDevice->findData(SoundConfiguration::soundOutputDevice()));

	KCModule::load();
}


bool SoundSettings::check()
{
	int inputDevice = getSelectedInputDeviceId();
	int outputDevice = getSelectedOutputDeviceId();
	int channels = deviceUi.kcfg_SoundChannels->value();
	int samplerate = deviceUi.kcfg_SoundSampleRate->value();

	bool ok = this->sc->checkDeviceSupport(inputDevice, outputDevice, channels, samplerate);

	if (!ok)
		KMessageBox::error(this, i18n("The selected sound configuration is not supported by your hardware.\n\nPlease double-check your configuration and, if necessairy, please contact your vendor."));

	return ok;
}

int SoundSettings::getSelectedInputDeviceId()
{
	return deviceUi.cbSoundInputDevice->itemData(deviceUi.cbSoundInputDevice->currentIndex()).toInt();
}

int SoundSettings::getSelectedOutputDeviceId()
{
	return deviceUi.cbSoundOutputDevice->itemData(deviceUi.cbSoundOutputDevice->currentIndex()).toInt();
}

void SoundSettings::save()
{
	check();
	KCModule::save();
	SoundConfiguration::setSoundInputDevice(getSelectedInputDeviceId());
	SoundConfiguration::setSoundOutputDevice(getSelectedOutputDeviceId());

	KSharedConfig::Ptr config = KSharedConfig::openConfig("simonsoundrc");
	KConfigGroup group(config, "Devices");
	group.writeEntry("SoundInputDevice", getSelectedInputDeviceId());
	group.writeEntry("SoundOutputDevice", getSelectedOutputDeviceId());
#ifdef Q_OS_LINUX
	group.writeEntry("SoundInputDeviceALSAName", sc->idToALSAName(getSelectedInputDeviceId()));
#endif
	config->sync();


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
	if (sc) delete sc;
}


