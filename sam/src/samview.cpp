/*
 *  Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "samview.h"
#include "samxmlhelper.h"
#include "accuracydisplay.h"
#include "conservativetraining.h"
#include "exporttestresults.h"
#include "juliustestconfigurationwidget.h"
#include "sphinxtestconfigurationwidget.h"
#include "testresultwidget.h"
#include "testresultplotter.h"
#include "reportparameters.h"
#include "carraydata.h"
#include "qwt_bars_item.h"
#include "qwt_series_data.h"
#include "corpusinformation.h"

#include <speechmodelcompilation/modelcompilerhtk.h>
#include <speechmodelcompilation/modelcompilersphinx.h>
#include <speechmodelcompilation/modelcompilationadapterhtk.h>
#include <speechmodelcompilation/modelcompilationadaptersphinx.h>
#include <simonscenarioui/scenariomanagementdialog.h>

#include <qwt_legend.h>
#include <qwt_legend_item.h>

#include <QHash>
#include <QThread>
#include <QPointer>
#include <KStandardAction>
#include <KAction>
#include <KActionCollection>
#include <KIcon>
#include <KStandardDirs>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KCMultiDialog>
#include <KMessageBox>
#include <KFileDialog>
#include <KGlobal>
#include <KInputDialog>
#include <KLocale>
#include <KDebug>
#include <KCmdLineArgs>
#include <QDomDocument>
#include <QUuid>

SamView::SamView(QWidget *parent, Qt::WFlags flags) : KXmlGuiWindow(parent, flags),
  m_startCompilationAfterAdaption(false),
  m_startTestAfterCompile(false),
  m_startTestAfterAdaption(false),
  m_exportAfterTest(false),
  m_dirty(false),
  m_creationCorpus(0),
  m_reportParameters(0),
  modelCompiler(0),
  modelCompilationAdapter(0),
  barGraph(0)
{
  KGlobal::locale()->insertCatalog("simonlib");
  ui.setupUi(this);
  ui.qpPlot->hide();

  barGraphLegend = new QwtLegend(ui.qpPlot);
  ui.qpPlot->insertLegend(barGraphLegend);

  initGraph();

  ui.saTestConfigurations->setWidget(ui.wgTestConfigurations);

  KAction* getPathsFromSimon = new KAction(this);
  getPathsFromSimon->setText(i18n("Modify simon's model"));
  getPathsFromSimon->setStatusTip(i18n("Manage simon's current model with ssc"));
  getPathsFromSimon->setIcon(KIcon("simon"));
  actionCollection()->addAction("getPathsFromSimon", getPathsFromSimon);
  connect(getPathsFromSimon, SIGNAL(triggered(bool)),
          this, SLOT(getBuildPathsFromSimon()));

  KAction* recompile = new KAction(this);
  recompile->setText(i18n("Build model"));
  recompile->setStatusTip(i18n("Build the currently open model."));
  recompile->setIcon(KIcon("view-refresh"));
  recompile->setShortcut(Qt::CTRL + Qt::Key_F5);
  actionCollection()->addAction("compileModel", recompile);
  connect(recompile, SIGNAL(triggered(bool)),
          this, SLOT(compileModel()));

  KAction* test= new KAction(this);
  test->setText(i18n("Test model"));
  test->setStatusTip(i18n("Test the model."));
  test->setIcon(KIcon("chronometer"));
  actionCollection()->addAction("testModel", test);
  connect(test, SIGNAL(triggered(bool)),
          this, SLOT(testModel()));

  KAction* testResults= new KAction(this);
  testResults->setText(i18n("Test results"));
  testResults->setStatusTip(i18n("Display the test results."));
  testResults->setIcon(KIcon("view-pim-tasks"));
  actionCollection()->addAction("testResults", testResults);
  connect(testResults, SIGNAL(triggered(bool)),
          this, SLOT(switchToTestResults()));

  KAction* exportTestResults = new KAction(this);
  exportTestResults->setText(i18n("Export test result"));
  exportTestResults->setStatusTip(i18n("Export the test results to a file."));
  exportTestResults->setIcon(KIcon("document-export"));
  actionCollection()->addAction("exportTestResults", exportTestResults);
  connect(exportTestResults, SIGNAL(triggered(bool)),
          this, SLOT(exportTestResults()));

  backendType = TestConfigurationWidget::SPHINX;
  m_User = "internalsamuser";


  KStandardAction::openNew(this, SLOT(newProject()), actionCollection());
  KStandardAction::save(this, SLOT(save()), actionCollection());
  KStandardAction::saveAs(this, SLOT(saveAs()), actionCollection());
  KStandardAction::open(this, SLOT(load()), actionCollection());
  KStandardAction::preferences(this, SLOT(showConfig()), actionCollection());
  KStandardAction::quit(this, SLOT(close()), actionCollection());

  setupGUI();

  connect(ui.cbType, SIGNAL(currentIndexChanged(int)), this, SLOT(backendChanged(int)));

  connect(ui.pbCompileModel, SIGNAL(clicked()), this, SLOT(compileModel()));
  connect(ui.pbTestModel, SIGNAL(clicked()), this, SLOT(testModel()));


  connect(ui.pbImportRecognitionSamples, SIGNAL(clicked()), this, SLOT(importRecognitionSamples()));
  connect(ui.rbDynamicModel, SIGNAL(toggled(bool)), this, SLOT(setDirty()));
  connect(ui.rbStaticModel, SIGNAL(toggled(bool)), this, SLOT(setDirty()));

  connect(ui.urOutputModel, SIGNAL(textChanged(QString)), this, SLOT(setDirty()));
  connect(ui.urPromptsBasePath, SIGNAL(textChanged(QString)), this, SLOT(setDirty()));
  connect(ui.urLexicon, SIGNAL(textChanged(QString)), this, SLOT(setDirty()));
  connect(ui.urGrammar, SIGNAL(textChanged(QString)), this, SLOT(setDirty()));
  connect(ui.urVocabulary, SIGNAL(textChanged(QString)), this, SLOT(setDirty()));
  connect(ui.urPrompts, SIGNAL(textChanged(QString)), this, SLOT(setDirty()));
  connect(ui.urBaseModel, SIGNAL(textChanged(QString)), this, SLOT(setDirty()));
  connect(ui.leScriptPrefix, SIGNAL(textChanged(QString)), this, SLOT(setDirty()));
  connect(ui.sbSampleRate, SIGNAL(valueChanged(int)), this, SLOT(setDirty()));

  ui.urOutputModel->setMode(KFile::File|KFile::LocalOnly);
  ui.urPromptsBasePath->setMode(KFile::Directory|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urLexicon->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urGrammar->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urVocabulary->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urPrompts->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urBaseModel->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);

  if(backendType == TestConfigurationWidget::SPHINX)
    backendChanged(0);
  else
    if(backendType == TestConfigurationWidget::JHTK)
      backendChanged(1);

  connect(ui.pbAddTestConfiguration, SIGNAL(clicked()), this, SLOT(addTestConfiguration()));

  connect(ui.pbSerializeScenarios, SIGNAL(clicked()), this, SLOT(serializeScenarios()));
  connect(ui.pbSerializePrompts, SIGNAL(clicked()), this, SLOT(serializePrompts()));

  connect(ui.pbCancelBuildModel, SIGNAL(clicked()), this, SLOT(abortModelCompilation()));
  connect(ui.pbCancelTestModel, SIGNAL(clicked()), this, SLOT(abortModelTest()));
  connect(ui.pbSaveCompleteBuildlog, SIGNAL(clicked()), this, SLOT(storeBuildLog()));

  if (KCmdLineArgs::parsedArgs()->count() > 0)
  {
    QString loadPath = KCmdLineArgs::parsedArgs()->url(0).toLocalFile();
    kDebug() << "Load path: " << loadPath;

    if (!loadPath.isEmpty())
      load(loadPath);
  }

  bool autoTestModel = KCmdLineArgs::parsedArgs()->isSet("t");
  if (KCmdLineArgs::parsedArgs()->isSet("s"))
  {
    getBuildPathsFromSimon();
    //async
    if (KCmdLineArgs::parsedArgs()->isSet("c"))
    {
      m_startCompilationAfterAdaption = true;
      if (autoTestModel) {
        m_startTestAfterCompile = true;
      }
    } else
      if (autoTestModel) {
        m_startTestAfterAdaption = true;
      }
  } else {
    if (KCmdLineArgs::parsedArgs()->isSet("c"))
    {
      compileModel();
      if (autoTestModel)
        m_startTestAfterCompile = true;
    }
    else if (autoTestModel)
      testModel();
  }
  if (KCmdLineArgs::parsedArgs()->isSet("e"))
    m_exportAfterTest = true;
}

bool SamView::askIfQuit()
{
  if (batchMode())
    return true;

  if (m_dirty)
  {
    int ret = KMessageBox::questionYesNoCancel(this, i18n("Your sam configuration has changed.\n\nDo you want to save?"));
    switch (ret)
    {
      case KMessageBox::Yes:
        save();
        return true;
      case KMessageBox::Cancel:
        return false;
      default:
        return true;
    }
  }
  return true;
}

bool SamView::close()
{
  kDebug() << "Closing";
  if (batchMode())
  {
    if (KCmdLineArgs::parsedArgs()->isSet("w"))
      save();
    return QWidget::close();
  }

  if (askIfQuit())
    return QWidget::close();

  return false;
}

void SamView::storeBuildLog()
{
  if (!modelCompiler->hasBuildLog())
  {
    error(i18n("No current log."));
    return;
  }

  QString buildLog = modelCompiler->getGraphicBuildLog();

  QString filename;
  if (KCmdLineArgs::parsedArgs()->isSet("l"))
    filename = KCmdLineArgs::parsedArgs()->getOption("l");
  else
    filename = KFileDialog::getSaveFileName(KUrl(), i18n("HTML files *.html"), this);
  if (filename.isEmpty()) return;
  QFile f(filename);
  if (!f.open(QIODevice::WriteOnly))
  {
    fatalError(i18nc("%1 is the file name", "Could not open output file %1.", filename));
    return;
  }
  f.write(buildLog.toUtf8());
}

void SamView::addTestConfiguration()
{
  TestConfigurationWidget *config(0) ;
  if(TestConfigurationWidget::IntToBackendType(ui.cbType->currentIndex()) == TestConfigurationWidget::SPHINX)
    config = new SphinxTestConfigurationWidget(this);
  else if(TestConfigurationWidget::IntToBackendType(ui.cbType->currentIndex()) == TestConfigurationWidget::JHTK)
    config = new JuliusTestConfigurationWidget(this);

  addTestConfiguration(config);
}

void SamView::addTestConfiguration(TestConfigurationWidget* config)
{
  connect(config, SIGNAL(destroyed()), this, SLOT(testConfigurationRemoved()));
  connect(config, SIGNAL(tagChanged()), this, SLOT(testConfigTagChanged()));
  connect(config, SIGNAL(changed()), this, SLOT(setDirty()));
  testConfigurations << config;
  ui.vlTestConfigurations->addWidget(config);

  TestResultWidget *result = new TestResultWidget(config, this);
  connect(result, SIGNAL(testComplete()), this, SLOT(subTestComplete()));
  connect(result, SIGNAL(testAborted()), this, SLOT(subTestAborted()));
  connect(result, SIGNAL(testStarted()), this, SLOT(subTestStarted()));
  connect(result, SIGNAL(destroyed()), this, SLOT(testResultRemoved()));
  testResults << result;
  ui.twTestResults->addTab(result, config->tag());
  setDirty();
}

void SamView::testConfigTagChanged()
{
  TestConfigurationWidget *configurationWidget = static_cast<TestConfigurationWidget*>(sender());
  for (int i=1; i < ui.twTestResults->count(); i++)
  {
    if (static_cast<TestResultWidget*>(ui.twTestResults->widget(i))->getConfiguration() == configurationWidget)
    {
      ui.twTestResults->setTabText(i, configurationWidget->tag());
      break;
    }
  }
}

void SamView::displayModelTestStatus()
{
  int running = 0;
  int idle = 0;
  int done = 0;
  foreach (TestResultWidget* test, testResults)
  {
    switch (test->getState())
    {
      case TestResultWidget::Idle:
        idle++;
        break;
      case TestResultWidget::Waiting:
      case TestResultWidget::Running:
        running++;
        break;
      case TestResultWidget::Done:
        done++;
        break;
    }
  }

  if (done == testResults.count())
  {
    ui.pbTestProgress->setValue(1);
    ui.pbTestProgress->setMaximum(1);
    allTestsFinished();
  } else if (idle == testResults.count())
  {
    ui.pbTestProgress->setValue(0);
    ui.pbTestProgress->setMaximum(1);
  } else {
    if (ui.pbTestProgress->maximum() != 0)
    {
      ui.pbTestProgress->setValue(0);
      ui.pbTestProgress->setMaximum(0);
    }
  }
}

void SamView::subTestStarted()
{
  kDebug() << "Subtest started...";
  ui.teTestLog->append(i18nc("Tag of the test", "Test started: %1",
                             static_cast<TestResultWidget*>(sender())->getTag()));

  displayModelTestStatus();
}

void SamView::subTestAborted()
{
  kDebug() << "Subtest aborted...";
  ui.teTestLog->append(i18nc("Tag of the test", "Test aborted: %1",
                             static_cast<TestResultWidget*>(sender())->getTag()));
  displayModelTestStatus();
}

void SamView::subTestComplete()
{
  kDebug() << "Subtest completed...";
  ui.teTestLog->append(i18nc("Tag of the test", "Test completed: %1",
                             static_cast<TestResultWidget*>(sender())->getTag()));
  displayModelTestStatus();
  startNextScheduledTest();
}

void SamView::startNextScheduledTest()
{
  foreach (TestResultWidget *test, testResults)
  {
    if (test->getState() == TestResultWidget::Waiting)
    {
      test->startTest();
      break;
    }
  }
}

void SamView::allTestsFinished()
{
  //fill general tab with information
  ui.lbTestResultInformation->setText(i18np("Ran 1 test.", "Ran %1 tests.", testResults.count()));

  ui.qpPlot->show();

  TestResultPlotter::plot(testResults, ui.qpPlot, barGraph, barGraphLegend);

  switchToTestResults();

  if (m_exportAfterTest)
  {
    m_exportAfterTest = false;
    exportTestResults();
  } else if (batchMode())
    exit(0);
}

void SamView::testConfigurationRemoved()
{
  testConfigurations.removeAll(static_cast<TestConfigurationWidget*>(sender()));
  setDirty();
}

void SamView::testResultRemoved()
{
  testResults.removeAll(static_cast<TestResultWidget*>(sender()));
}

void SamView::exportTestResults()
{
  ExportTestResults *e = new ExportTestResults(this);

  if (e->exportTestResults(m_reportParameters, creationCorpusInformation(), testResults, modelCompiler))
  {
    ReportParameters *temp = m_reportParameters;
    m_reportParameters = e->getReportParameters();

    if (temp && *temp != *m_reportParameters)
      setDirty();

    delete temp;

    //sync displays to potentially changed test result tags
//    foreach (TestConfigurationWidget *t, testConfigurations)
//      t->retrieveTag();//WARNING: why it can be changed?
  }
  delete e;

  if (batchMode())
    exit(0);
}

void SamView::backendChanged(int id)
{
  if(modelCompilationAdapter)
    delete modelCompilationAdapter;
  if(modelCompiler)
    delete modelCompiler;

  if(id == 0)
  {
    backendType = TestConfigurationWidget::SPHINX;
    modelCompilationAdapter = new ModelCompilationAdapterSPHINX(m_User, this);
    modelCompiler = new ModelCompilerSPHINX(m_User, this);
  } else if(id == 1)
  {
    backendType = TestConfigurationWidget::JHTK;
    modelCompilationAdapter = new ModelCompilationAdapterHTK(m_User, this);
    modelCompiler = new ModelCompilerHTK(m_User, this);
  }

  connect(modelCompilationAdapter, SIGNAL(adaptionComplete()), this, SLOT(slotModelAdaptionComplete()));
  connect(modelCompilationAdapter, SIGNAL(adaptionAborted()), this, SLOT(slotModelAdaptionAborted()));
  connect(modelCompilationAdapter, SIGNAL(status(QString,int,int)), this, SLOT(slotModelAdaptionStatus(QString,int,int)));
  connect(modelCompilationAdapter, SIGNAL(error(QString)), this, SLOT(slotModelAdaptionError(QString)));

  connect(modelCompiler, SIGNAL(modelCompiled()), this, SLOT(slotModelCompilationFinished()));
  connect(modelCompiler, SIGNAL(activeModelCompilationAborted()), this, SLOT(retrieveCompleteBuildLog()));
  connect(modelCompiler, SIGNAL(status(QString,int,int)), this, SLOT(slotModelCompilationStatus(QString,int,int)));
  connect(modelCompiler, SIGNAL(error(QString)), this, SLOT(slotModelCompilationError(QString)));

  connect(modelCompiler, SIGNAL(classUndefined(QString)), this,
          SLOT(slotModelCompilationClassUndefined(QString)));
  connect(modelCompiler, SIGNAL(wordUndefined(QString)), this,
          SLOT(slotModelCompilationWordUndefined(QString)));
  connect(modelCompiler, SIGNAL(phonemeUndefined(QString)), this,
          SLOT(slotModelCompilationPhonemeUndefined(QString)));
}

QList<CorpusInformation*> SamView::creationCorpusInformation()
{
  if (!m_creationCorpus)
    m_creationCorpus = createEmptyCorpusInformation();

  return QList<CorpusInformation*>() << m_creationCorpus;
}

void SamView::showConfig()
{
  QPointer<KCMultiDialog> configDialog = new KCMultiDialog(this);
  configDialog->addModule("simonsoundconfig", QStringList() << "");
  configDialog->addModule("simonmodelcompilationconfig", QStringList() << "");
  configDialog->exec();
  delete configDialog;
}


void SamView::newProject()
{
  if (!askIfQuit())
    return;
  clearCurrentConfiguration();

  ui.urOutputModel->clear();
  ui.urPromptsBasePath->clear();
  ui.urLexicon->clear();
  ui.urGrammar->clear();
  ui.urVocabulary->clear();
  ui.urPrompts->clear();
  qDeleteAll(testConfigurations); // will be removed automatically through signal magic
  ui.sbSampleRate->setValue(16000 /* default*/);
  ui.rbDynamicModel->click();
  ui.urBaseModel->clear();

  ui.twMain->setCurrentIndex(0);
  clearBuildLog();
  clearTest();
  ui.teAdaptLog->clear();
}


void SamView::load()
{
  QString filename = KFileDialog::getOpenFileName(KUrl(), i18n("sam projects *.sam"), this);
  if (filename.isEmpty()) return;

  load(filename);
}

void SamView::load(const QString& filename)
{
  if (!askIfQuit()) return;

  clearCurrentConfiguration();
  m_filename = filename;
  qDeleteAll(testConfigurations); //cleared by signal
  parseFile();
}

void SamView::save()
{
  if (m_filename.isEmpty()) {
    saveAs();
    return;
  }

  storeFile();
}


void SamView::saveAs()
{
  if (batchMode()) return;

  QString filename = KFileDialog::getSaveFileName(KUrl(), i18n("sam projects *.sam"), this);
  if (filename.isEmpty())
    return;

  m_filename = filename;
  storeFile();
}


void SamView::updateWindowTitle()
{
  QString decoFile = m_filename;
  if (m_filename.isEmpty())
    decoFile = i18n("Untitled");

  setWindowTitle(i18nc("%1 is file name", "sam - %1 [*]", decoFile));
  setWindowModified(m_dirty);
}


void SamView::parseFile()
{
  //read from m_filename
  QFile f(m_filename);
  if (!f.open(QIODevice::ReadOnly)) {
    fatalError(i18nc("%1 is file name", "Cannot open file: %1", m_filename));
  }

  QDomDocument doc;
  doc.setContent(f.readAll());

  QDomElement samProjectElem = doc.documentElement();
  if (samProjectElem.tagName() != "samProject")
  {
    fatalError(i18n("Corrupt or outdated sam configuration file."));
    return;
  }

  QDomElement creationElem = samProjectElem.firstChildElement("creation");
  m_creationCorpus = CorpusInformation::deSerialize(creationElem.firstChildElement("corpus"));

  ui.urOutputModel->setUrl(KUrl( SamXMLHelper::getText(creationElem, "outputModel") ));
  ui.urPrompts->setUrl(KUrl( SamXMLHelper::getText(creationElem, "prompts") ));
  ui.urPromptsBasePath->setUrl(KUrl( SamXMLHelper::getText(creationElem, "promptsBase") ));
  ui.urLexicon->setUrl(KUrl( SamXMLHelper::getText(creationElem, "lexicon") ));
  ui.urGrammar->setUrl(KUrl( SamXMLHelper::getText(creationElem, "grammar") ));
  ui.urVocabulary->setUrl(KUrl( SamXMLHelper::getText(creationElem, "vocabulary") ));
  ui.urDir->setUrl(KUrl(SamXMLHelper::getText(creationElem, "sphinxDict")));
  ui.sbSampleRate->setValue( SamXMLHelper::getInt(creationElem, "sampleRate") );

  ui.leScriptPrefix->setText(SamXMLHelper::getText(creationElem, "scriptPrefix"));
  ui.leMName->setText(SamXMLHelper::getText(creationElem, "sphinxModelName"));

  backendChanged(SamXMLHelper::getInt(creationElem, "backendType"));

  int modelType = SamXMLHelper::getInt(creationElem, "modelType");
  switch (modelType) {
    case 0: ui.rbStaticModel->click();
      break;
    case 1: ui.rbAdaptedBaseModel->click();
      break;
    case 2: ui.rbDynamicModel->click();
      break;
  }
  ui.urBaseModel->setUrl(KUrl(SamXMLHelper::getText(creationElem, "baseModel")));

  QDomElement testConfigurations = samProjectElem.firstChildElement("testConfigurations");
  QDomElement testConfiguration = testConfigurations.firstChildElement("testConfiguration");
  while (!testConfiguration.isNull())
  {
    addTestConfiguration(TestConfigurationWidget::deSerialize(testConfiguration));
    testConfiguration = testConfiguration.nextSiblingElement("testConfiguration");
  }

  QDomElement reportParametersElem = samProjectElem.firstChildElement("reportParameters");
  if (!reportParametersElem.isNull())
    m_reportParameters = ReportParameters::deSerialize(reportParametersElem);
  else
    m_reportParameters = 0;

  ui.twMain->setCurrentIndex(0);
  ui.teBuildLog->clear();
  ui.teAdaptLog->clear();
  updateWindowTitle();
  setClean();
}

int SamView::getModelType()
{
  if (ui.rbStaticModel->isChecked())
    return 0;
  else if (ui.rbAdaptedBaseModel->isChecked())
    return 1;
  return 2;
}

void SamView::storeFile()
{
  //store to m_filename
  QFile f(m_filename);
  if (!f.open(QIODevice::WriteOnly)) {
    fatalError(i18nc("%1 is file name", "Cannot open file: %1", m_filename));
    return;
  }

  QDomDocument doc;
  QDomElement samProjectElem = doc.createElement("samProject");

  QDomElement creationElem = doc.createElement("creation");

  if (m_creationCorpus)
    creationElem.appendChild(m_creationCorpus->serialize(&doc));

  SamXMLHelper::serializePath(&doc, creationElem, ui.urOutputModel, "outputModel");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urPrompts, "prompts");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urPromptsBasePath, "promptsBase");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urLexicon, "lexicon");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urGrammar, "grammar");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urVocabulary, "vocabulary");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urDir, "sphinxDict");

  SamXMLHelper::serializeText(&doc, creationElem, ui.leScriptPrefix->text(), "scriptPrefix");
  SamXMLHelper::serializeText(&doc, creationElem, ui.leMName->text(), "sphinxModelName");

  SamXMLHelper::serializeInt(&doc, creationElem, ui.sbSampleRate->value(), "sampleRate");
  SamXMLHelper::serializeInt(&doc, creationElem, getModelType(), "modelType");
  SamXMLHelper::serializeInt(&doc, creationElem, ui.cbType->currentIndex(), "backendType");

  SamXMLHelper::serializePath(&doc, creationElem, ui.urBaseModel, "baseModel");

  samProjectElem.appendChild(creationElem);

  QDomElement testConfigurationsElem = doc.createElement("testConfigurations");

  foreach (TestConfigurationWidget *config, testConfigurations)
    testConfigurationsElem.appendChild(config->serialize(&doc));
  samProjectElem.appendChild(testConfigurationsElem);

  if (m_reportParameters)
    samProjectElem.appendChild(m_reportParameters->serialize(&doc));

  doc.appendChild(samProjectElem);

  f.write(doc.toByteArray());

  setClean();
  updateWindowTitle();
}

void SamView::setDirty()
{
  m_dirty = true;
  updateWindowTitle();
}

void SamView::setClean()
{
  m_dirty = false;
  updateWindowTitle();
}

QHash<QString, QString> SamView::genAdaptionArgs(QString path)
{
  QHash<QString,QString> adaptionArgs;
  if(backendType == TestConfigurationWidget::SPHINX)
  {
    QString modelName = m_User+QUuid::createUuid().toString();
    adaptionArgs.insert("workingDir", path);
    adaptionArgs.insert("modelName", modelName);
    adaptionArgs.insert("stripContext", "true");
  }
  else if(backendType == TestConfigurationWidget::JHTK)
  {
    adaptionArgs.insert("lexicon", path+"lexicon");
    adaptionArgs.insert("grammar", path+"model.grammar");
    adaptionArgs.insert("simpleVocab", path+"simple.voca");
    adaptionArgs.insert("prompts", path+"samprompts");
    adaptionArgs.insert("stripContext", "true");
  }

  return adaptionArgs;
}

void SamView::getBuildPathsFromSimon()
{
  kDebug()<<"sarting adaptation";
  clearBuildLog();
  ui.urOutputModel->setUrl(KUrl(KStandardDirs::locateLocal("data", "simon/model/active.sbm")));
  ui.urPromptsBasePath->setUrl(KUrl(KStandardDirs::locateLocal("data", "simon/model/training.data/")));
  qDeleteAll(testConfigurations); //cleared by signal

  KSharedConfig::Ptr config = KSharedConfig::openConfig("speechmodelmanagementrc");
  KConfigGroup group(config, "Model");
  int sampleRate = group.readEntry("ModelSampleRate", "16000").toInt();

  int modelType = group.readEntry("ModelType", 2);
  switch (modelType) {
    case 0:
      //static model
    case 1:
      //adapted model
      ui.urBaseModel->setUrl(KStandardDirs::locate("data", "simon/model/basemodel.sbm"));
      ui.rbAdaptedBaseModel->click();
      break;
    case 2:
      //dynamic model
      ui.urBaseModel->clear();
      ui.rbDynamicModel->click();
      break;
  }
  if (modelType == 0)
    ui.rbStaticModel->click();

  ui.sbSampleRate->setValue(sampleRate);

  //target path is simon model folder
  //
  //we cannot really use a user-selected folder here for two reasons:
  // 1.) We call this method when sam starts so it should require no user interaction
  // 2.) simon will store the simond generated model there so the output files
  //     of simon will be picked up by sam.
  QString path = KStandardDirs::locate("data", "simon/model/");

  KSharedConfig::Ptr scenarioRc = KSharedConfig::openConfig("simonscenariosrc");
  KConfigGroup scenarioRcGroup(scenarioRc, "");
  QStringList scenarioIds = scenarioRcGroup.readEntry("SelectedScenarios", QStringList());
  kDebug() << "Scenario ids: " << scenarioIds;

  //cannot use the methods serializeScenariosRun and serializePromptsRun,
  //because this would make two calls to the model adaption manager
  //who, instead of queuing would abort the first job in order to work on the second one
  //this makes a lot of sense when seen in the context of simon / simond
  //
  //Moreover this way the model compilation adapter knows what we want to do and will
  //take several corner cases into account as well as streamline the model with the
  //information he can extract from the input files

  ui.leScriptPrefix->setText("simon/scripts");

  ModelCompilationAdapter::AdaptionType adaptionType = ModelCompilationAdapter::None;
  if (modelType == 0 /*static*/)
    adaptionType = ModelCompilationAdapter::AdaptLanguageModel;
  else
    adaptionType = (ModelCompilationAdapter::AdaptionType) (ModelCompilationAdapter::AdaptLanguageModel |
                                                            ModelCompilationAdapter::AdaptAcousticModel);

  QStringList scenarioPaths = findScenarios(scenarioIds);
  kDebug()<<"sarting adaptation";
  modelCompilationAdapter->startAdaption(adaptionType, scenarioPaths, path+"prompts", genAdaptionArgs(path));
}


QString SamView::getTargetDirectory()
{
  KMessageBox::information(this, i18n("You now have to provide a (preferably empty) folder where you want to serialize the scenarios to"), QString(), i18n("Do not ask again"));
  QString path = KFileDialog::getExistingDirectory(KUrl(), this, i18n("Serialized scenario output"));
  if (!path.isEmpty())
    path += QDir::separator();
  return path;
}

void SamView::importRecognitionSamples()
{
  QPointer<ConservativeTraining> training = new ConservativeTraining(this);
  training->exec();
  delete training;
}

void SamView::serializePrompts()
{
  QString promptsPath = KFileDialog::getOpenFileName(KUrl(KStandardDirs::locate("data", "simon/model/prompts")),
                                                     "", this, i18n("Open simon prompts"));
  if (promptsPath.isEmpty()) return;

  QString path = getTargetDirectory();
  if (path.isEmpty()) return;
  serializePromptsRun(promptsPath, path);
  switchToAdapt();
}


void SamView::switchToAdapt()
{
  ui.twMain->setCurrentIndex(1);
  clearBuildLog();
  ui.pbAdaptProgress->setValue(0);
}


void SamView::serializeScenarios()
{
  QString prefix = "simon/";                      //use simon scenarios and config
  QPointer<ScenarioManagementDialog> dlg = new ScenarioManagementDialog(prefix, this);
  if (dlg->exec()) {
    //reparse scenarios
    QStringList ids = dlg->getSelectedScenarioIds();
    QString path = getTargetDirectory();
    serializeScenariosRun(ids, path);
  }
  delete dlg;
  switchToAdapt();
}


void SamView::serializeScenariosRun(const QStringList& scenarioIds, const QString& output)
{
  if (output.isEmpty()) return;

  QStringList scenarioPaths = findScenarios(scenarioIds);

  modelCompilationAdapter->startAdaption(
        ModelCompilationAdapter::AdaptLanguageModel,
        scenarioPaths, ui.urPrompts->url().toLocalFile(),
        genAdaptionArgs(output));
}


void SamView::serializePromptsRun(const QString promptsPath, const QString& output)
{
  if (output.isEmpty()) return;

  QHash<QString,QString> adaptionArgs;
  adaptionArgs.insert("prompts", output+"prompts");
  //TODO: understand and customize
  modelCompilationAdapter->startAdaption(
        ModelCompilationAdapter::AdaptAcousticModel,
        QStringList(), promptsPath, adaptionArgs);
}


QStringList SamView::findScenarios(const QStringList& ids)
{
  QStringList scenarioPaths;
  foreach (const QString& id, ids) {
    QString resolvedPath = KStandardDirs::locate("data", "simon/scenarios/"+id);
    if (!QFile::exists(resolvedPath))
      KMessageBox::information(this, i18nc("%1 is scenario id", "Could not find scenario: %1", id));
    else scenarioPaths << resolvedPath;
  }
  return scenarioPaths;
}


void SamView::compileModel()
{
  ui.twMain->setCurrentIndex(2);

  clearBuildLog();

  int modelType = getModelType();
  kDebug() << "Model type: " << modelType;

  ModelCompiler::CompilationType type;
  switch (modelType) {
    case 0:
      //static model
      type = (ModelCompiler::CompileLanguageModel);
      break;
    case 1:
      //adapted base model
      type = (ModelCompiler::CompilationType)
             (ModelCompiler::CompileLanguageModel|ModelCompiler::AdaptSpeechModel);
      break;

    case 2:
      //dynamic model
      type = (ModelCompiler::CompilationType)
             (ModelCompiler::CompileLanguageModel|ModelCompiler::CompileSpeechModel);
      break;
    default:
      fatalError(i18n("Unknown model type"));
      return;
  }

  QHash<QString,QString> compilerArgs;

  if(backendType == TestConfigurationWidget::SPHINX)
  {
    compilerArgs.insert("audioPath",ui.urPromptsBasePath->url().toLocalFile());
    compilerArgs.insert("modelName", ui.leMName->text());
    compilerArgs.insert("modelDir", ui.urDir->url().toLocalFile());
  }
  else
    if(backendType == TestConfigurationWidget::JHTK)
    {
      compilerArgs.insert("samples",ui.urPromptsBasePath->url().toLocalFile());
      compilerArgs.insert("lexicon", ui.urLexicon->url().toLocalFile());
      compilerArgs.insert("grammar", ui.urGrammar->url().toLocalFile());
      compilerArgs.insert("vocab", ui.urVocabulary->url().toLocalFile());
      compilerArgs.insert("prompts", ui.urPrompts->url().toLocalFile());
      compilerArgs.insert("scriptBase", ui.leScriptPrefix->text());
    }


  modelCompiler->startCompilation(type, ui.urOutputModel->url().toLocalFile(),
                                  ui.urBaseModel->url().toLocalFile(), compilerArgs);
}

void SamView::abortModelCompilation()
{
  modelCompiler->abort();
}

void SamView::slotModelAdaptionComplete()
{
  ui.twMain->setCurrentIndex(0);

  TestConfigurationWidget *tconfig(0);

  bool fok = false;
  switch(backendType)
  {
    case TestConfigurationWidget::SPHINX:
    {
      kDebug()<<"Sphinx";

      QString modelPath = dynamic_cast<ModelCompilationAdapterSPHINX *>(modelCompilationAdapter)->workingDir();
      //    kDebug()<<"Got modeldir: " <<KUrl(modelPath).toLocalFile();
      if(!modelPath.isEmpty())
      {
        ui.urDir->setUrl(KUrl(modelPath));
        fok = true;
      }

      QString modelName = dynamic_cast<ModelCompilationAdapterSPHINX *>(modelCompilationAdapter)->modelName();
      if(!modelName.isEmpty()) ui.leMName->setText(modelName);

      //    QString audioLocation = KStandardDirs::locateLocal("appdata", "models/"+m_User+"/samples/");
      //    if(!audioLocation.isEmpty()) ui.urPromptsBasePath->setUrl(KUrl(audioLocation));

      break;
    }
    case TestConfigurationWidget::JHTK:
    {
      QString lexicon = dynamic_cast<ModelCompilationAdapterHTK *>(modelCompilationAdapter)->lexiconPath();
      QString grammar = dynamic_cast<ModelCompilationAdapterHTK *>(modelCompilationAdapter)->grammarPath();
      QString simpleVocab = dynamic_cast<ModelCompilationAdapterHTK *>(modelCompilationAdapter)->simpleVocabPath();
      QString prompts = dynamic_cast<ModelCompilationAdapterHTK *>(modelCompilationAdapter)->promptsPath();

      kDebug()<<lexicon<<"\n" << grammar <<"\n" << simpleVocab <<"\n" << prompts;

      if (!lexicon.isEmpty())
      {
        ui.urLexicon->setUrl(KUrl(lexicon));
        fok = true;
      }

      if (!grammar.isEmpty()) ui.urGrammar->setUrl(KUrl(grammar));
      if (!simpleVocab.isEmpty()) ui.urVocabulary->setUrl(KUrl(simpleVocab));
      if (!prompts.isEmpty())
      {
        ui.urPrompts->setUrl(KUrl(prompts));
        if (!m_creationCorpus)
          m_creationCorpus = createEmptyCorpusInformation();
        m_creationCorpus->setTotalSampleCount(modelCompilationAdapter->sampleCount());
        QFileInfo fi(prompts);
        QString path = fi.absolutePath();
        QString trainingDataPath = path+QDir::separator()+"training.data";
        ui.urPromptsBasePath->setUrl(KUrl(trainingDataPath));

        QString promptsTestPath = path+QDir::separator()+"samprompts_test";

        if (testConfigurations.isEmpty())
        {
          //automatically add appropriate test configuration
          QString testPromptsPathUsed;
          if (QFile::exists(promptsTestPath))
            testPromptsPathUsed = promptsTestPath;
          else
            testPromptsPathUsed = prompts;
          tconfig = new JuliusTestConfigurationWidget(createEmptyCorpusInformation(), KUrl(testPromptsPathUsed),
                                                      KUrl(trainingDataPath),ui.sbSampleRate->value(), this);
          QHash<QString, QString> params;
          params.insert("jconf", KStandardDirs::locate("data", "simond/default.jconf"));

          tconfig->init(params);
        }
      }
      break;
    }
    default:
      kDebug()<<"o_0";
  }

  addTestConfiguration(tconfig);

  if (fok)
  {
    if (!m_reportParameters)
      m_reportParameters = createEmptyReportParameters();

    m_reportParameters->setWordCount(modelCompilationAdapter->wordCount());
    m_reportParameters->setPronunciationCount(modelCompilationAdapter->pronunciationCount());
  }
  if (m_startCompilationAfterAdaption)
  {
    kDebug() << "Starting compilation";
    m_startCompilationAfterAdaption = false;
    compileModel();
  } else {
    if (m_startTestAfterAdaption)
    {
      m_startTestAfterAdaption = false;
      testModel();
    } else
      if (batchMode())
        exit(0);
  }
}

CorpusInformation* SamView::createEmptyCorpusInformation()
{
  return new CorpusInformation(i18nc("The tag name of an automatically added"
                                     " test set. The needed string really is please change this (for the user to change).",
                                     "PLEASE_CHANGE_THIS"), QString(), 0, 0, 0);
}

ReportParameters* SamView::createEmptyReportParameters()
{
  return new ReportParameters();
}

void SamView::slotModelAdaptionAborted()
{
  KMessageBox::information(this, i18n("Adaption aborted"));
}


void SamView::slotModelAdaptionStatus(QString status, int progress, int max)
{
  ui.pbAdaptProgress->setValue(progress);
  if (max != ui.pbAdaptProgress->maximum())
    ui.pbAdaptProgress->setMaximum(max);
  ui.teAdaptLog->append(status);
}


void SamView::slotModelAdaptionError(QString errorMessage)
{
  fatalError(i18nc("%1 is error message", "Failed to adapt model:\n\n%1", errorMessage));
}


void SamView::slotModelCompilationStatus(const QString& status, int now, int max)
{
  ui.pbBuildProgress->setMaximum(max);
  ui.pbBuildProgress->setValue(now);

  ui.teBuildLog->append(status);
}

void SamView::slotModelCompilationFinished()
{
  retrieveCompleteBuildLog();
  if (m_startTestAfterCompile)
  {
    testModel();
    m_startTestAfterCompile = false;
  } else if (batchMode())
    exit(0);
}

void SamView::slotModelCompilationError(const QString& error)
{
  retrieveCompleteBuildLog();
  fatalError(error);
}

void SamView::slotModelCompilationClassUndefined(const QString& undefinedClass)
{
  retrieveCompleteBuildLog();
  fatalError(i18n("Grammar class undefined: %1", undefinedClass));
}


void SamView::slotModelCompilationWordUndefined(const QString& undefinedWord)
{
  retrieveCompleteBuildLog();
  fatalError(i18n("Word undefined: %1", undefinedWord));
}


void SamView::slotModelCompilationPhonemeUndefined(const QString& undefinedPhoneme)
{
  retrieveCompleteBuildLog();
  fatalError(i18n("Phoneme undefined: %1", undefinedPhoneme));
}


void SamView::clearBuildLog()
{
  ui.teBuildLog->clear();
}

void SamView::retrieveCompleteBuildLog()
{
  QString graphicBuildLog = modelCompiler->getGraphicBuildLog();

  if (graphicBuildLog.size() > 300000) // ~ 300kb
  {
    //too big to display nicely
    ui.teBuildLog->append(i18n("Build log too big to display. Please use the button below to store it to a local file."));
  } else
  {
    ui.teBuildLog->clear();
    ui.teBuildLog->append(graphicBuildLog);
  }

  if (KCmdLineArgs::parsedArgs()->isSet("l"))
    storeBuildLog();
}


void SamView::switchToTestResults()
{
  ui.twMain->setCurrentIndex(4);
}

void SamView::clearCurrentConfiguration()
{
  delete m_reportParameters;
  delete m_creationCorpus;
  m_reportParameters = 0;
  m_creationCorpus = 0;
  m_filename.clear();
  clearTest();
  updateWindowTitle();
}

void SamView::clearTest()
{
  ui.teTestLog->clear();
  ui.lbTestResultInformation->clear();
  ui.qpPlot->hide();

  barGraph->detach();
  ui.qpPlot->replot();
  initGraph();
  barGraphLegend->clear();
}

void SamView::initGraph()
{
  delete barGraph;
  barGraph = new QwtBarsItem();
  barGraph->setType(QwtBarsItem::SideBySide);
  barGraph->attach(ui.qpPlot);
}

void SamView::testModel()
{
  ui.twMain->setCurrentIndex(3);

  clearTest();

  int startedTests = 0;
  foreach (TestResultWidget *test, testResults) {
    if (startedTests < QThread::idealThreadCount()) {
      test->startTest();
      ++startedTests;
    } else
      test->schedule();
  }

  if (testResults.isEmpty())
    KMessageBox::information(this, i18n("No tests configured yet. Please provide your test configuration in the input / output section."));

  startNextScheduledTest();
}


void SamView::abortModelTest()
{
  kDebug() << "Aborting model test";
  foreach (TestResultWidget *test, testResults)
    test->abort();
}

SamView::~SamView()
{
  modelCompiler->deleteLater();
  modelCompilationAdapter->deleteLater();
  delete m_reportParameters;
  delete m_creationCorpus;
}
