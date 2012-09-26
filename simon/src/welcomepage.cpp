/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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


#include "welcomepage.h"
#include <simonmodelmanagementui/TrainSamples/trainingswizard.h>
#include <simonrecognitioncontrol/recognitioncontrol.h>
#include "trainingtextaggregatormodel.h"
#include "version.h"

#include <simonscenarioui/scenariomanagementdialog.h>
#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/modelmanager.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/model.h>
#include <simonscenarios/trainingtext.h>
#include <simonactions/actionmanager.h>
#include <simonsound/volumewidget.h>

#include <QMetaObject>
#include <QVBoxLayout>
#include <KDebug>
#include <KLocalizedString>
#include <KCMultiDialog>
#include <KColorScheme>
#include <KMessageBox>
#include <KStandardDirs>

WelcomePage::WelcomePage(QAction *activationAction, QWidget* parent) : QWidget(parent),
  volumeWidget(new VolumeWidget(this, SoundClient::Background)),
  trainingTextModel(new TrainingTextAggregatorModel(this))
{
  ui.setupUi(this);
  static_cast<QVBoxLayout*>(ui.gbRecognition->layout())->insertWidget(1, volumeWidget);
  
  volumeWidget->enablePrompt(false);
  volumeWidget->init();

  connect(ScenarioManager::getInstance(), SIGNAL(scenarioSelectionChanged()), this, SLOT(displayScenarios()));
  connect(ScenarioManager::getInstance(), SIGNAL(deactivatedScenarioListChanged()), this, SLOT(displayScenarios()));
  
  connect(ui.pbScenarioConfiguration, SIGNAL(clicked()), this, SLOT(scenarioConfig()));
  connect(ui.pbAcousticConfiguration, SIGNAL(clicked()), this, SLOT(baseModelConfig()));
  connect(ui.pbAudioConfiguration, SIGNAL(clicked()), this, SLOT(audioConfig()));
  
  connect(ui.lwScenarios, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(updateScenarioDisplays()));
  connect(ui.pbStartTraining, SIGNAL(clicked(bool)), this, SLOT(startTraining()));
  
  connect(ui.pbEditScenario, SIGNAL(clicked(bool)), this, SIGNAL(editScenario()));
  connect(ActionManager::getInstance(), SIGNAL(processedRecognitionResult(RecognitionResult,bool)), this, SLOT(processedRecognitionResult(RecognitionResult, bool)));
  
  displayScenarios();
  displayAcousticModelInfo();
  
  ui.pbAcousticConfiguration->setIcon(KIcon("view-pim-news"));
  ui.pbAudioConfiguration->setIcon(KIcon("preferences-desktop-sound"));
  ui.pbScenarioConfiguration->setIcon(KIcon("view-list-tree"));
  ui.pbEditScenario->setIcon(KIcon("document-edit"));
  ui.pbStartTraining->setIcon(KIcon("view-pim-news"));

  const QString stylesheet("QGroupBox { background-image: url(\"%1\"); \
                            background-repeat: no-repeat; \
                            background-attachment: fixed; \
                            background-position: left top; \
                            font-weight: bold; }");
  ui.gbScenarios->setStyleSheet( stylesheet.arg( KStandardDirs::locate( "data", "simon/images/bg-scenarios.png" ) ));
  ui.gbTraining->setStyleSheet( stylesheet.arg( KStandardDirs::locate( "data", "simon/images/bg-training.png" ) ));
  ui.gbAcousticModel->setStyleSheet( stylesheet.arg( KStandardDirs::locate( "data", "simon/images/bg-acoustic-model.png" ) ));
  ui.gbRecognition->setStyleSheet( stylesheet.arg( KStandardDirs::locate( "data", "simon/images/bg-recognition.png" ) ));
  
  connect(RecognitionControl::getInstance(), SIGNAL(synchroniationCompleted()), this, SLOT(displayAcousticModelInfo()));
  connect(ui.tvTrainingTexts, SIGNAL(activated(QModelIndex)), this, SLOT(trainingsTextSelected(QModelIndex)));
  
  ui.pbActivation->setAction(activationAction);
  ui.tvTrainingTexts->setModel(trainingTextModel);
  ui.tvTrainingTexts->verticalHeader()->hide();
  ui.lwScenarios->setLineWidth(24);
}

void WelcomePage::processedRecognitionResult(const RecognitionResult& recognitionResult, bool accepted )
{
  QString result = recognitionResult.sentence();
  if (!accepted)
    result = i18nc("%1 is the result string", "%1 (no applicable command)", result);
    
  ui.lbRecognition->setText(result);
}

void WelcomePage::trainingsTextSelected ( const QModelIndex& index )
{
  TrainingText *text = static_cast<TrainingText*>(index.internalPointer());
  QString scenarioId = text->parentScenarioId();
  for (int i = 0; i < ui.lwScenarios->count(); i++) {
    if (ui.lwScenarios->item(i)->data(Qt::UserRole) == scenarioId) {
      kDebug() << "Selecting " << scenarioId;
      ui.lwScenarios->setCurrentRow(i);
      return;
    }
  }
}

void WelcomePage::displayAcousticModelInfo()
{
  QString description;
  Model* baseModel = ModelManager::getInstance()->createBaseModelContainer();
  if (baseModel) {
    switch (baseModel->baseModelType()) {
      case 2:
        description = i18n("You are using a user generated acoustic model.\nRecognition rate can be improved with training.");
        break;
      case 1:
        description = i18n("You are using an adapted base model.\nRecognition rate depends on the base model but can be improved with training.");
        break;
      case 0:
        description = i18n("You are using a static base model.\nRecognition rate depends solely on the base model.");
        break;
    }
    if (!baseModel->container().isNull()) {
      //we have a base model
      description += "\n\n";
      description += i18nc("%1 is model name, %2 is creation date", "Base model: %1 (Created: %2)", 
                          baseModel->modelName(), baseModel->modelCreationDate().toString(Qt::LocalDate));
    }
    delete baseModel;
  }
  Model *activeModel = ModelManager::getInstance()->createActiveContainer();
  if (activeModel) {
    if (!activeModel->container().isNull()) {
      //we have an active model
      description += "\n\n";
      description += i18nc("%1 is model name, %2 is creation date", "Active model: %1 (Created: %2)", 
                          activeModel->modelName(), activeModel->modelCreationDate().toString(Qt::LocalDate));
    }
    delete activeModel;
  }
  
  ui.lbBaseModelDescription->setText(description);
}

QString WelcomePage::getCurrentlySelectedScenarioId()
{
  if (ui.lwScenarios->currentItem())
    return ui.lwScenarios->currentItem()->data(Qt::UserRole).toString();
  
  return QString();
}

void WelcomePage::displayScenarios()
{
  setUpdatesEnabled(false);
  ui.lwScenarios->blockSignals(true);
  
  QString currentData = getCurrentlySelectedScenarioId();
  
  ui.lwScenarios->clear();

  QFont deactivatedFont = QFont();
  deactivatedFont.setItalic(true);
  QBrush deactivatedColor = KColorScheme(QPalette::Active).foreground(KColorScheme::InactiveText);

  QListWidgetItem *toSelect = 0;
  QList<Scenario*> scenarioList = ScenarioManager::getInstance()->getScenarios();
  foreach (Scenario* s, scenarioList) {
    QListWidgetItem *i = new QListWidgetItem(s->icon(), s->name());
    i->setData(Qt::UserRole, s->id());
    
    if (!s->isActive()) {
      i->setFont(deactivatedFont);
      i->setForeground(deactivatedColor);
    }
    ui.lwScenarios->addItem(i);
    
    if (s->id() == currentData)
      toSelect = i;
  }
  
  if (toSelect)
    ui.lwScenarios->setCurrentItem(toSelect);
  
  ui.lwScenarios->blockSignals(false);
  setUpdatesEnabled(true);
  updateTrainingsTexts();
}

void WelcomePage::updateScenarioDisplays()
{
  QString currentId = getCurrentlySelectedScenarioId();
  Scenario *scenario = ScenarioManager::getInstance()->getScenario(currentId);

  if (!scenario) {
    KMessageBox::error(this, i18nc("%1 is scenario id", "Could not retrieve Scenario \"%1\"", currentId));
    return;
  }
  ScenarioManager::getInstance()->updateDisplays(scenario, true);
}

void WelcomePage::displayScenarioPrivate ( Scenario* scenario )
{
  QListWidgetItem *item;
  int currentRow = 0;
  while ((item = ui.lwScenarios->item(currentRow++)) != 0) {
    if (item->data(Qt::UserRole) == scenario->id()) {
      ui.lwScenarios->setCurrentItem(item);
      break;
    }
  }
  
  ui.pbEditScenario->setText(i18nc("%1 is the scenario to change", "Open \"%1\"", scenario->name()));
}

void WelcomePage::updateTrainingsTexts()
{
  kDebug() << "=====================";
  QList<TrainingTextCollection*> collections;
  foreach (Scenario *s, ScenarioManager::getInstance()->getScenarios()) {
    kDebug() << "Trainings texts from: " << s;
    collections << s->texts();
  }
  trainingTextModel->setCollections(collections);
}

void WelcomePage::startTraining()
{
  QModelIndex selectedIndex = ui.tvTrainingTexts->currentIndex();
  if (!selectedIndex.isValid()) {
    KMessageBox::information(this, i18n("Please select a text first.\n\nIf there are no texts available, "
                                        "then none of your loaded Scenarios provide training texts.\n\nTo add a "
                                        "new text, select the scenario to modify on the left, open it and add "
                                        "the training text in the \"Training\" menu."));
    return;
  }
  
  TrainingText* text = static_cast<TrainingText*>(selectedIndex.internalPointer());
  if (!text)
    return;

  QPointer<TrainingsWizard> wizard = new TrainingsWizard(this);

  if (wizard->init(*text))
    wizard->exec();

  delete wizard;
}

void WelcomePage::showEvent(QShowEvent* event)
{
  QWidget::showEvent(event);
  //start recording after the event handling
  QMetaObject::invokeMethod(volumeWidget, "start", Qt::QueuedConnection);
}

void WelcomePage::hideEvent(QHideEvent* event)
{
  QWidget::hideEvent(event);
  //stop recording after the event handling
  QMetaObject::invokeMethod(volumeWidget, "stop", Qt::QueuedConnection);
}

void WelcomePage::audioConfig()
{
  KCMultiDialog configDialog(this);
  configDialog.addModule("simonsoundconfig");
  configDialog.exec();
}

void WelcomePage::baseModelConfig()
{
  KCMultiDialog configDialog(this);
  configDialog.addModule("simonspeechmodelmanagementconfig");
  configDialog.exec();
}

void WelcomePage::scenarioConfig()
{
  ScenarioManagementDialog *dlg = new ScenarioManagementDialog("simon/", this);
  if (dlg->updateScenarioConfiguration())
  {
    //reload scenario information
    kDebug() << "Reloading Scenario Information";

    if (!ScenarioManager::getInstance()->setupScenarios(true /* force change */))
      KMessageBox::sorry(this, i18n("Could not re-initialize scenarios. Please restart simon!"));

    displayScenarios();
  }
  dlg->deleteLater();
}
