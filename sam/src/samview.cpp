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

SamView::SamView(QWidget *parent, Qt::WFlags flags) : KXmlGuiWindow(parent, flags),
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
  connect(modelCompilationManager, SIGNAL(modelCompiled()), this, SLOT(retrieveCompleteBuildLog()));
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

  QString loadPath = KCmdLineArgs::parsedArgs()->getOption("l");
  if (!loadPath.isEmpty())
    load(loadPath);
}

void SamView::storeBuildLog()
{
  if (!modelCompilationManager->hasBuildLog())
  {
    KMessageBox::sorry(this, i18n("No current log."));
    return;
  }

  QString buildLog = modelCompilationManager->getGraphicBuildLog();

  QString filename = KFileDialog::getSaveFileName(KUrl(), i18n("HTML files *.html"), this);
  if (filename.isEmpty()) return;
  QFile f(filename);
  if (!f.open(QIODevice::WriteOnly))
  {
    KMessageBox::sorry(this, i18n("Could not open output file %1.", filename));
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
  testConfigurations << config;
  ui.vlTestConfigurations->addWidget(config);

  TestResultWidget *result = new TestResultWidget(config, this);
  connect(result, SIGNAL(testComplete()), this, SLOT(subTestComplete()));
  connect(result, SIGNAL(testAborted()), this, SLOT(subTestAborted()));
  connect(result, SIGNAL(testStarted()), this, SLOT(subTestStarted()));
  connect(result, SIGNAL(destroyed()), this, SLOT(testResultRemoved()));
  testResults << result;
  ui.twTestResults->addTab(result, config->tag());
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
  ui.qpPlot->replot();

  switchToTestResults();
}

void SamView::testConfigurationRemoved()
{
  testConfigurations.removeAll(static_cast<TestConfigurationWidget*>(sender()));
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
    delete m_reportParameters;
    m_reportParameters = e->getReportParameters();
    //sync displays to potentially changed test result tags
    foreach (TestConfigurationWidget *t, testConfigurations)
      t->retrieveTag();
  }
  delete e;
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
  ui.rbDynamicModel->animateClick();
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

  setWindowTitle(i18n("sam - %1", decoFile));
}


void SamView::parseFile()
{
  //read from m_filename
  QFile f(m_filename);
  if (!f.open(QIODevice::ReadOnly)) {
    KMessageBox::error(this, i18n("Cannot open file: %1", m_filename));
  }

  m_creationCorpus = readCorpusInformation(f);

  ui.urHmmDefs->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
  ui.urTiedlist->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
  ui.urDict->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
  ui.urDFA->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
  ui.urPromptsBasePath->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
  ui.urLexicon->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
  ui.urGrammar->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
  ui.urVocabulary->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
  ui.urPrompts->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
  ui.urTreeHed->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
  ui.urWavConfig->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
  ui.sbSampleRate->setValue(QString::fromUtf8(f.readLine()).trimmed().toInt());

  int modelType = f.readLine().trimmed().toInt();
  switch (modelType) {
    case 0: ui.rbStaticModel->animateClick();
    break;
    case 1: ui.rbAdaptedBaseModel->animateClick();
    break;
    case 2: ui.rbDynamicModel->animateClick();
    break;
  }
  ui.urBaseHmmDefs->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
  ui.urBaseTiedlist->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
  ui.urBaseMacros->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
  ui.urBaseStats->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));

  int testConfigCount = f.readLine().trimmed().toInt(); 
  for (int i=0; i < testConfigCount; i++)
  {
    CorpusInformation *testInfo = readCorpusInformation(f);

    KUrl hmmDefsUrl = KUrl(QString::fromUtf8(f.readLine().trimmed()));
    KUrl tiedlistUrl = KUrl(QString::fromUtf8(f.readLine().trimmed()));
    KUrl dictUrl = KUrl(QString::fromUtf8(f.readLine().trimmed()));
    KUrl dfaUrl = KUrl(QString::fromUtf8(f.readLine().trimmed()));
    KUrl testPromptsUrl = KUrl(QString::fromUtf8(f.readLine().trimmed()));
    KUrl testPromptsBasePathUrl = KUrl(QString::fromUtf8(f.readLine().trimmed()));
    KUrl jconfUrl = KUrl(QString::fromUtf8(f.readLine().trimmed()));
    int sampleRate = f.readLine().trimmed().toInt();

    addTestConfiguration(new TestConfigurationWidget(testInfo, hmmDefsUrl, tiedlistUrl, dictUrl,
          dfaUrl, testPromptsUrl, testPromptsBasePathUrl, jconfUrl, sampleRate));
  }

  QString title = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();
  QString tag = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();
  QString taskDefinition = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();
  QString outputTemplate = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();
  QString conclusion = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();
  QString experimentTag = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();
  QDate experimentDate = QDate::fromString(QString::fromUtf8(f.readLine()).trimmed(), Qt::ISODate);
  QString experimentDescription = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();
  QString systemTag = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();
  QString systemDefinition = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();
  QString vocabularyTag = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();
  QString vocabularyNotes = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();
  QString grammarTag = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();
  QString grammarNotes = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();

  int wordCount = f.readLine().trimmed().toInt();
  int pronunciationCount = f.readLine().trimmed().toInt();

  bool ok = true;
  ReportParameters::OutputOptions options = (ReportParameters::OutputOptions) f.readLine().trimmed().toInt(&ok);

  if (ok)
    m_reportParameters = new ReportParameters(title, tag,
        taskDefinition, options, outputTemplate,
        conclusion, experimentTag, experimentDate, experimentDescription,
        systemTag, systemDefinition, vocabularyTag, vocabularyNotes, grammarTag,
        grammarNotes, wordCount, pronunciationCount
        );
  else 
    m_reportParameters = 0;

  ui.twMain->setCurrentIndex(0);
  ui.teBuildLog->clear();
  ui.teAdaptLog->clear();
  updateWindowTitle();
}

CorpusInformation* SamView::readCorpusInformation(QFile &f)
{
  QString tag = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();
  QString notes = QString::fromUtf8(f.readLine()).replace("<%newline%>", "\n").trimmed();
  int speakers = f.readLine().trimmed().toInt();
  int samples = f.readLine().trimmed().toInt();
  int totalSamples = f.readLine().trimmed().toInt();
  return new CorpusInformation(tag, notes, speakers, samples, totalSamples);
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
    KMessageBox::error(this, i18n("Cannot open file: %1", m_filename));
  }

  storeCorpusInformation(f, m_creationCorpus);
  f.write(ui.urHmmDefs->url().toLocalFile().toUtf8()+'\n');
  f.write(ui.urTiedlist->url().toLocalFile().toUtf8()+'\n');
  f.write(ui.urDict->url().toLocalFile().toUtf8()+'\n');
  f.write(ui.urDFA->url().toLocalFile().toUtf8()+'\n');
  f.write(ui.urPromptsBasePath->url().toLocalFile().toUtf8()+'\n');
  f.write(ui.urLexicon->url().toLocalFile().toUtf8()+'\n');
  f.write(ui.urGrammar->url().toLocalFile().toUtf8()+'\n');
  f.write(ui.urVocabulary->url().toLocalFile().toUtf8()+'\n');
  f.write(ui.urPrompts->url().toLocalFile().toUtf8()+'\n');
  f.write(ui.urTreeHed->url().toLocalFile().toUtf8()+'\n');
  f.write(ui.urWavConfig->url().toLocalFile().toUtf8()+'\n');
  f.write(QByteArray::number(ui.sbSampleRate->value())+'\n');

  f.write(QByteArray::number(getModelType())+'\n');
  f.write(ui.urBaseHmmDefs->url().toLocalFile().toUtf8()+'\n');
  f.write(ui.urBaseTiedlist->url().toLocalFile().toUtf8()+'\n');
  f.write(ui.urBaseMacros->url().toLocalFile().toUtf8()+'\n');
  f.write(ui.urBaseStats->url().toLocalFile().toUtf8()+'\n');

  f.write(QByteArray::number(testConfigurations.count())+'\n');
  foreach (TestConfigurationWidget *config, testConfigurations)
  {
    storeCorpusInformation(f, config->corpusInformation());

    f.write(config->hmmDefs().toLocalFile().toUtf8()+'\n');
    f.write(config->tiedlist().toLocalFile().toUtf8()+'\n');
    f.write(config->dict().toLocalFile().toUtf8()+'\n');
    f.write(config->dfa().toLocalFile().toUtf8()+'\n');
    f.write(config->testPrompts().toLocalFile().toUtf8()+'\n');
    f.write(config->testPromptsBasePath().toLocalFile().toUtf8()+'\n');
    f.write(config->jconf().toLocalFile().toUtf8()+'\n');
    f.write(QByteArray::number(config->sampleRate())+'\n');
  }

  if (m_reportParameters)
  {
    f.write(m_reportParameters->title().replace('\n', "<%newline%>").toUtf8()+'\n');
    f.write(m_reportParameters->tag().replace('\n', "<%newline%>").toUtf8()+'\n');
    f.write(m_reportParameters->taskDefinition().replace('\n', "<%newline%>").toUtf8()+'\n');
    f.write(m_reportParameters->outputTemplate().replace('\n', "<%newline%>").toUtf8()+'\n');
    f.write(m_reportParameters->conclusion().replace('\n', "<%newline%>").toUtf8()+'\n');
    f.write(m_reportParameters->experimentTag().replace('\n', "<%newline%>").toUtf8()+'\n');
    f.write(m_reportParameters->experimentDate().toString(Qt::ISODate).toUtf8()+'\n');
    f.write(m_reportParameters->experimentDescription().replace('\n', "<%newline%>").toUtf8()+'\n');
    f.write(m_reportParameters->systemTag().replace('\n', "<%newline%>").toUtf8()+'\n');
    f.write(m_reportParameters->systemDefinition().replace('\n', "<%newline%>").toUtf8()+'\n');
    f.write(m_reportParameters->vocabularyTag().replace('\n', "<%newline%>").toUtf8()+'\n');
    f.write(m_reportParameters->vocabularyNotes().replace('\n', "<%newline%>").toUtf8()+'\n');
    f.write(m_reportParameters->grammarTag().replace('\n', "<%newline%>").toUtf8()+'\n');
    f.write(m_reportParameters->grammarNotes().replace('\n', "<%newline%>").toUtf8()+'\n');
    f.write(QByteArray::number(m_reportParameters->wordCount())+'\n');
    f.write(QByteArray::number(m_reportParameters->pronunciationCount())+'\n');

    f.write(QByteArray::number(m_reportParameters->options())+'\n');
  }

  updateWindowTitle();
}

void SamView::storeCorpusInformation(QFile &f, CorpusInformation* info)
{
  f.write(info->tag().replace('\n', "<%newline%>").toUtf8()+'\n');
  f.write(info->notes().replace('\n', "<%newline%>").toUtf8()+'\n');
  f.write(QByteArray::number(info->speakers())+'\n');
  f.write(QByteArray::number(info->samples())+'\n');
  f.write(QByteArray::number(info->samplesTotal())+'\n');
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
      ui.rbAdaptedBaseModel->animateClick();
      break;
    case 2:
      //dynamic model
      ui.urBaseHmmDefs->clear();
      ui.urBaseTiedlist->clear();
      ui.urBaseMacros->clear();
      ui.urBaseStats->clear();
      ui.rbDynamicModel->animateClick();
      break;
  }
  if (modelType == 0)
    ui.rbStaticModel->animateClick();

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
      KMessageBox::error(this, i18n("Unknown model type"));
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
    ui.urWavConfig->url().toLocalFile());
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
  KMessageBox::error(this, i18n("Failed to adapt model:\n\n%1", errorMessage));
}


void SamView::slotModelCompilationStatus(const QString& status, int now, int max)
{
  ui.pbBuildProgress->setMaximum(max);
  ui.pbBuildProgress->setValue(now);

  ui.teBuildLog->append(status);
}


void SamView::slotModelCompilationError(const QString& error)
{
  KMessageBox::error(this, error);
  retrieveCompleteBuildLog();
}


void SamView::slotModelCompilationClassUndefined(const QString& undefinedClass)
{
  KMessageBox::error(this, i18n("Grammar class undefined: %1", undefinedClass));
  retrieveCompleteBuildLog();
}


void SamView::slotModelCompilationWordUndefined(const QString& undefinedWord)
{
  KMessageBox::error(this, i18n("Word undefined: %1", undefinedWord));
  retrieveCompleteBuildLog();
}


void SamView::slotModelCompilationPhonemeUndefined(const QString& undefinedPhoneme)
{
  KMessageBox::error(this, i18n("Phoneme undefined: %1", undefinedPhoneme));
  retrieveCompleteBuildLog();
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
