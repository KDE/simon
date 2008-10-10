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

#include <KMessageBox>
#include <KIcon>
#include <QLabel>
#include <QCheckBox>
#include <QGridLayout>
#include <KPushButton>
#include <KConfigGroup>

#include <KLocalizedString>
#include <KAboutData>
#include <KEditListBox>
#include <KFontChooser>

QPointer<SoundSettings> SoundSettings::instance;

/**
 * \brief Constructor - inits the help text and the gui
 * \author Peter Grasch
 * @param parent the parent of the widget
 */
SoundSettings::SoundSettings(QWidget* parent, const QVariantList& args):
		KCModule(KGlobal::mainComponent(), parent)
{
	Q_UNUSED(args);

	
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////            GUI                    /////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	QHBoxLayout *mainLay = new QHBoxLayout(this);
	QTabWidget *tabWidget = new QTabWidget(this);
	mainLay->addWidget(tabWidget);
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	
	QWidget *coreConfig = new QWidget(this);
	QGridLayout *lay = new QGridLayout(coreConfig);
	lay->addWidget(new QLabel(i18n("Eingabegerät:"), this), 0, 0);
	lay->addWidget(new QLabel(i18n("Ausgabegerät:"), this), 1, 0);
	lay->addWidget(new QLabel(i18n("Kanäle:"), this), 2, 0);
	lay->addWidget(new QLabel(i18n("Samplerate:"), this), 3, 0);
	
	cbSoundInputDevice = new QComboBox(this);
	lay->addWidget(cbSoundInputDevice, 0, 1);
	cbSoundOutputDevice = new QComboBox(this);
	lay->addWidget(cbSoundOutputDevice, 1, 1);
	
	cbSoundChannels = new QComboBox(this);
	cbSoundChannels->addItem(i18n("Mono"), 1);
	cbSoundChannels->addItem(i18n("Stereo"), 2);
	lay->addWidget(cbSoundChannels, 2, 1);
	
	sbSoundSampleRate = new SimonSpinBox(this);
	sbSoundSampleRate->setMinimum(8000);
	sbSoundSampleRate->setMaximum(192000);
	lay->addWidget(sbSoundSampleRate, 3, 1);
	
	lay->setRowMinimumHeight(4,10);
	
	QHBoxLayout *hBox = new QHBoxLayout();
	
	KPushButton *pbTest = new KPushButton(this);
	pbTest->setText(i18n("Testen"));
	pbTest->setIcon(KIcon("help-hint"));
	connect(pbTest, SIGNAL(clicked()), this, SLOT(checkWithSuccessMessage()));
	hBox->addWidget(pbTest);
	
	hBox->addStretch();
	
	KPushButton *pbReload = new KPushButton(this);
	pbReload->setText(i18n("Neu laden"));
	pbReload->setIcon(KIcon("view-refresh"));
	connect(pbReload, SIGNAL(clicked()), this, SLOT(load()));
	hBox->addWidget(pbReload);
	
	lay->addLayout(hBox, 5, 0, 1, -1);
	lay->setRowStretch(6, 10);
	
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	
	QWidget *postProcessingConfig = new QWidget(this);
	QVBoxLayout *procLay = new QVBoxLayout(postProcessingConfig);
	
	cbProcessInternal = new QCheckBox(i18n("Auch interne Aufnahmen nachbearbeiten"), this);
	elwProcessingFilters = new KEditListBox(i18n("Nachbearbeitungskommandos"), this);
	procLay->addWidget(cbProcessInternal);
	procLay->addWidget(elwProcessingFilters);
	
	
	
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	fontChooser = new KFontChooser(this);
	
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	
	tabWidget->addTab(coreConfig, i18n("Gerätekonfiguration"));
	tabWidget->addTab(fontChooser, i18n("Vorleseschriftart"));
	tabWidget->addTab(postProcessingConfig, i18n("Nachbearbeitung"));

	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	
	
	KAboutData *about = new KAboutData(
				"soundsettings", "", ki18n("Aufnahmen"),
				"0.1", ki18n("Einstellungen für die Aufnahme und Wiedergabe von Sounds"), KAboutData::License_GPL);
	about->setProgramIconName("preferences-desktop-sound");
	setAboutData( about );

	config = KSharedConfig::openConfig(KGlobal::mainComponent(),
					"simonsoundrc");
	
	
	connect(cbSoundInputDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
	connect(cbSoundOutputDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
	connect(cbSoundChannels, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
	connect(sbSoundSampleRate, SIGNAL(valueChanged(int)), this, SLOT(slotChanged()));
	connect(fontChooser, SIGNAL(fontSelected (const QFont &)), this, SLOT(slotChanged()));
	connect(cbProcessInternal, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(elwProcessingFilters, SIGNAL(changed()), this, SLOT(slotChanged()));

	this->sc= new SoundControl();
	load();
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
	Q_ASSERT(config);

	KConfigGroup cg(config, "");
	
	storedInputDevice = cg.readEntry("InputDevice", 0);
	storedOutputDevice = cg.readEntry("OutputDevice", 0);
	storedChannels = cg.readEntry("Channels", 1);
	storedSampleRate = cg.readEntry("SampleRate", 16000);
	storedPromptFont = cg.readEntry("PromptFont", QFont());
	storedProcessInternal = cg.readEntry("ProcessInternal", false);
	storedPostProcessingFilters = cg.readEntry("ProcessingFilters", QStringList());
	
	fontChooser->setFont(storedPromptFont);
	
	cbProcessInternal->setChecked(storedProcessInternal);
	elwProcessingFilters->setItems(storedPostProcessingFilters);
	
	cbSoundInputDevice->clear();
	cbSoundOutputDevice->clear();
	
	SoundDeviceList *in = sc->getInputDevices();

	cbSoundInputDevice->clear();
	for ( int i=0; i<in->count(); i++ )
	{
		int deviceid= in->at(i).getDeviceID();
		cbSoundInputDevice->addItem (in->at(i).getName(),deviceid );
	}
	cbSoundInputDevice->setCurrentIndex(cbSoundInputDevice->findData(storedInputDevice));



	SoundDeviceList *out = sc->getOutputDevices();

	cbSoundOutputDevice->clear();
	for ( int i=0; i<out->count(); i++ )
	{
		int deviceid= out->at (i).getDeviceID();
		cbSoundOutputDevice->addItem (out->at(i).getName(),deviceid );
	}
	cbSoundOutputDevice->setCurrentIndex(cbSoundOutputDevice->findData(storedOutputDevice));

	
	cbSoundChannels->setCurrentIndex(cbSoundChannels->findData(storedChannels));
	
	sbSoundSampleRate->setValue(storedSampleRate);
	
	emit changed(false);
}

bool SoundSettings::check()
{
	int inputDevice = cbSoundInputDevice->itemData(cbSoundInputDevice->currentIndex()).toInt();
	int outputDevice = cbSoundOutputDevice->itemData(cbSoundOutputDevice->currentIndex()).toInt();
	int channels = cbSoundChannels->itemData(cbSoundChannels->currentIndex()).toInt();
	int samplerate = sbSoundSampleRate->value();

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
	
	storedInputDevice = cbSoundInputDevice->itemData(cbSoundInputDevice->currentIndex()).toInt();
	storedOutputDevice = cbSoundOutputDevice->itemData(cbSoundOutputDevice->currentIndex()).toInt();
	storedChannels = cbSoundChannels->itemData(cbSoundChannels->currentIndex()).toInt();
	storedSampleRate = sbSoundSampleRate->value();
	
	bool fontChanged=false;
	
	if (fontChooser->font() != storedPromptFont)
		fontChanged=true;
	storedPromptFont = fontChooser->font();
	
	if (fontChanged)
		emit promptFontChanged(fontChooser->font());

	storedProcessInternal = cbProcessInternal->isChecked();
	storedPostProcessingFilters = elwProcessingFilters->items();
	
	KConfigGroup cg(config, "");
	cg.writeEntry("InputDevice", storedInputDevice);
	cg.writeEntry("OutputDevice", storedOutputDevice);
	cg.writeEntry("Channels", storedChannels);
	cg.writeEntry("SampleRate", storedSampleRate);

	cg.writeEntry("PromptFont", storedPromptFont);

	cg.writeEntry("ProcessInternal", storedProcessInternal);
	cg.writeEntry("ProcessingFilters", storedPostProcessingFilters);
	
	cg.sync();
	emit changed(false);
}

void SoundSettings::defaults()
{
	cbSoundChannels->setCurrentIndex(0);
	sbSoundSampleRate->setValue(16000);
	save();
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
	instance=0;
	if (sc) delete sc;
}


