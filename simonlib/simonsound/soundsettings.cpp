/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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
#include "soundconfig.h"
#include "soundserver.h"
#include "devicesettings.h"
#include "samplegroupcontextsettings.h"

#include <KMessageBox>
#include <QVBoxLayout>
#include <KIcon>

#include <KLocalizedString>
#include <KAboutData>
#include <KPageWidgetItem>
#include <KPageWidget>
#include <KDebug>
#include <KPluginFactory>

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

  deviceSettings = new DeviceSettings(this);
  connect(deviceSettings, SIGNAL(changed(bool)), this, SIGNAL(changed(bool)));

  QWidget *vadConfig = new QWidget(this);
  vadUi.setupUi(vadConfig);

  QWidget *postProcessingConfig = new QWidget(this);
  postProcUi.setupUi(postProcessingConfig);

  QWidget *trainingConfig = new QWidget(this);
  trainingSettignsUi.setupUi(trainingConfig);

  m_sampleGroupContext = new SampleGroupContextSettings(this);
  QWidget *sampleGroupConditions = m_sampleGroupContext;
  connect(m_sampleGroupContext, SIGNAL(changed()),
          this, SLOT(slotChanged()));

  //	QWidget *promptConfig = new QWidget(this);
  //	promptUi.setupUi(promptConfig);

  KPageWidgetItem *deviceConfItem = pageWidget->addPage(deviceSettings, i18n("Device Configuration"));
  KPageWidgetItem *vadConfItem = pageWidget->addPage(vadConfig, i18n("Voice Activity Detection"));
  KPageWidgetItem *trainingConfigItem = pageWidget->addPage(trainingConfig, i18n("Training"));
  //KPageWidgetItem *promptConfItem = pageWidget->addPage(promptConfig, i18n("Prompt Font"));
  KPageWidgetItem *postProcConfItem = pageWidget->addPage(postProcessingConfig, i18n("Post-Processing"));
  KPageWidgetItem *sampleGroupConditionItem = pageWidget->addPage(sampleGroupConditions, i18n("Context"));

  deviceConfItem->setIcon(KIcon("audio-card"));
  vadConfItem->setIcon(KIcon("media-playback-start"));
  trainingConfigItem->setIcon(KIcon("view-pim-news"));
  //promptConfItem->setIcon(KIcon("draw-text"));
  postProcConfItem->setIcon(KIcon("applications-other"));
  sampleGroupConditionItem->setIcon(KIcon("preferences-activities"));

  deviceConfItem->setHeader("");
  vadConfItem->setHeader("");
  trainingConfigItem->setHeader("");
  //promptConfItem->setHeader("");
  postProcConfItem->setHeader("");
  sampleGroupConditionItem->setHeader("");

  KAboutData *about = new KAboutData(
    "soundsettings", "", ki18n("Recordings"),
    "0.1", ki18n("Configuration for the Recording and Playback of sounds"), KAboutData::License_GPL);
  #if KDE_IS_VERSION(4,0,80)
  about->setProgramIconName("preferences-desktop-sound");
  #endif
  setAboutData( about );

  addConfig(SoundConfiguration::self(), this);
}


/**
 * \author Peter Grasch
 * \return success
 */
void SoundSettings::load()
{
  deviceSettings->load();

  KCModule::load();
}


void SoundSettings::save()
{
  //even when not enabled this call will be save
  //The sound input / output devices are not affected by this
  //and this way we store the configuration regarding the prompt font
  //and the postprocessing commands
  KCModule::save();

  m_sampleGroupContext->saveChanges();

  deviceSettings->save();
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
}
