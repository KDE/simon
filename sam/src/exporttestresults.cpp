/*
 *  Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "exporttestresults.h"
#include "reportparameters.h"
#include "corpusinformation.h"
#include "templatevaluelist.h"
#include "corpusinformationwidget.h"
#include "testresultwidget.h"
#include "testconfigurationwidget.h"
#include "version.h"
#include "latexreporttemplateengine.h"
#include <speechmodelcompilation/modelcompilationmanager.h>
#include <QWidget>
#include <QDir>
#include <QFile>
#include <KGlobal>
#include <KLocale>
#include <KCmdLineArgs>
#include <KMessageBox>
#include <KTabWidget>
#include <KStandardDirs>
#include <KProcess>
#include <KDebug>
#include <KFileDialog>

ExportTestResults::ExportTestResults(QWidget *parent) : KDialog(parent)
{
  QWidget *w = new QWidget(this);
  ui.setupUi(w);
  setMainWidget(w);
  setCaption(i18n("Export test results"));
  connect(ui.pbRetrieveSystemInformation, SIGNAL(clicked()), this, SLOT(initSystemDefinition()));
}

void ExportTestResults::initTemplates()
{
  QStringList templates;

  QStringList directories = KGlobal::dirs()->findDirs("data", "sam/reports/templates");
  QDir d;
  QStringList files;
  foreach (const QString& dir, directories)
  {
    if (!d.cd(dir)) continue;
    QStringList relFiles = d.entryList(QDir::NoDotAndDotDot|QDir::Files);
    foreach (const QString& f, relFiles)
      files << dir+f;
  }
  kDebug() << "Found templates: " << files;

  foreach (const QString& file, files)
  {
    QFile f(file);
    if (!f.open(QIODevice::ReadOnly))
    {
      kDebug() << "Could not open file: " << file;
      continue;
    }

    QString name = QString::fromUtf8(f.readLine().trimmed());
    ui.cbOutputFormat->addItem(name, file);
  }
}

void ExportTestResults::clearCorpora()
{
  qDeleteAll(m_creationCorporaWidgets);
  m_creationCorporaWidgets.clear();
  qDeleteAll(m_testCorporaWidgets);
  m_testCorporaWidgets.clear();
}

void ExportTestResults::initSystemDefinition()
{
  QString systemInfo;
  systemInfo += i18n("sam: part of simon %1\n", QString::fromAscii(simon_version));
  
  systemInfo += ModelCompilationManager::information(
      !ui.cbDetailedSystemInformation->isChecked() /*short version*/);

  ui.teSystemDefinition->setPlainText(systemInfo);
}

QHash<QString, QString> ExportTestResults::createTemplateValues()
{
  QHash<QString,QString> values;
  values.insert("title", ui.leTitle->text());
  values.insert("tag", ui.leTag->text());
  values.insert("taskDefinition", ui.teTaskDefinition->toPlainText());
  values.insert("conclusion", ui.teConclusion->toPlainText());
  values.insert("experimentTag", ui.leExperimentTag->text());
  values.insert("experimentDate", ui.dpExperimentDate->date().toString(Qt::DefaultLocaleShortDate));
  values.insert("experimentDateISO", ui.dpExperimentDate->date().toString(Qt::ISODate));
  values.insert("experimentDescription", ui.teExperimentDescription->toPlainText());
  values.insert("systemTag", ui.leSystemTag->text());
  values.insert("systemDefinition", ui.teSystemDefinition->toPlainText());
  values.insert("vocabularyTag", ui.leVocabularyTag->text());
  values.insert("vocabularyNotes", ui.teVocabularyNotes->toPlainText());
  values.insert("grammarTag", ui.leGrammarTag->text());
  values.insert("grammarNotes", ui.teGrammarNotes->toPlainText());

  int wordCount = ui.sbWordCount->value();
  int pronunciationCount = ui.sbPronunciationCount->value();
  int averagePronunciationsPerWord = ((float) wordCount) / ((float)pronunciationCount);
  values.insert("wordCount", QString::number(wordCount));
  values.insert("pronunciationCount", QString::number(pronunciationCount));
  values.insert("averagePronunciationsPerWord", QString::number(averagePronunciationsPerWord, 'd', 2));

  values.insert("trainingsCorpusCount", QString::number(m_creationCorporaWidgets.count()));
  values.insert("testCorpusCount", QString::number(m_testCorporaWidgets.count()));
  return values;
}

TemplateValueList* ExportTestResults::extractCorpusTemplateInformation(const QString& id, QList<CorpusInformationWidget*> widgets)
{
  TemplateValueList *l = new TemplateValueList(id);
  foreach (CorpusInformationWidget* w, widgets)
  {
    QHash<QString,QString> values;
    CorpusInformation *info = w->information();
    TemplateValueList *thisList = new TemplateValueList(info->tag());
    thisList->add("corpusTag", info->tag());
    thisList->add("corpusNotes", info->notes());
    thisList->add("corpusSpeakers", QString::number(info->speakers()));
    thisList->add("corpusSamples", QString::number(info->samples()));
    thisList->add("corpusSamplesPerSpeaker", QString::number(((float) info->samples()) / 
          ((float) info->speakers()), 'd', 2));
    l->addChild(thisList);
  }

  return l;
}

QString ExportTestResults::printPercentage(float rate)
{
  return QString::number(rate*100.0, 'd', 2);
}

QList<TemplateValueList*> ExportTestResults::createTemplateValueLists()
{
  QList<TemplateValueList*> list;

  list << extractCorpusTemplateInformation("trainingCorpora", m_creationCorporaWidgets);
  list << extractCorpusTemplateInformation("testCorpora", m_testCorporaWidgets);

  TemplateValueList *testResultsT = new TemplateValueList("testResults");
  foreach (TestResultWidget *result, testResults)
  {
    TemplateValueList *testResultT = new TemplateValueList("testResult");
    testResultT->add("testResultTag", result->getTag());
    testResultT->add("corpusTag", result->getTag()); // alias for continuity
    testResultT->add("testResultAccuracy", printPercentage(result->getAccuracy()));
    testResultT->add("testResultWER", printPercentage(result->getWordErrorRate()));
    testResultT->add("testResultConfidence", printPercentage(result->getConfidence()));

    testResultT->add("testResultSubstitutionErrors", QString::number(result->getSubstitutionErrors()));
    testResultT->add("testResultInsertionErrors", QString::number(result->getInsertionErrors()));
    testResultT->add("testResultDeletionErrors", QString::number(result->getDeletionErrors()));
    int sentenceCount = result->getSentenceCount();
    testResultT->add("testResultSentenceCount", QString::number(sentenceCount));

    TemplateValueList *sentenceResultsT = new TemplateValueList("sentenceResults");
    for (int i=0; i < sentenceCount; i++)
    {
      TemplateValueList *sentenceResultT = new TemplateValueList("sentenceResult");

      sentenceResultT->add("sentenceResultSentence", result->getSentencePrompt(i));
      sentenceResultT->add("sentenceResultCount", QString::number(result->getSentenceCount(i)));
      sentenceResultT->add("sentenceResultWER", result->getSentenceWER(i));
      sentenceResultT->add("sentenceResultAccuracy", result->getSentenceAccuracy(i));
      sentenceResultT->add("sentenceResultSubstitutionErrors", QString::number(result->getSentenceSubstitutionErrors(i)));
      sentenceResultT->add("sentenceResultInsertionErrors", QString::number(result->getSentenceInsertionErrors(i)));
      sentenceResultT->add("sentenceResultDeletionErrors", QString::number(result->getSentenceDeletionErrors(i)));
      sentenceResultsT->addChild(sentenceResultT);
    }
    testResultT->addChild(sentenceResultsT);

    testResultsT->addChild(testResultT);
  }
  list << testResultsT;

  return list;
}

void ExportTestResults::createReport()
{
  QString templateFile = getSelectedTemplate();
  QFile f(templateFile);
  if (!f.open(QIODevice::ReadOnly))
  {
    fatalError(i18n("Could not initialize output template at \"%1\". "
          "Please select a valid output format.", templateFile));
    return;
  }

  QString outputTemplateName = QString::fromUtf8(f.readLine().trimmed());
  QByteArray outputEngine = f.readLine().trimmed();

  //hardcode only currently available output engine
  if (outputEngine != "LATEX")
  {
    fatalError(i18n("Output type not supported: %1.\n\n"
          "Please make sure that your output format is compatible with this version of sam.", QString::fromUtf8(outputEngine)));
    return;
  }

  ReportTemplateEngine *engine = new LatexReportTemplateEngine;
  QByteArray templateData = f.readAll().trimmed();

  QString outputFile;
  if (KCmdLineArgs::parsedArgs()->isSet("e"))
    outputFile = KCmdLineArgs::parsedArgs()->getOption("e");
  else
    outputFile = KFileDialog::getSaveFileName(KUrl(), engine->fileType(), this);
  
  if (!outputFile.isEmpty())
  {
    saveCorporaInformation();
    if (!engine->parse(templateData, createTemplateValues(), createTemplateValueLists(),
          ui.cbGraphs->isChecked(), ui.cbTables->isChecked(), outputFile))
      fatalError(i18n("Failed to parse template: %1", engine->lastError()));
  }
   
  delete engine;
}

int ExportTestResults::exec()
{
  if (batchMode())
    return true;

  return KDialog::exec();
}

void ExportTestResults::displayCorpora(KTabWidget* tableWidget, QList<CorpusInformationWidget*>& list,
    const QList<CorpusInformation*>& corpora)
{
  kDebug() << "Displaying corpora: " << corpora;
  foreach (CorpusInformation* corpus, corpora)
  {
    kDebug() << corpus->tag();
    CorpusInformationWidget *widget = new CorpusInformationWidget(corpus, this);
    tableWidget->addTab(widget, corpus->tag());
    list << widget;
  }
}

bool ExportTestResults::exportTestResults(ReportParameters *reportParameters, QList<CorpusInformation*> creationCorpora,
                            QList<TestResultWidget*> testResults)
{
  clearCorpora();
  initTemplates();
  this->testResults = testResults;
  if (reportParameters)
  {
    ui.leTitle->setText(reportParameters->title());
    ui.leTag->setText(reportParameters->tag());
    ui.teTaskDefinition->setPlainText(reportParameters->taskDefinition());
    ui.cbOutputFormat->setCurrentIndex(ui.cbOutputFormat->findData(reportParameters->outputTemplate()));
    ui.teConclusion->setPlainText(reportParameters->conclusion());
    ui.leExperimentTag->setText(reportParameters->experimentTag());
    ui.dpExperimentDate->setDate(reportParameters->experimentDate());
    ui.teExperimentDescription->setPlainText(reportParameters->experimentDescription());
    ui.leSystemTag->setText(reportParameters->systemTag());
    ui.teSystemDefinition->setPlainText(reportParameters->systemDefinition());
    ui.leVocabularyTag->setText(reportParameters->vocabularyTag());
    ui.teVocabularyNotes->setPlainText(reportParameters->vocabularyNotes());
    ui.sbWordCount->setValue(reportParameters->wordCount());
    ui.sbPronunciationCount->setValue(reportParameters->wordCount());
    ui.leGrammarTag->setText(reportParameters->grammarTag());
    ui.teGrammarNotes->setPlainText(reportParameters->grammarNotes());

    ReportParameters::OutputOptions options = reportParameters->options();
    ui.cbTables->setChecked(options & ReportParameters::Tables);
    ui.cbGraphs->setChecked(options & ReportParameters::Graphs);
  }
  else
    initSystemDefinition();

  QList<CorpusInformation*> testCorpora;
  foreach (TestResultWidget* testResult, testResults)
    testCorpora << testResult->getConfiguration()->corpusInformation();

  kDebug() << "Exporting creation corpora: " << creationCorpora.count();
  displayCorpora(ui.twTrainingCorpora, m_creationCorporaWidgets, creationCorpora);
  displayCorpora(ui.twTestCorpora, m_testCorporaWidgets, testCorpora);

  if (exec())
  {
    createReport();
    return true;
  }
  return false;
}

QString ExportTestResults::getSelectedTemplate()
{
  return ui.cbOutputFormat->itemData(ui.cbOutputFormat->currentIndex()).toString();
}

QList<CorpusInformation*> ExportTestResults::getCorpusInformation(const QList<CorpusInformationWidget*>& widgets)
{
  QList<CorpusInformation*> infos;
  foreach (CorpusInformationWidget *w, widgets)
  {
    w->submit();
    infos << w->information();
  }
  return infos;
}

QList<CorpusInformation*> ExportTestResults::getTestCorpusInformation()
{
  return getCorpusInformation(m_testCorporaWidgets);
}

QList<CorpusInformation*> ExportTestResults::getTrainingCorpusInformation()
{
  return getCorpusInformation(m_creationCorporaWidgets);
}


ReportParameters* ExportTestResults::getReportParameters()
{
  ReportParameters::OutputOptions options = ReportParameters::None;
  if (ui.cbTables->isChecked())
    options |= ReportParameters::Tables;
  if (ui.cbGraphs->isChecked())
    options |= ReportParameters::Graphs;

  ReportParameters *reportParameters = new ReportParameters(ui.leTitle->text(), 
      ui.leTag->text(), ui.teTaskDefinition->toPlainText(),
      options, getSelectedTemplate(), ui.teConclusion->toPlainText(),
      ui.leExperimentTag->text(), ui.dpExperimentDate->date(),
      ui.teExperimentDescription->toPlainText(), ui.leSystemTag->text(),
      ui.teSystemDefinition->toPlainText(), ui.leVocabularyTag->text(),
      ui.teVocabularyNotes->toPlainText(), ui.leGrammarTag->text(), 
      ui.teGrammarNotes->toPlainText(),
      ui.sbWordCount->value(), ui.sbPronunciationCount->value()
      );
  return reportParameters;
}

void ExportTestResults::saveCorporaInformation()
{
  foreach (CorpusInformationWidget* w, m_testCorporaWidgets)
    w->submit();
  foreach (CorpusInformationWidget* w, m_creationCorporaWidgets)
    w->submit();
}

ExportTestResults::~ExportTestResults()
{
  qDeleteAll(m_creationCorporaWidgets);
  qDeleteAll(m_testCorporaWidgets);
}

