/*
 *   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
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
#include <simonmodelmanagementui/promptsview.h>
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
#include <QQueue>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <KIcon>
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
  
  connect(ui.twScenarios, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(updateScenarioDisplays()));
  connect(ui.pbStartTraining, SIGNAL(clicked(bool)), this, SLOT(startTraining()));
  
  connect(ui.pbEditScenario, SIGNAL(clicked(bool)), this, SIGNAL(editScenario()));
  connect(ActionManager::getInstance(), SIGNAL(processedRecognitionResult(RecognitionResult,bool)), this, SLOT(processedRecognitionResult(RecognitionResult, bool)));
  
  connect(ui.pbManageSamples, SIGNAL(clicked()), this, SLOT(manageSamples()));

  displayScenarios();
  displayAcousticModelInfo();
  
  ui.pbAcousticConfiguration->setIcon(KIcon("view-pim-news"));
  ui.pbAudioConfiguration->setIcon(KIcon("preferences-desktop-sound"));
  ui.pbScenarioConfiguration->setIcon(KIcon("view-list-tree"));
  ui.pbEditScenario->setIcon(KIcon("document-edit"));
  ui.pbStartTraining->setIcon(KIcon("view-pim-news"));
  ui.pbManageSamples->setIcon(KIcon("view-list-tree"));

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
  ui.twScenarios->setLineWidth(24);
}

void WelcomePage::processedRecognitionResult(const RecognitionResult& recognitionResult, bool accepted )
{
  QString result = recognitionResult.sentence();
  if (!accepted)
    result = i18nc("%1 is the result string", "%1 (no applicable command)", result);
    
  ui.lbRecognition->setText(result);
}

QTreeWidgetItem* WelcomePage::findScenario(const QString& id) const
{
  QQueue<QTreeWidgetItem*> toCheck;
  toCheck.enqueue(ui.twScenarios->invisibleRootItem());
  do {
    QTreeWidgetItem* current = toCheck.dequeue();
    if (current->data(0, Qt::UserRole) == id) {
      return current;
    }
    for (int i = 0; i < current->childCount(); i++)
      toCheck.enqueue(current->child(i));
  } while (!toCheck.isEmpty());
  return 0;
}
void WelcomePage::trainingsTextSelected ( const QModelIndex& index )
{
  TrainingText *text = static_cast<TrainingText*>(index.internalPointer());

  QString scenarioId = text->parentScenarioId();
  ui.twScenarios->setCurrentItem(findScenario(scenarioId));
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
  if (ui.twScenarios->currentItem())
    return ui.twScenarios->currentItem()->data(0, Qt::UserRole).toString();
  
  return QString();
}

void WelcomePage::displayScenario(QTreeWidgetItem* parent, Scenario *s)
{
  QFont deactivatedFont = QFont();
  deactivatedFont.setItalic(true);
  QBrush deactivatedColor = KColorScheme(QPalette::Active).foreground(KColorScheme::InactiveText);

  QTreeWidgetItem *i = new QTreeWidgetItem(QStringList() << s->name());
  i->setIcon(0, s->icon());
  i->setData(0, Qt::UserRole, s->id());

  if (!s->isActive()) {
    i->setFont(0, deactivatedFont);
    i->setForeground(0, deactivatedColor);
  }

  parent->addChild(i);
  foreach (Scenario *sub, s->childScenarios())
    displayScenario(i, sub);
}

void WelcomePage::displayScenarios()
{
  setUpdatesEnabled(false);
  ui.twScenarios->blockSignals(true);
  
  QString currentData = getCurrentlySelectedScenarioId();
  
  ui.twScenarios->clear();

  QList<Scenario*> scenarioList = ScenarioManager::getInstance()->getScenarios();
  QSet<Scenario*> topLevelScenarios = QSet<Scenario*>::fromList(scenarioList);
  foreach (Scenario* s, scenarioList)
    foreach (Scenario* sub, s->childScenarios())
      topLevelScenarios.remove(sub);

  foreach (Scenario* s, scenarioList)
    if (topLevelScenarios.contains(s))
      displayScenario(ui.twScenarios->invisibleRootItem(), s);
  
  ui.twScenarios->setCurrentItem(findScenario(currentData));
  
  ui.twScenarios->blockSignals(false);
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
  QTreeWidgetItem *item = findScenario(scenario->id());
  ui.twScenarios->setCurrentItem(item);
  
  ui.pbEditScenario->setText(i18nc("%1 is the scenario to change", "Open \"%1\"", scenario->name()));
}

void WelcomePage::updateTrainingsTexts()
{
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

void WelcomePage::manageSamples()
{
  QPointer<PromptsView> p(new PromptsView(this));
  p->exec();
  delete p;
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
  ScenarioManagementDialog::configureScenarios(this);
}
