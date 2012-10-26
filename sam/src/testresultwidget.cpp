/*
 *  Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
 *  Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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

#include "testresultwidget.h"
#include "sphinxtestconfigurationwidget.h"
#include "juliustestconfigurationwidget.h"
#include "corpusinformation.h"
#include <simonmodeltest/juliusmodeltest.h>
#include <simonmodeltest/sphinxmodeltest.h>
#include <simonmodeltest/fileresultmodel.h>
#include <simonmodeltest/testresultmodel.h>
#include <simonmodeltest/recognizerresult.h>
#include <simonsound/recwidget.h>
#include <QSortFilterProxyModel>
#include <KStandardDirs>
#include <KMessageBox>
#include <QFile>
#include <QDir>
#include <typeinfo>

TestResultWidget::TestResultWidget(TestConfigurationWidget *configuration, QWidget *parent) : QWidget(parent),
  currentState(TestResultWidget::Idle),
  config(configuration),
	fileResultModelProxy(new QSortFilterProxyModel(this))
{
  ui.setupUi(this);

  connect(config, SIGNAL(destroyed()), this, SLOT(deleteLater()));

  if(typeid(*config) == typeid(SphinxTestConfigurationWidget))
  {
    #ifdef BACKEND_TYPE_JHTK
    // this could happen when starting a test, loaded from a configuration set up on a computer that did support sphinx
    emit testAborted();
    slotModelTestError("Sam compiled without SPHINX support", QByteArray());
    #else
    modelTest = new SphinxModelTest("internalsamuser_"+config->tag(), this);
    #endif
  }
  else if(typeid(*config) == typeid(JuliusTestConfigurationWidget)) {
    modelTest = new JuliusModelTest("internalsamuser_"+config->tag(), this);
  }

  connect(modelTest, SIGNAL(testComplete()), this, SLOT(slotModelTestCompleted()));
  connect(modelTest, SIGNAL(testAborted()), this, SLOT(slotModelTestAborted()));
  connect(modelTest, SIGNAL(status(QString,int,int)), this, SLOT(slotModelTestStatus(QString,int,int)));
  connect(modelTest, SIGNAL(recognitionInfo(QString)), this, SLOT(slotModelTestRecognitionInfo(QString)));
  connect(modelTest, SIGNAL(error(QString,QByteArray)), this, SLOT(slotModelTestError(QString,QByteArray)));

  ui.tvFiles->setModel(fileResultModelProxy);
  ui.tvFiles->setSortingEnabled(true);
  
  fileResultModelProxy->setFilterKeyColumn(0);
  fileResultModelProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  fileResultModelProxy->setSortRole(Qt::UserRole+1);
  connect(ui.leResultFilesFilter, SIGNAL(textChanged(QString)), fileResultModelProxy, SLOT(setFilterFixedString(QString)));

  ui.tvFiles->setSelectionBehavior(QAbstractItemView::SelectRows);
  connect(ui.tvFiles, SIGNAL(pressed(QModelIndex)), this, SLOT(slotFileResultSelected(QModelIndex)));
  connect(ui.pbEditSample, SIGNAL(clicked()), this, SLOT(slotEditSelectedSample()));

  fileResultModelProxy->setSourceModel(modelTest->recognizerResultsModel());
  QSortFilterProxyModel *wordFilterProxy = new QSortFilterProxyModel(this);
  wordFilterProxy->setSourceModel(modelTest->wordResultsModel());
  ui.tvWordResults->setModel(wordFilterProxy);
  QSortFilterProxyModel *sentenceFilterProxy = new QSortFilterProxyModel(this);
  sentenceFilterProxy->setSourceModel(modelTest->sentenceResultsModel());
  ui.tvSentenceResults->setModel(sentenceFilterProxy);
}

void TestResultWidget::slotModelTestAborted()
{
  currentState = TestResultWidget::Aborted;
  retrieveCompleteTestLog();
  emit testAborted();
}

void TestResultWidget::slotModelTestCompleted()
{
  currentState = TestResultWidget::Done;
  retrieveCompleteTestLog();
  analyzeTestOutput();
  config->corpusInformation()->setTotalSampleCount(modelTest->getTotalSampleCount());
  emit testComplete();
}

void TestResultWidget::startTest()
{
  currentState = TestResultWidget::Running;

  QHash<QString, QString> testParams;

  if(typeid(*config) == typeid(SphinxTestConfigurationWidget))
  {
    SphinxTestConfigurationWidget *sconfig = dynamic_cast<SphinxTestConfigurationWidget*>(config);
    testParams.insert("modelDir", sconfig->sphinxModelDir().toLocalFile());
    testParams.insert("grammar", sconfig->sphinxGrammar().toLocalFile());
    testParams.insert("dictionary", sconfig->sphinxDictionary().toLocalFile());
  }
  else if(typeid(*config) == typeid(JuliusTestConfigurationWidget))
  {
    JuliusTestConfigurationWidget *jconfig = dynamic_cast<JuliusTestConfigurationWidget*>(config);
    testParams.insert("hmmDefsPath", jconfig->hmmDefs().toLocalFile());
    testParams.insert("tiedListPath", jconfig->tiedlist().toLocalFile());
    testParams.insert("dictPath", jconfig->dict().toLocalFile());
    testParams.insert("dfaPath", jconfig->dfa().toLocalFile());
    testParams.insert("juliusJConf", jconfig->jconf().toLocalFile());
  }

  modelTest->startTest(config->testPromptsBasePath().toLocalFile(),
                       config->testPrompts().toLocalFile(),
                       config->sampleRate(), testParams);

  emit testStarted();
}

void TestResultWidget::slotEditSelectedSample()
{
  QModelIndex index = ui.tvFiles->currentIndex();
  if (!index.isValid())
    return;

  QString fileName = ui.tvFiles->model()->data(index, Qt::UserRole).toString();
  RecognizerResult *t = modelTest->getRecognizerResult(fileName);

  if (!t) return;

  //copy to temp
  QString justFileName = fileName.mid(fileName.lastIndexOf(QDir::separator())+1);
  QString tempFileName = KStandardDirs::locateLocal("tmp",
    "sam/internalsamuser/edit/"+justFileName);

  //if file could not be copied this is not a reason to display an error or to abort
  //because we could have already deleted the file
  QFile::copy(fileName, tempFileName);

  QPointer<KDialog> d = new KDialog(0);
  RecWidget *rec = new RecWidget(i18n("Modify sample"),
    t->getPrompt(), tempFileName.left(tempFileName.lastIndexOf('.')), true, d);
  d->setMainWidget(rec);
  if (d->exec()) {
    if (!QFile::exists(tempFileName)) {
      //sample has been deleted
      //removing file from prompts
      QFile prompts(config->testPrompts().toLocalFile());
      QString tempPromptsPath = KStandardDirs::locateLocal("tmp", "sam/internalsamuser/edit/prompts");
      QFile temp(tempPromptsPath);
      if ((!prompts.open(QIODevice::ReadOnly)) ||
      (!temp.open(QIODevice::WriteOnly))) {
        KMessageBox::error(this, i18n("Could not modify prompts file"));
      }
      else {
        while (!prompts.atEnd()) {
          QString line = QString::fromUtf8(prompts.readLine());
          if (!line.startsWith(justFileName.left(justFileName.lastIndexOf('.'))+' ')) {
            temp.write(line.toUtf8());
          }
        }

        if ((!QFile::remove(config->testPrompts().toLocalFile())) ||
        (!QFile::copy(tempPromptsPath, config->testPrompts().toLocalFile()))) {
          KMessageBox::error(this, i18n("Could not overwrite prompts file"));
        }

        if (!QFile::remove(fileName)) {
          KMessageBox::error(this, i18nc("%1 is file name", "Could not remove original sample:  %1.", fileName));
        }
      }
    }
    else {
      if (!QFile::exists(fileName)) {
        //we have to re-add this to the prompts
        QFile prompts(config->testPrompts().toLocalFile());
        if (!prompts.open(QIODevice::WriteOnly|QIODevice::Append)) {
          KMessageBox::error(this, i18n("Could not modify prompts file"));
        }
        else {
          prompts.write(QString("%1 %2").arg(justFileName).arg(t->getPrompt()).toUtf8());
        }
      }
      else if (!QFile::remove(fileName)) {
        KMessageBox::error(this, i18nc("%1 is file name", "Could not remove original sample:  %1.", fileName));
      }

      //copy sample back
      if (!QFile::copy(tempFileName, fileName)) {
        KMessageBox::error(this, i18nc("%1 is source file name, %2 is destination file name", "Could not copy sample from temporary path %1 to %2.",
          tempFileName, fileName));
      }
    }
  }

  //remove temp sample
  QFile::remove(tempFileName);
  delete d;
}


void TestResultWidget::slotFileResultSelected(const QModelIndex& index)
{
  QString fileName = ui.tvFiles->model()->data(index, Qt::UserRole).toString();
  RecognizerResult *t = modelTest->getRecognizerResult(fileName);

  if (!t) return;

  RecognitionResultList results = t->getResults();

  QString resultInfo="";
  int i=1;
  foreach (const RecognitionResult& result, results) {
    resultInfo += i18n("Result %1 of %2\n=====================\n", i, results.count());
    resultInfo += result.toString();
    resultInfo += "\n\n";
    i++;
  }

  ui.teResultDetails->clear();
  ui.teResultDetails->append(resultInfo);
}

void TestResultWidget::abort()
{
  modelTest->abort();
}

void TestResultWidget::slotModelTestStatus(const QString& status, int now, int max)
{
  ui.pbTestProgress->setMaximum(max);
  ui.pbTestProgress->setValue(now);

  ui.teTestLog->append(status);
}


void TestResultWidget::slotModelTestRecognitionInfo(const QString& status)
{
  ui.teTestLog->append(status);
}


void TestResultWidget::slotModelTestError(const QString& error, const QByteArray& protocol)
{
  slotModelTestAborted();
  KMessageBox::detailedSorry(0, error, protocol);
}

void TestResultWidget::retrieveCompleteTestLog()
{
  ui.teTestLog->clear();
  ui.teTestLog->append(modelTest->getGraphicLog());
}

void TestResultWidget::displayRate(QProgressBar *pbRate, float rate)
{
  pbRate->setValue(qRound(rate*100.0f));
  pbRate->setFormat(QString::number(rate*100.0f, 'f', 2)+" %");
}

void TestResultWidget::analyzeTestOutput()
{
  float overallRecognitionRate = modelTest->getOverallConfidence();
  displayRate(ui.pbRecognitionRate, overallRecognitionRate);

  displayRate(ui.pbAccuracy, modelTest->getOverallAccuracy());
  displayRate(ui.pbWordErrorRate, modelTest->getOverallWER());
}


QString TestResultWidget::getTag()
{
  return config->tag();
}

float TestResultWidget::getAccuracy()
{
  return modelTest->getOverallAccuracy();
}

float TestResultWidget::getWordErrorRate()
{
  return modelTest->getOverallWER();
}

float TestResultWidget::getConfidence()
{
  return modelTest->getOverallConfidence();
}

int TestResultWidget::getSampleCount()
{
  return modelTest->getTotalSampleCount();
}

int TestResultWidget::getCorrect()
{
  return modelTest->getCorrect();
}

int TestResultWidget::getSubstitutionErrors()
{
  return modelTest->getSubstitutionErrors();
}

int TestResultWidget::getInsertionErrors()
{
  return modelTest->getInsertionErrors();
}

int TestResultWidget::getDeletionErrors()
{
  return modelTest->getDeletionErrors();
}

int TestResultWidget::getSentenceCount()
{
  return modelTest->getSentenceCount();
}

void TestResultWidget::schedule()
{
  currentState = TestResultWidget::Waiting;
}

QVariant TestResultWidget::getSentenceResultValue(int row, int column)
{
  if (!modelTest->sentenceResultsModel()) return QVariant();

  return modelTest->sentenceResultsModel()->data(row, column);
}

QString TestResultWidget::getSentencePrompt(int i)
{
  return getSentenceResultValue(i, 0).toString();
}

int TestResultWidget::getSentenceCount(int i)
{
  return getSentenceResultValue(i, 1).toInt();
}

QString TestResultWidget::getSentenceWER(int i)
{
  return getSentenceResultValue(i, 6).toString();
}

QString TestResultWidget::getSentenceAccuracy(int i)
{
  return getSentenceResultValue(i, 5).toString();
}

int TestResultWidget::getSentenceSubstitutionErrors(int i)
{
  return getSentenceResultValue(i, 9).toInt();
}

int TestResultWidget::getSentenceInsertionErrors(int i)
{
  return getSentenceResultValue(i, 7).toInt();
}

int TestResultWidget::getSentenceDeletionErrors(int i)
{
  return getSentenceResultValue(i, 8).toInt();
}

TestResultWidget::~TestResultWidget()
{
  modelTest->deleteLater();
  fileResultModelProxy->deleteLater();
}
