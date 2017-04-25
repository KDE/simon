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

#include "speechmodelsettings.h"
#include "basemodelsettings.h"
#include "modelmanager.h"
#include "speechmodelmanagementconfiguration.h"

#include "ui_trainingsettings.h"
#include "ui_languageprofilesettings.h"

#include <KFileDialog>
#include <KDateTime>
#include <KPageWidget>
#include <KTar>
#include <KPluginFactory>
#include <QStandardPaths>

#include <KMessageBox>

#include <KConfigGroup>

K_PLUGIN_FACTORY( SpeechModelSettingsFactory,
registerPlugin< SpeechModelSettings >();
)

// K_EXPORT_PLUGIN( SpeechModelSettingsFactory("simonlib") )

SpeechModelSettings::SpeechModelSettings(QWidget* parent, const QVariantList& args): KCModule(parent),
  uiTrainingsData(new Ui::TrainingSettingsWidget()), uiLanguageProfile(new Ui::LanguageProfileSettingsWidget())
{
  QVBoxLayout *lay = new QVBoxLayout(this);
  KPageWidget *pageWidget = new KPageWidget(this);
  lay->addWidget(pageWidget);

  if (args.count() == 1)
    pageWidget->setFaceType(KPageView::Tabbed);


  baseModelSettings = new BaseModelSettings(this);
  connect(baseModelSettings, SIGNAL(changed(bool)), this, SLOT(slotSubChanged(bool)));

  QWidget *trainingsDataWidget = new QWidget(this);
  uiTrainingsData->setupUi(trainingsDataWidget);


  QWidget *languageProfileWidget = new QWidget(this);
  uiLanguageProfile->setupUi(languageProfileWidget);


  KPageWidgetItem *baseModel = pageWidget->addPage(baseModelSettings, i18n("Base Model"));
  baseModel->setIcon(QIcon::fromTheme("view-statistics"));
  baseModel->setHeader("");
  KPageWidgetItem *trainingsData = pageWidget->addPage(trainingsDataWidget, i18n("Training data"));
  trainingsData->setIcon(QIcon::fromTheme("view-pim-news"));
  trainingsData->setHeader("");
  KPageWidgetItem *languageProfile = pageWidget->addPage(languageProfileWidget, i18n("Language Profile"));
  languageProfile->setIcon(QIcon::fromTheme("applications-education-language"));
  languageProfile->setHeader("");

  addConfig(SpeechModelManagementConfiguration::self(), this);
  uiLanguageProfile->pbLoadLanguageProfile->setIcon(QIcon::fromTheme("document-open"));

  connect(uiLanguageProfile->pbLoadLanguageProfile, SIGNAL(clicked()), this, SLOT(loadLanguageProfile()));
}

void SpeechModelSettings::slotSubChanged(bool changed)
{
  if (changed)
    slotChanged();
}

void SpeechModelSettings::slotChanged()
{
  emit changed(true);
}

void SpeechModelSettings::load()
{
  baseModelSettings->load();
  uiLanguageProfile->lbProfileName->setText(ModelManager::getInstance()->languageProfileName());
  KCModule::load();
  emit changed(false);
}

void SpeechModelSettings::save()
{
  baseModelSettings->save();
  if (!m_languageProfileToImport.isEmpty()) {
    QString targetPathLanguageProfile = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "model/languageProfile";
    if (QFile::exists(targetPathLanguageProfile) && !QFile::remove(targetPathLanguageProfile)) {
      KMessageBox::sorry(this, i18n("Could not remove current language profile"));
      return;
    }
    if (!QFile::copy(m_languageProfileToImport, targetPathLanguageProfile))
      KMessageBox::sorry(this, i18n("Could not import language profile."));
    else
      touchLanguageProfileDate();

    m_languageProfileToImport.clear();
  }

  ModelManager::getInstance()->setLanguageProfileName(uiLanguageProfile->lbProfileName->text());

  int smpFreq = SpeechModelManagementConfiguration::modelSampleRate();
  KCModule::save();
  if (smpFreq != SpeechModelManagementConfiguration::modelSampleRate()) {
    //it changed
    KMessageBox::information(this, i18n("The sample rate has been changed. This will influence the generated speech model.\n\nHowever, the model needs to be recompiled. Please start a manual synchronization now."));
  }
}


void SpeechModelSettings::defaults()
{
  baseModelSettings->defaults();
  ModelManager::getInstance()->setLanguageProfileName(i18nc("Filename of a not yet selected language profile", "None"));

  QFile::remove(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "model/languageProfile");

  m_languageProfileToImport.clear();

  KCModule::defaults();

  load();
}

void SpeechModelSettings::touchLanguageProfileDate()
{
  KConfig config( QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "model/modelsrcrc", KConfig::SimpleConfig) ;
  KConfigGroup cGroup(&config, "");
  cGroup.writeEntry("LanguageDescriptionDate", KDateTime::currentUtcDateTime().dateTime());
  config.sync();
}

void SpeechModelSettings::loadLanguageProfile()
{
  QString path = KFileDialog::getOpenFileName(QUrl(), QString(), this, i18n("Select language profile"));
  if (path.isEmpty()) return;

  m_languageProfileToImport = path;
  emit changed(true);
  uiLanguageProfile->lbProfileName->setText(QFileInfo(path).fileName());
}

#include "speechmodelsettings.moc"
