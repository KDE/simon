/* 
 *  Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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

#include "afarasview.h"
#include <simonsound/recwidget.h>
#include <QDir>
//#include <QProcess>
#include <QVBoxLayout>
#include <QKeySequence>
#include <QKeyEvent>
#include <QPointer>
#include <KDebug>
#include <KMessageBox>
#include <KStandardAction>
#include <KActionCollection>
#include <KCMultiDialog>

AfarasView::AfarasView(QWidget *parent, Qt::WFlags flags) : KXmlGuiWindow(parent, flags),
  recorder(0),
  currentIndex(0)
{
  KGlobal::locale()->insertCatalog("simonlib");
  ui.setupUi(this);

  connect(ui.pbStartReview, SIGNAL(clicked()), this, SLOT(start()));
  connect(ui.pbBlacklistSample, SIGNAL(toggled(bool)), this, SLOT(blackListSample(bool)));
  connect(ui.pbPreviousSample, SIGNAL(clicked()), this, SLOT(previousSample()));
  connect(ui.pbNextSample, SIGNAL(clicked()), this, SLOT(nextSample()));
  connect(ui.pbBack, SIGNAL(clicked()), this, SLOT(stop()));

  connect(ui.urInputPrompts, SIGNAL(textChanged(QString)), this, SLOT(inputFilesChanged()));
  connect(ui.urPathPrefix, SIGNAL(textChanged(QString)), this, SLOT(inputFilesChanged()));
  connect(ui.urOutputBlacklist, SIGNAL(textChanged(QString)), this, SLOT(inputFilesChanged()));

  ui.wgRecording->setLayout(new QVBoxLayout());

  KStandardAction::quit(this, SLOT(close()), actionCollection());
  KStandardAction::preferences(this, SLOT(showConfig()), actionCollection());
  setupGUI();
}

void AfarasView::stop()
{
  ui.swMain->setCurrentIndex(0);
  delete recorder;
  recorder=0;
}

void AfarasView::inputFilesChanged()
{
  ui.pbStartReview->setEnabled(QFile::exists(ui.urInputPrompts->url().toLocalFile()) 
        && QDir().exists(ui.urPathPrefix->url().toLocalFile())
        && !ui.urOutputBlacklist->url().toLocalFile().isEmpty());
}

void AfarasView::start()
{
  //initing prompts
  prompts.clear();

  QString promptsPath = ui.urInputPrompts->url().toLocalFile();
  QFile promptsF(promptsPath);
  if (!promptsF.open(QIODevice::ReadOnly)) {
    KMessageBox::sorry(this, i18nc("%1 is the path to the prompts file", "Could not open input prompts: %1", promptsPath));
    return;
  }
  while (!promptsF.atEnd()) {
    QString line = QString::fromUtf8(promptsF.readLine(4096));
    int separator = line.indexOf(" ");
    if (separator == -1)
      continue;
    QString file = line.left(separator);
    QString prompt = line.mid(separator+1).trimmed();

    prompts << QPair<QString,QString>(file, prompt);
  }

  QFile buildLogF(ui.urBuildLog->url().toLocalFile());
  if (buildLogF.open(QIODevice::ReadOnly)) {
    //find forced alignment stage
    QList< QPair<double, QString> > fileScores;

    QByteArray log;
    QByteArray alignLog;
    while (!buildLogF.atEnd() && !log.contains("Label file will be used to align each file")) 
      log += buildLogF.read(4096);

    while (!buildLogF.atEnd() && !alignLog.contains("HTK Configuration Parameters")) 
      alignLog += buildLogF.read(4096);

    QStringList splitLines = QString::fromUtf8(alignLog).split("Aligning File");

    foreach (const QString& sample, splitLines) {
      int index = sample.indexOf("mfcs")+5;
      int scoreIndex = sample.indexOf("frames] ")+8;
      
      QString file = sample.mid(index, sample.indexOf(".mfc")-index);
      double score = sample.mid(scoreIndex, sample.indexOf(" ", scoreIndex+8)-scoreIndex).toDouble();

      //kDebug() << "File: " << file << " score: " << score;
      fileScores << QPair<double,QString>(score, file);
    }
    qSort(fileScores);
    kDebug() << "Sorted file scores: " << fileScores.count();
    QList< QPair<QString,QString> > promptsSorted;
    int startIndex = 0;
    for (int j=0; j < fileScores.count(); j++) {
      //find appropriate prompts entry if possible
      // and move this to the front of the prompts list to check
      // ignore everything before startIndex which denotes the already filled
      // area (think about in place selection sort)
      for (int i=startIndex; i < prompts.count(); i++)
        if (prompts[i].first == fileScores[j].second) {
          prompts.insert(startIndex++, prompts.takeAt(i));
          break;
        }
    }
  }

  if (prompts.count() == 0) {
    KMessageBox::sorry(this, i18n("Prompts file is empty or has an invalid format"));
    return;
  }

  QFile blackListF(ui.urOutputBlacklist->url().toLocalFile());
  if (blackListF.open(QIODevice::ReadOnly))
    blackListedRecordings = QString::fromUtf8(blackListF.readAll()).split('\n', QString::SkipEmptyParts);

  currentIndex = 0;
  setupRecorder();

  ui.swMain->setCurrentIndex(1);
}

void AfarasView::showConfig()
{
  QPointer<KCMultiDialog> configDialog = new KCMultiDialog(this);
  configDialog->addModule("simonsoundconfig", QStringList() << "");
  configDialog->exec();
  delete configDialog;
}

void AfarasView::nextSample()
{
  currentIndex++;
  setupRecorder();
}

void AfarasView::previousSample()
{
  currentIndex--;
  setupRecorder();
}

void AfarasView::blackListSample(bool list)
{
  kDebug() << "Blacklisting sample";
  QString file = currentSample();
  if (list) {
    if (!blackListedRecordings.contains(file)) {
      blackListedRecordings << file;
      save();
    }
  } else {
    blackListedRecordings.removeAll(file);
    save();
  }
}

void AfarasView::save()
{
  kDebug() << "Saving blacklist: " << blackListedRecordings;
  QFile f(ui.urOutputBlacklist->url().toLocalFile());
  if (!f.open(QIODevice::WriteOnly))
  {
    KMessageBox::sorry(this, i18n("Could not open blacklist file for reading"));
    return;
  }
  f.write(blackListedRecordings.join("\n").toUtf8());
  f.close();
}

QString AfarasView::currentSample() const
{
  return prompts.at(currentIndex).first;
}

void AfarasView::setupRecorder()
{
  ui.lbCurrentState->setText(i18n("Sample %1 of %2", currentIndex+1, prompts.count()));
  ui.pbPreviousSample->setEnabled(currentIndex > 0);
  ui.pbNextSample->setEnabled(currentIndex+1 < prompts.count());

  QString fileSuffix = currentSample();
  QString file = ui.urPathPrefix->url().toLocalFile() + QDir::separator() + fileSuffix;
  QString prompt = prompts.at(currentIndex).second;

  ui.pbBlacklistSample->setChecked(blackListedRecordings.contains(fileSuffix));
  
  ui.wgRecording->layout()->removeWidget(recorder);

  delete recorder;
  recorder = 0;

  recorder = new RecWidget("", prompt,
                            file, true, this, QList<SimonSound::DeviceConfiguration>(), true);
  ui.wgRecording->layout()->addWidget(recorder);
}

void AfarasView::keyPressEvent ( QKeyEvent * event )
{
  if (ui.swMain->currentIndex() != 1)
  {
    QWidget::keyPressEvent(event);
    return;
  }

  switch (event->key())
  {
    case Qt::Key_P: {
        
            //QProcess::startDetached(QString("aplay -c 1 -r 16000 %1").arg(recorder->getFileTemplate()+".wav"));
        if (recorder->isPlaying())
          recorder->stopPlayback();
        else
          recorder->play();
      }
      break;
    case Qt::Key_B: ui.pbBlacklistSample->click(); break;
    case Qt::Key_Backspace: ui.pbPreviousSample->click(); break;
    case Qt::Key_N: ui.pbNextSample->click(); break;
    default:
      QWidget::keyPressEvent(event);
      break;
  }

}

AfarasView::~AfarasView()
{
}
