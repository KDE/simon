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
 #include <kgenericfactory.h>

K_PLUGIN_FACTORY( SoundSettingsFactory, 
			registerPlugin< SoundSettings >(); 
		)
        
K_EXPORT_PLUGIN( SoundSettingsFactory("SoundSettings") )

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
	
	QWidget *postProcessingConfig = new QWidget(this);
	postProcUi.setupUi(postProcessingConfig);

	QWidget *promptConfig = new QWidget(this);
	promptUi.setupUi(promptConfig);

	KPageWidgetItem *deviceConfItem = pageWidget->addPage(coreConfig, i18n("Gerätekonfiguration"));
	KPageWidgetItem *promptConfItem = pageWidget->addPage(promptConfig, i18n("Vorleseschriftart"));
	KPageWidgetItem *postProcConfItem = pageWidget->addPage(postProcessingConfig, i18n("Nachbearbeitung"));

	deviceConfItem->setIcon(KIcon("audio-card"));
	promptConfItem->setIcon(KIcon("draw-text"));
	postProcConfItem->setIcon(KIcon("applications-other"));

	deviceConfItem->setHeader("");
	promptConfItem->setHeader("");
	postProcConfItem->setHeader("");
	
	KAboutData *about = new KAboutData(
				"soundsettings", "", ki18n("Aufnahmen"),
				"0.1", ki18n("Einstellungen für die Aufnahme und Wiedergabe von Sounds"), KAboutData::License_GPL);
	about->setProgramIconName("preferences-desktop-sound");
	setAboutData( about );

	this->sc= new SoundControl();

	addConfig(SoundConfiguration::self(), this);
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
void SoundSettings::load()
{
	deviceUi.kcfg_SoundInputDevice->clear();
	deviceUi.kcfg_SoundOutputDevice->clear();

	SoundDeviceList *in = sc->getInputDevices();

	deviceUi.kcfg_SoundInputDevice->clear();
	for ( int i=0; i<in->count(); i++ )
	{
		int deviceid= in->at(i).getDeviceID();
		deviceUi.kcfg_SoundInputDevice->addItem (in->at(i).getName(),deviceid );
	}
	deviceUi.kcfg_SoundInputDevice->setCurrentIndex(
			deviceUi.kcfg_SoundInputDevice->findData(SoundConfiguration::soundInputDevice()));



	SoundDeviceList *out = sc->getOutputDevices();
	deviceUi.kcfg_SoundOutputDevice->clear();
	for ( int i=0; i<out->count(); i++ )
	{
		int deviceid= out->at (i).getDeviceID();
		deviceUi.kcfg_SoundOutputDevice->addItem (out->at(i).getName(),deviceid );
	}
	deviceUi.kcfg_SoundOutputDevice->setCurrentIndex(deviceUi.kcfg_SoundOutputDevice->findData(SoundConfiguration::soundOutputDevice()));

	KCModule::load();
}

bool SoundSettings::check()
{
	int inputDevice = deviceUi.kcfg_SoundInputDevice->itemData(deviceUi.kcfg_SoundInputDevice->currentIndex()).toInt();
	int outputDevice = deviceUi.kcfg_SoundOutputDevice->itemData(deviceUi.kcfg_SoundOutputDevice->currentIndex()).toInt();
	int channels = deviceUi.kcfg_SoundChannels->value();
	int samplerate = deviceUi.kcfg_SoundSampleRate->value();

	bool ok = this->sc->checkDeviceSupport(inputDevice, outputDevice, channels, samplerate);

	if (!ok)
		KMessageBox::error(this, i18n("Die ausgewählte Soundkonfiguration wird von der Hardware nicht unterstützt.\n\nBitte korrigieren Sie die Werte.\n\nFalls notwendig, wenden Sie sich bitte an Ihren Soundkartenhersteller."));

	
// 	if ((channels != CoreConfiguration::soundChannels()) || 
// 			(samplerate != CoreConfiguration::soundSampleRate()))
// 	{
// 		KMessageBox::information(this, i18n("Die konfigurierten Kanäle / Samplerate der Trainingstexte passt nicht mit den Soundeinstellungen zusammen.\n\nBitte korrigieren Sie ihre Konfiguration hier oder definieren Sie entsprechende Nachbearbeitungsketten um den Unterschied auszugleichen.") );
// 		ok=false;
// 	}

	return ok;
}


void SoundSettings::save()
{
	check();
	KCModule::save();
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


