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
#include "speechmodelmanagementconfiguration.h"
#include <KMessageBox>
#include <KGlobal>
#include <KFileDialog>
#include <KDateTime>
#include <KPageWidget>
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
  KPageWidgetItem *trainingsData = pageWidget->addPage(trainingsDataWidget, i18n("Trainingsdata"));
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
  
  ui.pbLoadBaseHMM->setIcon(KIcon("document-open"));
  ui.pbLoadBaseTiedlist->setIcon(KIcon("document-open"));
  ui.pbLoadBaseMacros->setIcon(KIcon("document-open"));
  ui.pbLoadBaseStats->setIcon(KIcon("document-open"));
  
  connect(uiLanguageProfile.pbLoadLanguageProfile, SIGNAL(clicked()), this, SLOT(loadLanguageProfile()));

  connect(ui.pbLoadBaseHMM, SIGNAL(clicked()), this, SLOT(loadBaseHMM()));
  connect(ui.pbLoadBaseTiedlist, SIGNAL(clicked()), this, SLOT(loadBaseTiedlist()));
  connect(ui.pbLoadBaseMacros, SIGNAL(clicked()), this, SLOT(loadBaseMacros()));
  connect(ui.pbLoadBaseStats, SIGNAL(clicked()), this, SLOT(loadBaseStats()));
}

void SpeechModelSettings::slotChanged()
{
  emit changed(true);
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

  ui.lbLastLoadedBaseHMM->setText(
      translateDefault(ScenarioManager::getInstance()->baseModelHMMName()));
  ui.lbLastLoadedBaseTiedlist->setText(
      translateDefault(ScenarioManager::getInstance()->baseModelTiedlistName()));
  ui.lbLastLoadedBaseMacros->setText(
      translateDefault(ScenarioManager::getInstance()->baseModelMacros()));
  ui.lbLastLoadedBaseStats->setText(
      translateDefault(ScenarioManager::getInstance()->baseModelStats()));
  uiLanguageProfile.lbProfileName->setText(
      translateDefault(ScenarioManager::getInstance()->languageProfileName()));
  KCModule::load();
  emit changed(false);
}

QString SpeechModelSettings::translateDefault(const QString& in)
{
  if (in == "None")
    return i18nc("Filename of a not yet selected file", "None");

  return in;
}

void SpeechModelSettings::save()
{
  int modelType = 0;
  if (ui.rbBaseModel->isChecked())
    modelType = 1;
  else if (ui.rbDynamic->isChecked())
    modelType = 2;

  if (!m_hmmDefsToImport.isEmpty()) {
    QString targetPathHMM = KStandardDirs::locateLocal("appdata", "model/basehmmdefs");
    if (QFile::exists(targetPathHMM) && !QFile::remove(targetPathHMM)) {
      KMessageBox::sorry(this, i18n("Could not remove current base model"));
      return;
    }
    if (!QFile::copy(m_hmmDefsToImport, targetPathHMM))
      KMessageBox::sorry(this, i18n("Could not import hmm definitions."));

    m_hmmDefsToImport.clear();
  }
  
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

  if (!m_tiedlistToImport.isEmpty()) {
    QString targetPathTiedlist = KStandardDirs::locateLocal("appdata", "model/basetiedlist");
    if (QFile::exists(targetPathTiedlist) && !QFile::remove(targetPathTiedlist)) {
      KMessageBox::sorry(this, i18n("Could not remove current base model"));
      return;
    }
    if (!QFile::copy(m_tiedlistToImport, targetPathTiedlist))
      KMessageBox::sorry(this, i18n("Could not import tiedlist."));
    m_tiedlistToImport.clear();
  }

  if (!m_macrosToImport.isEmpty()) {
    QString targetPathMacros = KStandardDirs::locateLocal("appdata", "model/basemacros");
    if (QFile::exists(targetPathMacros) && !QFile::remove(targetPathMacros)) {
      KMessageBox::sorry(this, i18n("Could not remove current base model"));
      return;
    }
    if (!QFile::copy(m_macrosToImport, targetPathMacros))
      KMessageBox::sorry(this, i18n("Could not import macros."));
    m_macrosToImport.clear();
  }

  if (!m_statsToImport.isEmpty()) {
    QString targetPathStats = KStandardDirs::locateLocal("appdata", "model/basestats");
    if (QFile::exists(targetPathStats) && !QFile::remove(targetPathStats)) {
      KMessageBox::sorry(this, i18n("Could not remove current base model"));
      return;
    }
    if (!QFile::copy(m_statsToImport, targetPathStats))
      KMessageBox::sorry(this, i18n("Could not import stats."));
    m_statsToImport.clear();
  }

  ScenarioManager::getInstance()->setBaseModel(modelType, ui.lbLastLoadedBaseHMM->text(), ui.lbLastLoadedBaseTiedlist->text(),
    ui.lbLastLoadedBaseMacros->text(), ui.lbLastLoadedBaseStats->text());
  
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
  QString noName = i18nc("Filename of a not yet selected file", "None");
  ScenarioManager::getInstance()->setBaseModel(2, noName, noName, noName, noName);
  
  ScenarioManager::getInstance()->setLanguageProfileName(noName);

  QFile::remove(KStandardDirs::locateLocal("appdata", "model/basehmmdefs"));
  QFile::remove(KStandardDirs::locateLocal("appdata", "model/basetiedlist"));
  QFile::remove(KStandardDirs::locateLocal("appdata", "model/basemacros"));
  QFile::remove(KStandardDirs::locateLocal("appdata", "model/basestats"));
  QFile::remove(KStandardDirs::locateLocal("appdata", "model/languageProfile"));

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

void SpeechModelSettings::importBaseModelFromDirectory(QDir dir)
{
  lastDirectory = dir.path();
  dir.setFilter(QDir::Files);
  
  Q_FOREACH(const QString& file, dir.entryList()) {
    if (m_hmmDefsToImport.isEmpty() && file.toLower().contains("hmm")) {
      m_tiedlistToImport = file;
      QFileInfo f(file);
      lastDirectory = f.path();
      ui.lbLastLoadedBaseTiedlist->setText(f.fileName());
    }
    
    if (m_tiedlistToImport.isEmpty() && file.toLower().contains("tiedlist")) {
      m_tiedlistToImport = file;
      QFileInfo f(file);
      lastDirectory = f.path();
      ui.lbLastLoadedBaseTiedlist->setText(f.fileName());
    }
    
    if (m_macrosToImport.isEmpty() && file.toLower().contains("macros")) {
      m_macrosToImport = file;
      QFileInfo f(file);
      lastDirectory = f.path();
      ui.lbLastLoadedBaseMacros->setText(QFileInfo(file).fileName());
    }
    
    if (m_statsToImport.isEmpty() && file.toLower().contains("stats")) {
      m_statsToImport = file;
      QFileInfo f(file);
      lastDirectory = f.path();
      ui.lbLastLoadedBaseStats->setText(f.fileName());
    }
  }
  emit changed(true);
}

void SpeechModelSettings::loadBaseHMM()
{
  QString path = KFileDialog::getOpenFileName(KUrl(lastDirectory), QString(), this, i18n("Select base hmmdefs"));
  if (path.isEmpty()) return;

  m_hmmDefsToImport = path;
  ui.lbLastLoadedBaseHMM->setText(QFileInfo(path).fileName());
  importBaseModelFromDirectory(QDir(QFileInfo(path).path()));
}


void SpeechModelSettings::loadBaseTiedlist()
{
  QString path = KFileDialog::getOpenFileName(KUrl(lastDirectory), QString(), this, i18n("Select base tiedlist"));
  if (path.isEmpty()) return;

  m_tiedlistToImport = path;
  ui.lbLastLoadedBaseTiedlist->setText(QFileInfo(path).fileName());
  importBaseModelFromDirectory(QDir(QFileInfo(path).path()));
}


void SpeechModelSettings::loadBaseMacros()
{
  QString path = KFileDialog::getOpenFileName(KUrl(lastDirectory), QString(), this, i18n("Select base macros"));
  if (path.isEmpty()) return;

  m_macrosToImport = path;
  ui.lbLastLoadedBaseMacros->setText(QFileInfo(path).fileName());
  importBaseModelFromDirectory(QDir(QFileInfo(path).path()));
}


void SpeechModelSettings::loadBaseStats()
{
  QString path = KFileDialog::getOpenFileName(KUrl(lastDirectory), QString(), this, i18n("Select base stats"));
  if (path.isEmpty()) return;

  m_statsToImport = path;
  ui.lbLastLoadedBaseStats->setText(QFileInfo(path).fileName());
  importBaseModelFromDirectory(QDir(QFileInfo(path).path()));
}


void SpeechModelSettings::loadLanguageProfile()
{
  QString path = KFileDialog::getOpenFileName(KUrl(lastDirectory), QString(), this, i18n("Select language profile"));
  if (path.isEmpty()) return;

  m_languageProfileToImport = path;

  emit changed(true);
  
  uiLanguageProfile.lbProfileName->setText(QFileInfo(path).fileName());
}



SpeechModelSettings::~SpeechModelSettings()
{
}
