/* 
 *  Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>

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

#include "conservativetraining.h"
#include "ui_conservativetraining.h"

#include <QFile>
#include <QDir>
#include <QStringList>

#include <KMessageBox>
#include <KDebug>

ConservativeTraining::ConservativeTraining(QWidget *parent) : KDialog(parent),
  ui(new Ui::ConservativeTrainingDlg)
{
  QWidget *widget = new QWidget( this );
  ui->setupUi(widget);
  setMainWidget( widget );
  setCaption( i18n("Conservative Training") );

  connect(ui->urInputFolder, SIGNAL(textChanged(const QString&)), this, SLOT(updateComplete()));
  connect(ui->urOutputPrompts, SIGNAL(textChanged(const QString&)), this, SLOT(updateComplete()));
  connect(ui->sbMinimumConfidence, SIGNAL(valueChanged(double)), this, SLOT(updateComplete()));

  updateComplete();
}

void ConservativeTraining::updateComplete()
{
  enableButtonOk(ui->urInputFolder->url().isValid() && ui->urOutputPrompts->url().isValid());
}

void ConservativeTraining::accept()
{
  QString path;
  QString promptsPath;
  double minConfidence;

  path = ui->urInputFolder->url().toLocalFile();
  promptsPath = ui->urOutputPrompts->url().toLocalFile();
  minConfidence = ui->sbMinimumConfidence->value();
  
  bool ok;

  //path = KFileDialog::getExistingDirectory(KUrl(), this, i18n("Recognition sample folder"));
  //if (path.isEmpty()) return;
  // minConfidence = KInputDialog::getDouble(i18n("Minimum confidence"), i18n("You can limit "
        // "the import to samples that were recognized with at least the following confidence rate "
        // "(range: 0 - 1; Setting this to 0 means to import everything)."),
        // 0.7, 0, 1,0.1,5,&ok);
  // if (!ok) return;

  //promptsPath = KFileDialog::getSaveFileName(KUrl(), i18n("Prompts files"), this, i18n("Output prompts"));

  QFile prompts(promptsPath);
  if (!prompts.open(QIODevice::WriteOnly)) {
    KMessageBox::sorry(this, i18n("Couldn't open output prompts"));
    return;
  }

  kDebug() << "Importing: " << path << minConfidence;

  QString delimiter = ".wav-log.txt";
  int delimiterSize = delimiter.size();

  QStringList logs = QDir(path).entryList(QStringList() << "*"+delimiter, 
      QDir::Files|QDir::Readable|QDir::NoDotAndDotDot);

  foreach (const QString& log, logs) {
    QFile f(path+QDir::separator()+log);
    if (!f.open(QIODevice::ReadOnly)) {
      kWarning() << "Couldn't open file: " << log;
      continue;
    }
    QList< QPair<double, QString> > scoring;
    while (!f.atEnd()) {
      QString sentence = QString::fromUtf8(f.readLine());
      sentence = sentence.mid(sentence.indexOf(": ")+2).trimmed();
      f.readLine(); // sampa
      f.readLine(); // raw sampa
      QString averageConfidence = QString::fromUtf8(f.readLine());
      averageConfidence = averageConfidence.mid(averageConfidence.indexOf(": ")+2).trimmed();

      double dConfidence = KGlobal::locale()->readNumber(averageConfidence, &ok) / 100;
      if (ok &&
          (dConfidence >= minConfidence)) {
        kDebug() << "Sentence: " << sentence << "Confidence: " << dConfidence;
        scoring << qMakePair(dConfidence, sentence);
      }
      if (!ok) kDebug() << "Not ok: " << averageConfidence;

      f.readLine(); // individual confidences
      f.readLine(); // empty line
    }

    if (scoring.isEmpty()) continue;

    qSort(scoring);
    QString file = log.left(log.size()-delimiterSize);
    kDebug() << "Best guess for file: " << file << scoring.last().second;

    prompts.write(QString("%1 %2\n").arg(file).arg(scoring.last().second.toUpper()).toUtf8());
  }

  prompts.close();
  KDialog::accept();
}

ConservativeTraining::~ConservativeTraining()
{
  delete ui;
}

