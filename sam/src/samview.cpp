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
#include "accuracydisplay.h"
#include "exporttestresults.h"
#include "testconfigurationwidget.h"
#include "testresultwidget.h"
#include "testresultplotter.h"
#include "reportparameters.h"
#include "carraydata.h"
#include "qwt_bars_item.h"
#include "qwt_series_data.h"
#include "corpusinformation.h"

#include <speechmodelcompilation/modelcompilationmanager.h>
#include <speechmodelcompilationadapter/modelcompilationadapterhtk.h>
#include <simonscenarioui/scenariomanagementdialog.h>

#include <qwt_legend.h>
#include <qwt_legend_item.h>

#include <QHash>
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
#include <KLocale>
#include <KDebug>
#include <KCmdLineArgs>
#include <QDomDocument>
#include "samxmlhelper.h"

SamView::SamView(QWidget *parent, Qt::WFlags flags) : KXmlGuiWindow(parent, flags),
  m_startCompilationAfterAdaption(false),
  m_startTestAfterCompile(false),
  m_exportAfterTest(false),
  m_dirty(false),
  m_creationCorpus(0),
  m_reportParameters(0),
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
  getPathsFromSimon->setText(i18n("Modify simons model"));
  getPathsFromSimon->setStatusTip(i18n("Manage simons current model with ssc"));
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


  connect(ui.pbCompileModel, SIGNAL(clicked()), this, SLOT(compileModel()));
  connect(ui.pbTestModel, SIGNAL(clicked()), this, SLOT(testModel()));

  KStandardAction::openNew(this, SLOT(newProject()), actionCollection());
  KStandardAction::save(this, SLOT(save()), actionCollection());
  KStandardAction::saveAs(this, SLOT(saveAs()), actionCollection());
  KStandardAction::open(this, SLOT(load()), actionCollection());
  KStandardAction::preferences(this, SLOT(showConfig()), actionCollection());
  KStandardAction::quit(this, SLOT(close()), actionCollection());

  setupGUI();

  connect(ui.rbDynamicModel, SIGNAL(toggled(bool)), this, SLOT(setDirty()));
  connect(ui.rbStaticModel, SIGNAL(toggled(bool)), this, SLOT(setDirty()));

  connect(ui.urHmmDefs, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.urTiedlist, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.urDict, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.urDFA, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.urPromptsBasePath, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.urLexicon, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.urGrammar, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.urVocabulary, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.urPrompts, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.urTreeHed, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.urWavConfig, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.urBaseHmmDefs, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.urBaseTiedlist, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.urBaseMacros, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.urBaseStats, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.leScriptPrefix, SIGNAL(textChanged(const QString&)), this, SLOT(setDirty()));
  connect(ui.sbSampleRate, SIGNAL(valueChanged(int)), this, SLOT(setDirty()));

  ui.urHmmDefs->setMode(KFile::File|KFile::LocalOnly);
  ui.urTiedlist->setMode(KFile::File|KFile::LocalOnly);
  ui.urDict->setMode(KFile::File|KFile::LocalOnly);
  ui.urDFA->setMode(KFile::File|KFile::LocalOnly);
  ui.urPromptsBasePath->setMode(KFile::Directory|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urLexicon->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urGrammar->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urVocabulary->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urPrompts->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urTreeHed->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urWavConfig->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urBaseHmmDefs->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urBaseTiedlist->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urBaseMacros->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urBaseStats->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);

  modelCompilationAdapter = new ModelCompilationAdapterHTK("internalsamuser", this);
  connect(modelCompilationAdapter, SIGNAL(adaptionComplete()), this, SLOT(slotModelAdaptionComplete()));
  connect(modelCompilationAdapter, SIGNAL(adaptionAborted()), this, SLOT(slotModelAdaptionAborted()));
  connect(modelCompilationAdapter, SIGNAL(status(QString, int)), this, SLOT(slotModelAdaptionStatus(QString, int)));
  connect(modelCompilationAdapter, SIGNAL(error(QString)), this, SLOT(slotModelAdaptionError(QString)));

  modelCompilationManager = new ModelCompilationManager("internalsamuser", this);
  connect(modelCompilationManager, SIGNAL(modelCompiled()), this, SLOT(slotModelCompilationFinished()));
  connect(modelCompilationManager, SIGNAL(activeModelCompilationAborted()), this, SLOT(retrieveCompleteBuildLog()));
  connect(modelCompilationManager, SIGNAL(status(const QString&, int, int)), this, SLOT(slotModelCompilationStatus(const QString&, int, int)));
  connect(modelCompilationManager, SIGNAL(error(const QString&)), this, SLOT(slotModelCompilationError(const QString&)));

  connect(modelCompilationManager, SIGNAL(classUndefined(const QString&)), this,
    SLOT(slotModelCompilationClassUndefined(const QString&)));
  connect(modelCompilationManager, SIGNAL(wordUndefined(const QString&)), this,
    SLOT(slotModelCompilationWordUndefined(const QString&)));
  connect(modelCompilationManager, SIGNAL(phonemeUndefined(const QString&)), this,
    SLOT(slotModelCompilationPhonemeUndefined(const QString&)));

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
    }
    if (autoTestModel)
      m_startTestAfterCompile = true;
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
  if (!modelCompilationManager->hasBuildLog())
  {
    error(i18n("No current log."));
    return;
  }

  QString buildLog = modelCompilationManager->getGraphicBuildLog();

  QString filename;
  if (KCmdLineArgs::parsedArgs()->isSet("l"))
    filename = KCmdLineArgs::parsedArgs()->getOption("l");
  else
    filename = KFileDialog::getSaveFileName(KUrl(), i18n("HTML files *.html"), this);
  if (filename.isEmpty()) return;
  QFile f(filename);
  if (!f.open(QIODevice::WriteOnly))
  {
    fatalError(i18n("Could not open output file %1.", filename));
    return;
  }
  f.write(buildLog.toUtf8());
}

void SamView::addTestConfiguration()
{
  TestConfigurationWidget *config = new TestConfigurationWidget(this);
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
  ui.teTestLog->append(i18n("Test started: %1", 
        static_cast<TestResultWidget*>(sender())->getTag()));

  displayModelTestStatus();
}

void SamView::subTestAborted()
{
  kDebug() << "Subtest aborted...";
  ui.teTestLog->append(i18n("Test aborted: %1", 
        static_cast<TestResultWidget*>(sender())->getTag()));
  displayModelTestStatus();
}

void SamView::subTestComplete()
{
  kDebug() << "Subtest completed...";
  ui.teTestLog->append(i18n("Test completed: %1", 
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
  if (testResults.count() == 1)
    ui.lbTestResultInformation->setText(i18n("Ran 1 test.", testResults.count()));
  else 
    ui.lbTestResultInformation->setText(i18n("Ran %1 tests.", testResults.count()));

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
  
  if (e->exportTestResults(m_reportParameters, creationCorpusInformation(), testResults))
  {
    ReportParameters *temp = m_reportParameters;
    m_reportParameters = e->getReportParameters();

    if (*temp != *m_reportParameters)
      setDirty();

    delete temp;

    //sync displays to potentially changed test result tags
    foreach (TestConfigurationWidget *t, testConfigurations)
      t->retrieveTag();
  }
  delete e;

  if (batchMode())
    exit(0);
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

  ui.urHmmDefs->clear();
  ui.urTiedlist->clear();
  ui.urDict->clear();
  ui.urDFA->clear();
  ui.urPromptsBasePath->clear();
  ui.urLexicon->clear();
  ui.urGrammar->clear();
  ui.urVocabulary->clear();
  ui.urPrompts->clear();
  qDeleteAll(testConfigurations); // will be removed automatically through signal magic
  ui.urTreeHed->clear();
  ui.urWavConfig->clear();
  ui.sbSampleRate->setValue(16000 /* default*/);
  ui.rbDynamicModel->click();
  ui.urBaseHmmDefs->clear();
  ui.urBaseTiedlist->clear();
  ui.urBaseMacros->clear();
  ui.urBaseStats->clear();

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

  setWindowTitle(i18n("sam - %1 [*]", decoFile));
  setWindowModified(m_dirty);
}


void SamView::parseFile()
{
  //read from m_filename
  QFile f(m_filename);
  if (!f.open(QIODevice::ReadOnly)) {
    fatalError(i18n("Cannot open file: %1", m_filename));
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

  ui.urHmmDefs->setUrl(KUrl( SamXMLHelper::getText(creationElem, "hmm") ));
  ui.urTiedlist->setUrl(KUrl( SamXMLHelper::getText(creationElem, "tiedlist") ));
  ui.urDict->setUrl(KUrl( SamXMLHelper::getText(creationElem, "dict") ));
  ui.urDFA->setUrl(KUrl( SamXMLHelper::getText(creationElem, "dfa") ));
  ui.urPrompts->setUrl(KUrl( SamXMLHelper::getText(creationElem, "prompts") ));
  ui.urPromptsBasePath->setUrl(KUrl( SamXMLHelper::getText(creationElem, "promptsBase") ));
  ui.urLexicon->setUrl(KUrl( SamXMLHelper::getText(creationElem, "lexicon") ));
  ui.urGrammar->setUrl(KUrl( SamXMLHelper::getText(creationElem, "grammar") ));
  ui.urVocabulary->setUrl(KUrl( SamXMLHelper::getText(creationElem, "vocabulary") ));
  ui.urTreeHed->setUrl(KUrl( SamXMLHelper::getText(creationElem, "tree") ));
  ui.urWavConfig->setUrl(KUrl( SamXMLHelper::getText(creationElem, "wavConfig") ));
  ui.sbSampleRate->setValue( SamXMLHelper::getInt(creationElem, "sampleRate") );
  
  ui.leScriptPrefix->setText(SamXMLHelper::getText(creationElem, "scriptPrefix"));

  int modelType = SamXMLHelper::getInt(creationElem, "modelType");
  switch (modelType) {
    case 0: ui.rbStaticModel->click();
    break;
    case 1: ui.rbAdaptedBaseModel->click();
    break;
    case 2: ui.rbDynamicModel->click();
    break;
  }
  ui.urBaseHmmDefs->setUrl(KUrl(SamXMLHelper::getText(creationElem, "baseHmm")));
  ui.urBaseTiedlist->setUrl(KUrl(SamXMLHelper::getText(creationElem, "baseTiedlist")));
  ui.urBaseMacros->setUrl(KUrl(SamXMLHelper::getText(creationElem, "baseMacros")));
  ui.urBaseStats->setUrl(KUrl(SamXMLHelper::getText(creationElem, "baseStats")));

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
    fatalError(i18n("Cannot open file: %1", m_filename));
    return;
  }
  
  QDomDocument doc;
  QDomElement samProjectElem = doc.createElement("samProject");

  QDomElement creationElem = doc.createElement("creation");
  
  if (m_creationCorpus)
    creationElem.appendChild(m_creationCorpus->serialize(&doc));

  SamXMLHelper::serializePath(&doc, creationElem, ui.urHmmDefs, "hmm");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urTiedlist, "tiedlist");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urDict, "dict");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urDFA, "dfa");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urPrompts, "prompts");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urPromptsBasePath, "promptsBase");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urLexicon, "lexicon");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urGrammar, "grammar");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urVocabulary, "vocabulary");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urTreeHed, "tree");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urWavConfig, "wavConfig");
  
  
  SamXMLHelper::serializeText(&doc, creationElem, ui.leScriptPrefix->text(), "scriptPrefix");
  
  SamXMLHelper::serializeInt(&doc, creationElem, ui.sbSampleRate->value(), "sampleRate");
  SamXMLHelper::serializeInt(&doc, creationElem, getModelType(), "modelType");
  
  SamXMLHelper::serializePath(&doc, creationElem, ui.urBaseHmmDefs, "baseHmm");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urBaseTiedlist, "baseTiedlist");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urBaseMacros, "baseMacros");
  SamXMLHelper::serializePath(&doc, creationElem, ui.urBaseStats, "baseStats");
  
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

void SamView::getBuildPathsFromSimon()
{
  clearBuildLog();
  ui.urHmmDefs->setUrl(KUrl(KStandardDirs::locateLocal("data", "simon/model/hmmdefs")));
  ui.urTiedlist->setUrl(KUrl(KStandardDirs::locateLocal("data", "simon/model/tiedlist")));
  ui.urDict->setUrl(KUrl(KStandardDirs::locateLocal("data", "simon/model/model.dict")));
  ui.urDFA->setUrl(KUrl(KStandardDirs::locateLocal("data", "simon/model/model.dfa")));
  ui.urPromptsBasePath->setUrl(KUrl(KStandardDirs::locateLocal("data", "simon/model/training.data/")));
  qDeleteAll(testConfigurations); //cleared by signal

  ui.urTreeHed->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/tree1.hed")));
  ui.urWavConfig->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/wav_config")));

  KSharedConfig::Ptr config = KSharedConfig::openConfig("speechmodelmanagementrc");
  KConfigGroup group(config, "Model");
  int sampleRate = group.readEntry("ModelSampleRate", "16000").toInt();

  int modelType = group.readEntry("ModelType", 2);
  switch (modelType) {
    case 0:
      //static model
    case 1:
      //adapted model
      ui.urBaseHmmDefs->setUrl(KStandardDirs::locate("data", "simon/model/basehmmdefs"));
      ui.urBaseTiedlist->setUrl(KStandardDirs::locate("data", "simon/model/basetiedlist"));
      ui.urBaseMacros->setUrl(KStandardDirs::locate("data", "simon/model/basemacros"));
      ui.urBaseStats->setUrl(KStandardDirs::locate("data", "simon/model/basestats"));
      ui.rbAdaptedBaseModel->click();
      break;
    case 2:
      //dynamic model
      ui.urBaseHmmDefs->clear();
      ui.urBaseTiedlist->clear();
      ui.urBaseMacros->clear();
      ui.urBaseStats->clear();
      ui.rbDynamicModel->click();
      break;
  }
  if (modelType == 0)
    ui.rbStaticModel->click();

  ui.sbSampleRate->setValue(sampleRate);

  //target path is simon model folder
  //
  //we can not really use a user selected folder here for two reasons:
  // 1.) We call this method when sam starts so it should require no user interaction
  // 2.) simon will store the simond generated model there so the output files
  //     of simon will be picked up by sam.
  QString path = KStandardDirs::locate("data", "simon/model/");

  KSharedConfig::Ptr scenarioRc = KSharedConfig::openConfig("simonscenariosrc");
  KConfigGroup scenarioRcGroup(scenarioRc, "");
  QStringList scenarioIds = scenarioRcGroup.readEntry("SelectedScenarios", QStringList());
  kDebug() << "Scenario ids: " << scenarioIds;

  //can not use the methods serializeScenariosRun and serializePromptsRun,
  //because this would make two calls to the model adaption manager
  //who, instead of queuing would abort the first job in order to work on the second one
  //this makes a lot of sense when seen in the context of simon / simond
  //
  //Moreover this way the model compilation adapter knows what we want to do and will
  //take several corner cases into account as well as streamline the model with the
  //information he can extract from the input files

  ModelCompilationAdapter::AdaptionType adaptionType = ModelCompilationAdapter::None;
  if (modelType == 0 /*static*/)
    adaptionType = (ModelCompilationAdapter::AdaptionType) (ModelCompilationAdapter::AdaptLanguageModel | 
        ModelCompilationAdapter::AdaptAcousticModel | ModelCompilationAdapter::AdaptIndependently);
  else
    adaptionType = (ModelCompilationAdapter::AdaptionType) (ModelCompilationAdapter::AdaptLanguageModel | 
        ModelCompilationAdapter::AdaptAcousticModel);
    
  ui.leScriptPrefix->setText("simon/scripts");

  QStringList scenarioPaths = findScenarios(scenarioIds);
  modelCompilationAdapter->startAdaption(
    adaptionType, path+"lexicon", path+"model.grammar",
    path+"simple.voca", path+"samprompts", scenarioPaths, path+"prompts");
}


QString SamView::getTargetDirectory()
{
  KMessageBox::information(this, i18n("You now have to provide a (preferably empty) directory where you want to serialize the scenarios to"), QString(), i18n("Do not ask again"));
  QString path = KFileDialog::getExistingDirectory(KUrl(), this, i18n("Serialized scenario output"));
  if (!path.isEmpty())
    path += QDir::separator();
  return path;
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
    (ModelCompilationAdapter::AdaptionType) (ModelCompilationAdapter::AdaptLanguageModel),
    output+"lexicon", output+"model.grammar",
    output+"simple.voca", output+"prompts",
    scenarioPaths, ui.urPrompts->url().toLocalFile());
}


void SamView::serializePromptsRun(const QString promptsPath, const QString& output)
{
  if (output.isEmpty()) return;

  modelCompilationAdapter->startAdaption(
    (ModelCompilationAdapter::AdaptionType) (ModelCompilationAdapter::AdaptAcousticModel),
    QString(), QString(),
    QString(), output+"prompts",
    QStringList(), promptsPath);
}


QStringList SamView::findScenarios(const QStringList& ids)
{
  QStringList scenarioPaths;
  foreach (const QString& id, ids) {
    QString resolvedPath = KStandardDirs::locate("data", "simon/scenarios/"+id);
    if (!QFile::exists(resolvedPath))
      KMessageBox::information(this, i18n("Could not find scenario: %1", id));
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

  ModelCompilationManager::CompilationType type;
  switch (modelType) {
    case 0:
      //static model
      type = (ModelCompilationManager::CompileLanguageModel);

      QFile::remove(ui.urHmmDefs->url().toLocalFile());
      QFile::remove(ui.urTiedlist->url().toLocalFile());
      QFile::copy(ui.urBaseHmmDefs->url().toLocalFile(), ui.urHmmDefs->url().toLocalFile());
      QFile::copy(ui.urBaseTiedlist->url().toLocalFile(), ui.urTiedlist->url().toLocalFile());
      break;
    case 1:
      //adapted base model
      type = (ModelCompilationManager::CompilationType)
        (ModelCompilationManager::CompileLanguageModel|ModelCompilationManager::AdaptSpeechModel);
      break;

    case 2:
      //dynamic model
      type = (ModelCompilationManager::CompilationType)
        (ModelCompilationManager::CompileLanguageModel|ModelCompilationManager::CompileSpeechModel);
      break;
    default:
      fatalError(i18n("Unknown model type"));
      return;
  }
  modelCompilationManager->startCompilation(
    type,
    ui.urHmmDefs->url().toLocalFile(),
    ui.urTiedlist->url().toLocalFile(),
    ui.urDict->url().toLocalFile(),
    ui.urDFA->url().toLocalFile(),

    ui.urBaseHmmDefs->url().toLocalFile(),
    ui.urBaseTiedlist->url().toLocalFile(),
    ui.urBaseMacros->url().toLocalFile(),
    ui.urBaseStats->url().toLocalFile(),

    ui.urPromptsBasePath->url().toLocalFile(),
    ui.urLexicon->url().toLocalFile(),
    ui.urGrammar->url().toLocalFile(),
    ui.urVocabulary->url().toLocalFile(),
    ui.urPrompts->url().toLocalFile(),
    ui.urTreeHed->url().toLocalFile(),
    ui.urWavConfig->url().toLocalFile(),
    ui.leScriptPrefix->text());
}


void SamView::abortModelCompilation()
{
  modelCompilationManager->abort();
}


void SamView::slotModelAdaptionComplete()
{
  ui.twMain->setCurrentIndex(0);

  QString lexicon = modelCompilationAdapter->lexiconPath();
  QString grammar = modelCompilationAdapter->grammarPath();
  QString simpleVocab = modelCompilationAdapter->simpleVocabPath();
  QString prompts = modelCompilationAdapter->promptsPath();

  if (!lexicon.isEmpty()) 
  {
    ui.urLexicon->setUrl(KUrl(lexicon));
    
    if (!m_reportParameters)
      m_reportParameters = createEmptyReportParameters();
    
    m_reportParameters->setWordCount(modelCompilationAdapter->wordCount());
    m_reportParameters->setPronunciationCount(modelCompilationAdapter->pronunciationCount());
  }
  if (!grammar.isEmpty()) ui.urGrammar->setUrl(KUrl(grammar));
  if (!simpleVocab.isEmpty()) ui.urVocabulary->setUrl(KUrl(simpleVocab));
  if (!prompts.isEmpty()) {
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
      addTestConfiguration(new TestConfigurationWidget(
              createEmptyCorpusInformation(), ui.urHmmDefs->url(), 
              ui.urTiedlist->url(), ui.urDict->url(),
              ui.urDFA->url(), KUrl(testPromptsPathUsed), KUrl(trainingDataPath), 
              KUrl(KStandardDirs::locate("data", "simond/default.jconf")), ui.sbSampleRate->value(), 
              this));
    }
  }
  if (m_startCompilationAfterAdaption)
  {
    kDebug() << "Starting compilation";
    m_startCompilationAfterAdaption = false;
    compileModel();
  } else if (batchMode())
    exit(0);
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


void SamView::slotModelAdaptionStatus(QString status, int progress)
{
  ui.pbAdaptProgress->setValue(progress);
  ui.teAdaptLog->append(status);
}


void SamView::slotModelAdaptionError(QString errorMessage)
{
  fatalError(i18n("Failed to adapt model:\n\n%1", errorMessage));
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
  QString graphicBuildLog = modelCompilationManager->getGraphicBuildLog();

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
  m_filename = QString();
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

  foreach (TestResultWidget *test, testResults)
    test->schedule();

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
  modelCompilationManager->deleteLater();
  modelCompilationAdapter->deleteLater();
  delete m_reportParameters;
  delete m_creationCorpus;
}
