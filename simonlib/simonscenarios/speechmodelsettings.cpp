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
#include <QMenu>

#include <KMessageBox>
#include <KAction>
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
  
  connect(ui.cbBaseModels, SIGNAL(currentIndexChanged(int)), this, SLOT(baseModelSelectionChanged()));
  connect(ui.cbAdapt, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));

  uiLanguageProfile.pbLoadLanguageProfile->setIcon(KIcon("document-open"));
  
  connect(uiLanguageProfile.pbLoadLanguageProfile, SIGNAL(clicked()), this, SLOT(loadLanguageProfile()));

  connect(ui.pbExport, SIGNAL(clicked()), this, SLOT(exportBaseModel()));

  QMenu *importMenu = new QMenu(this);
  QAction *ghnsImport = importMenu->addAction(KIcon("get-hot-new-stuff"), i18n("Download"));
  QAction *fileImport = importMenu->addAction(KIcon("document-import"), i18n("Import"));
  QAction *fileCreate = importMenu->addAction(KIcon("document-import"), i18n("Create from model files"));

  connect(ghnsImport, SIGNAL(triggered()), this, SLOT(getNewBaseModels()));
  connect(fileImport, SIGNAL(triggered()), this, SLOT(openBaseModel()));
  connect(fileCreate, SIGNAL(triggered()), this, SLOT(createBaseModel()));

  ui.pbImport->setMenu(importMenu);

  ui.pbImport->setIcon(KIcon("document-import"));
  ui.pbExport->setIcon(KIcon("document-export"));
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
    // build imported path
    QString fileName = QFileInfo(path).fileName();
    int i = 0;
    QString targetPath;
    while (QFile::exists(targetPath = KStandardDirs::locateLocal("appdata", "model/base/"+QString::number(i)+fileName)))
      ++i;

    if (!QFile::copy(path, targetPath)) {
      KMessageBox::sorry(this, i18n("Failed to import base model to Simon."));
      return;
    }
    addBaseModelToSelection(targetPath);
    ui.cbBaseModels->setCurrentIndex(ui.cbBaseModels->count() - 1);

    emit changed(true);
  }
}

void SpeechModelSettings::addBaseModelToSelection(const QString& path)
{
  ui.cbBaseModels->addItem(baseModelDescription(path), path);
}

void SpeechModelSettings::baseModelSelectionChanged()
{
  ui.cbAdapt->setVisible(ui.cbBaseModels->currentIndex() > 0);
  slotChanged();
}

void SpeechModelSettings::slotChanged()
{
  emit changed(true);
}

QString SpeechModelSettings::baseModelDescription(const QString& path)
{
  if (path.isEmpty())
    return baseModelDescription(QString(), QDateTime());
  
  QString name;
  QDateTime dateTime;
  KTar tar(path, "application/x-gzip");
  Model::parseContainer(tar, dateTime, name);
  return baseModelDescription(name, dateTime);
}

QString SpeechModelSettings::baseModelDescription(const QString& name, const QDateTime& dateTime)
{
  if (name.isNull())
    return i18n("Do not use a base model");

  return i18nc("%1 is model name, %2 is creation date", "%1 (Created: %2)", 
                      name, dateTime.toString(Qt::LocalDate));
}

void SpeechModelSettings::getNewBaseModels()
{
  //TODO
}

void SpeechModelSettings::setupBaseModelSelection()
{
  ui.cbBaseModels->clear();
  addBaseModelToSelection(QString());
  QString baseModelsBasePath = KStandardDirs::locateLocal("appdata", "model/base/");
  QDir baseModelsDir(baseModelsBasePath);
  foreach (const QString& path, baseModelsDir.entryList(QDir::Files|QDir::NoDotAndDotDot))
    addBaseModelToSelection(baseModelsBasePath + path);

  ui.cbBaseModels->setCurrentIndex(ui.cbBaseModels->findData(ScenarioManager::getInstance()->baseModel()));
  baseModelSelectionChanged();
}

void SpeechModelSettings::load()
{
   ui.cbAdapt->setChecked(ScenarioManager::getInstance()->baseModelType() == 1);
  setupBaseModelSelection();

  m_storedModelType = ScenarioManager::getInstance()->baseModelType();
  
  uiLanguageProfile.lbProfileName->setText(ScenarioManager::getInstance()->languageProfileName());
  KCModule::load();
  emit changed(false);
}

void SpeechModelSettings::save()
{
  QString selectedBaseModel = ui.cbBaseModels->itemData(ui.cbBaseModels->currentIndex()).toString();

  int modelType = 2;
  if (!selectedBaseModel.isNull()) {
    modelType = ui.cbAdapt->isChecked() ? 1 : 0;
  }

  if (ScenarioManager::getInstance()->baseModel() != selectedBaseModel) {
    QString targetPath = KStandardDirs::locateLocal("appdata", "model/basemodel.sbm");
    
    bool succ = true;
    if (QFile::exists(targetPath) && !QFile::remove(targetPath)) {
      KMessageBox::sorry(this, i18n("Could not remove current base model"));
      succ = false;
      return;
    }
    if (!QFile::copy(selectedBaseModel, targetPath)) {
      KMessageBox::sorry(this, i18n("Could not import hmm definitions."));
      succ = false;
    }

    if (succ)
      ScenarioManager::getInstance()->setBaseModel(selectedBaseModel);
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
  ScenarioManager::getInstance()->setBaseModel(QString());
  ScenarioManager::getInstance()->setBaseModelType(2);
  
  ScenarioManager::getInstance()->setLanguageProfileName(i18nc("Filename of a not yet selected language profile", "None"));

  QFile::remove(KStandardDirs::locateLocal("appdata", "model/basemodel.sbm"));
  QFile::remove(KStandardDirs::locateLocal("appdata", "model/languageProfile"));

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

