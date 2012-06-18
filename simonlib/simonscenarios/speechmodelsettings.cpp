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

#include "speechmodelsettings.h"
#include <simonscenarios/scenariomanager.h>
#include "basemodelsettings.h"
#include "model.h"
#include "speechmodelmanagementconfiguration.h"

#include <QDomDocument>
#include <QDomElement>

#include <KMessageBox>
#include <KGlobal>
#include <KFileDialog>
#include <KDateTime>
#include <KPageWidget>
#include <KTar>
#include <kgenericfactory.h>

K_PLUGIN_FACTORY( SpeechModelSettingsFactory,
registerPlugin< SpeechModelSettings >();
)

K_EXPORT_PLUGIN( SpeechModelSettingsFactory("simonlib") )

SpeechModelSettings::SpeechModelSettings(QWidget* parent, const QVariantList& args): KCModule(KGlobal::mainComponent(), parent)
{
  QVBoxLayout *lay = new QVBoxLayout(this);
  KPageWidget *pageWidget = new KPageWidget(this);
  lay->addWidget(pageWidget);

  if (args.count() == 1)
    pageWidget->setFaceType(KPageView::Tabbed);

  
  QWidget *baseModelWidget = new QWidget(this);
  ui.setupUi(baseModelWidget);
  
  QWidget *trainingsDataWidget = new QWidget(this);
  uiTrainingsData.setupUi(trainingsDataWidget);
  
  
  QWidget *languageProfileWidget = new QWidget(this);
  uiLanguageProfile.setupUi(languageProfileWidget);


  KPageWidgetItem *baseModel = pageWidget->addPage(baseModelWidget, i18n("Base Model"));
  baseModel->setIcon(KIcon("view-statistics"));
  baseModel->setHeader("");
  KPageWidgetItem *trainingsData = pageWidget->addPage(trainingsDataWidget, i18n("Training data"));
  trainingsData->setIcon(KIcon("view-pim-news"));
  trainingsData->setHeader("");
  KPageWidgetItem *languageProfile = pageWidget->addPage(languageProfileWidget, i18n("Language Profile"));
  languageProfile->setIcon(KIcon("applications-education-language"));
  languageProfile->setHeader("");

  addConfig(SpeechModelManagementConfiguration::self(), this);
  
  connect(ui.rbStatic, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
  connect(ui.rbBaseModel, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
  connect(ui.rbDynamic, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));

  uiLanguageProfile.pbLoadLanguageProfile->setIcon(KIcon("document-open"));
  
  connect(uiLanguageProfile.pbLoadLanguageProfile, SIGNAL(clicked()), this, SLOT(loadLanguageProfile()));

  connect(ui.pbCreate, SIGNAL(clicked()), this, SLOT(createBaseModel()));
  connect(ui.pbOpen, SIGNAL(clicked()), this, SLOT(openBaseModel()));
  connect(ui.pbExport, SIGNAL(clicked()), this, SLOT(exportBaseModel()));
}

void SpeechModelSettings::exportBaseModel()
{
  QString activePath = KStandardDirs::locate("appdata", "model/active.sbm");
  if (!QFile::exists(activePath)) {
    KMessageBox::sorry(this, i18n("There is no active model currently available."));
    return;
  }
  QString path = KFileDialog::getSaveFileName(KUrl(), "*.sbm", this, i18n("Select output file name"));
  if (path.isEmpty())
    return;
  if (!QFile::copy(activePath, path))
    KMessageBox::sorry(this, i18n("Could not copy active model to \"%1\".", path));
}

void SpeechModelSettings::createBaseModel()
{
  QPointer<BaseModelSettings> baseModelSettings(new BaseModelSettings(this));
  importBaseModel(baseModelSettings->buildModel());
  delete baseModelSettings;
}

void SpeechModelSettings::openBaseModel()
{
  importBaseModel(KFileDialog::getOpenFileName(KUrl(), "*.sbm", this, i18n("Open Simon base model")));
}

void SpeechModelSettings::importBaseModel ( const QString& path )
{
  if (!path.isEmpty()) {
    m_baseModelToImport = path;
    updateBaseModelDescription(path);
    emit changed(true);
  }
}

void SpeechModelSettings::slotChanged()
{
  emit changed(true);
}

void SpeechModelSettings::updateBaseModelDescription ( const QString& path )
{
  KTar tar(path, "application/x-gzip");
  QString description = i18n("No description of model available");
  Model::parseContainer(tar, m_baseModelDate, m_baseModelName);
  description = baseModelDescription();
  
  ui.lbModelName->setText(description);
}

QString SpeechModelSettings::baseModelDescription()
{
  if (m_baseModelName == "None")
    return i18n("No base model loaded");

  return i18nc("%1 is model name, %2 is creation date", "%1 (Created: %2)", 
                      m_baseModelName, m_baseModelDate.toString(Qt::LocalDate));
}

void SpeechModelSettings::load()
{
  switch (ScenarioManager::getInstance()->baseModelType()) {
    case 0: ui.rbStatic->animateClick();
    break;
    case 1: ui.rbBaseModel->animateClick();
    break;
    case 2: ui.rbDynamic->animateClick();
    break;
    default:
      kDebug() << "Unknown model type" << ScenarioManager::getInstance()->baseModelType();
  }

  m_storedModelType = ScenarioManager::getInstance()->baseModelType();
  m_baseModelName = ScenarioManager::getInstance()->baseModelName();
  m_baseModelDate = ScenarioManager::getInstance()->baseModelCreationDate();
  
  ui.lbModelName->setText(baseModelDescription());
  
  KCModule::load();
  emit changed(false);
}

void SpeechModelSettings::save()
{
  int modelType = 0;
  if (ui.rbBaseModel->isChecked())
    modelType = 1;
  else if (ui.rbDynamic->isChecked())
    modelType = 2;

  if (!m_baseModelToImport.isEmpty()) {
    QString targetPath = KStandardDirs::locateLocal("appdata", "model/basemodel.sbm");
    
    bool succ = true;
    if (QFile::exists(targetPath) && !QFile::remove(targetPath)) {
      KMessageBox::sorry(this, i18n("Could not remove current base model"));
      succ = false;
      return;
    }
    if (!QFile::copy(m_baseModelToImport, targetPath)) {
      KMessageBox::sorry(this, i18n("Could not import hmm definitions."));
      succ = false;
    }

    m_baseModelToImport.clear();
    if (!succ) {
      m_baseModelName = ScenarioManager::getInstance()->baseModelName();
      m_baseModelDate = ScenarioManager::getInstance()->baseModelCreationDate();
    } else {
      ScenarioManager::getInstance()->setBaseModel(m_baseModelName, m_baseModelDate);
    }
  }
  ScenarioManager::getInstance()->setBaseModelType(modelType);
  
  if (!m_languageProfileToImport.isEmpty()) {
    QString targetPathLanguageProfile = KStandardDirs::locateLocal("appdata", "model/languageProfile");
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
  
  ScenarioManager::getInstance()->setLanguageProfileName(uiLanguageProfile.lbProfileName->text());
  
  int smpFreq = SpeechModelManagementConfiguration::modelSampleRate();
  KCModule::save();
  if (smpFreq != SpeechModelManagementConfiguration::modelSampleRate()) {
    //it changed
    KMessageBox::information(this, i18n("The sample rate has been changed. This will influence the generated speech model.\n\nHowever, the model needs to be recompiled. Please start a manual synchronization now."));
  }
}


void SpeechModelSettings::defaults()
{
  QString noName = i18nc("Filename of a not yet selected base model", "None");
  ScenarioManager::getInstance()->setBaseModel(noName, QDateTime());
  ScenarioManager::getInstance()->setBaseModelType(2);
  
  ScenarioManager::getInstance()->setLanguageProfileName(noName);

  QFile::remove(KStandardDirs::locateLocal("appdata", "model/basemodel.sbm"));
  QFile::remove(KStandardDirs::locateLocal("appdata", "model/languageProfile"));

  m_baseModelToImport.clear();
  m_languageProfileToImport.clear();
  
  KCModule::defaults();

  load();
}

void SpeechModelSettings::touchLanguageProfileDate()
{
  KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  cGroup.writeEntry("LanguageDescriptionDate", KDateTime::currentUtcDateTime().dateTime());
  config.sync();
}

void SpeechModelSettings::loadLanguageProfile()
{
  QString path = KFileDialog::getOpenFileName(KUrl(), QString(), this, i18n("Select language profile"));
  if (path.isEmpty()) return;

  m_languageProfileToImport = path;
  emit changed(true);
  uiLanguageProfile.lbProfileName->setText(QFileInfo(path).fileName());
}

