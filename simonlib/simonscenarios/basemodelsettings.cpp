/*
 *   Copyright (C) 2012 Peter Grasch <grasch@simon-listens.org>
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


#include "basemodelsettings.h"
#include "createbasemodel.h"
#include "model.h"
#include "speechmodelmanagementconfiguration.h"
#include "ui_modelsettingsdlg.h"
#include <simonscenarios/scenariomanager.h>
#include <QMenu>
#include <knewstuff3/downloaddialog.h>
#include <KTar>
#include <KMessageBox>
#include <KFileDialog>

BaseModelSettings::BaseModelSettings ( QWidget* parent, Qt::WFlags flags ) : QWidget ( parent, flags), ui(new Ui::ModelDlg())
{
  ui->setupUi(this);
  
  connect(ui->cbBaseModels, SIGNAL(currentIndexChanged(int)), this, SLOT(baseModelSelectionChanged()));
  connect(ui->cbAdapt, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
  
  ui->pbRemove->setIcon(KIcon("list-remove"));
  
  connect(ui->pbExport, SIGNAL(clicked()), this, SLOT(exportBaseModel()));

  QMenu *importMenu = new QMenu(this);
  QAction *ghnsImport = importMenu->addAction(KIcon("get-hot-new-stuff"), i18n("Download"));
  QAction *fileImport = importMenu->addAction(KIcon("document-import"), i18n("Import"));
  QAction *fileCreate = importMenu->addAction(KIcon("document-import"), i18n("Create from model files"));

  connect(ghnsImport, SIGNAL(triggered()), this, SLOT(getNewBaseModels()));
  connect(fileImport, SIGNAL(triggered()), this, SLOT(openBaseModel()));
  connect(fileCreate, SIGNAL(triggered()), this, SLOT(createBaseModel()));

  connect(ui->pbRemove, SIGNAL(clicked()), this, SLOT(removeBaseModel()));

  ui->pbImport->setMenu(importMenu);

  ui->pbImport->setIcon(KIcon("document-import"));
  ui->pbExport->setIcon(KIcon("document-export"));
}

void BaseModelSettings::setMinimal(bool minimal)
{
  ui->pbExport->setVisible(!minimal);
}

void BaseModelSettings::slotChanged()
{
  emit changed(true);
}

QString BaseModelSettings::baseModelDescription(const QString& path)
{
  if (path.isEmpty())
    return baseModelDescription(QString(), QDateTime());
  
  QString name;
  QDateTime dateTime;
  KTar tar(path, "application/x-gzip");
  Model::parseContainer(tar, dateTime, name);
  return baseModelDescription(name, dateTime);
}

QString BaseModelSettings::baseModelDescription(const QString& name, const QDateTime& dateTime)
{
  if (name.isNull())
    return i18n("Do not use a base model");

  return i18nc("%1 is model name, %2 is creation date", "%1 (Created: %2)", 
                      name, dateTime.toString(Qt::LocalDate));
}

void BaseModelSettings::getNewBaseModels()
{
  QPointer<KNS3::DownloadDialog> dialog = new KNS3::DownloadDialog(KStandardDirs::locate("config", "simonbasemodels.knsrc"));
  dialog->exec();

  if (!dialog) return;

  delete dialog;

  setupBaseModelSelection();
}

void BaseModelSettings::load()
{
  kDebug() << "Load";
  kDebug() << this;
  ui->cbAdapt->setChecked(ScenarioManager::getInstance()->baseModelType() == 1);
  setupBaseModelSelection();

  m_storedModelType = ScenarioManager::getInstance()->baseModelType();
}

void BaseModelSettings::setupBaseModelSelection()
{
  ui->cbBaseModels->clear();
  addBaseModelToSelection(QString());
  QString baseModelsBasePath = KStandardDirs::locateLocal("appdata", "model/base/");
  QDir baseModelsDir(baseModelsBasePath);
  foreach (const QString& path, baseModelsDir.entryList(QDir::Files|QDir::NoDotAndDotDot))
    addBaseModelToSelection(baseModelsBasePath + path);

  if (ScenarioManager::getInstance()->baseModelType() == 2)
    ui->cbBaseModels->setCurrentIndex(0); // no base model
  else
    ui->cbBaseModels->setCurrentIndex(qMax(0, ui->cbBaseModels->findData(ScenarioManager::getInstance()->baseModel())));
  baseModelSelectionChanged();
}

void BaseModelSettings::exportBaseModel()
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

void BaseModelSettings::createBaseModel()
{
  QPointer<CreateBaseModel> baseModelSettings(new CreateBaseModel(this));
  importBaseModel(baseModelSettings->buildModel());
  delete baseModelSettings;
}

void BaseModelSettings::openBaseModel()
{
  importBaseModel(KFileDialog::getOpenFileName(KUrl(), "*.sbm", this, i18n("Open Simon base model")));
}

void BaseModelSettings::removeBaseModel()
{
  QString baseModel = ui->cbBaseModels->itemData(ui->cbBaseModels->currentIndex()).toString();
  if (!QFile::exists(baseModel)) {
    kDebug() << "Doesn't exist: " << baseModel;
    return;
  }

  if (KMessageBox::questionYesNo(this, i18n("Do you really want to remove the selected base model?")) == KMessageBox::Yes) {
    if (!QFile::remove(baseModel)) {
      KMessageBox::sorry(this, i18n("Could not remove base model"));
    } else {
      setupBaseModelSelection();
      save();
    }
  }
}

void BaseModelSettings::importBaseModel ( const QString& path )
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
    ui->cbBaseModels->setCurrentIndex(ui->cbBaseModels->count() - 1);

    emit changed(true);
  }
}

void BaseModelSettings::addBaseModelToSelection(const QString& path)
{
  ui->cbBaseModels->addItem(baseModelDescription(path), path);
}

void BaseModelSettings::baseModelSelectionChanged()
{
  ui->cbAdapt->setVisible(ui->cbBaseModels->currentIndex() > 0);
  slotChanged();
}

void BaseModelSettings::save()
{

  QString selectedBaseModel = ui->cbBaseModels->itemData(ui->cbBaseModels->currentIndex()).toString();

  int modelType = 2;
  if (!selectedBaseModel.isNull()) {
    modelType = ui->cbAdapt->isChecked() ? 1 : 0;
    if (ScenarioManager::getInstance()->baseModel() != selectedBaseModel) {
      QString targetPath = KStandardDirs::locateLocal("appdata", "model/basemodel.sbm");
      
      bool succ = true;
      if (QFile::exists(targetPath) && !QFile::remove(targetPath)) {
        KMessageBox::sorry(this, i18n("Could not remove current base model"));
        succ = false;
        return;
      }
      if (!QFile::copy(selectedBaseModel, targetPath)) {
        KMessageBox::sorry(this, i18n("Could not import base model."));
        succ = false;
      }

      if (succ)
        ScenarioManager::getInstance()->setBaseModel(selectedBaseModel);
    }
  }

  ScenarioManager::getInstance()->setBaseModelType(modelType);
}

void BaseModelSettings::defaults()
{
  ScenarioManager::getInstance()->setBaseModel(QString());
  ScenarioManager::getInstance()->setBaseModelType(2);

  QFile::remove(KStandardDirs::locateLocal("appdata", "model/basemodel.sbm"));
  QFile::remove(KStandardDirs::locateLocal("appdata", "model/languageProfile"));

  load();
}

