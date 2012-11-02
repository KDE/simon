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

#include "graphemetophoneme.h"
#include <KProcess>
#include <KStandardDirs>
#include <KDebug>
#include <KLocalizedString>
#include <QFile>

GraphemeToPhoneme::GraphemeToPhoneme(QObject *parent) : QObject(parent),
	sequitur(new KProcess(this)),
	m_state(GraphemeToPhoneme::Idle)
{
  connect(sequitur, SIGNAL(finished(int)), this, SLOT(nextStep(int)));
}

bool GraphemeToPhoneme::findSequitur(QString& out)
{
  out = KStandardDirs::findExe("g2p.py");
  
  if (out.isEmpty()) {
    out = i18n("<html><body>Sequitur G2P is required to use this feature.\n\n"
	"It's free software and you can download it from "
	"<a href=\"http://www-i6.informatik.rwth-aachen.de/web/Software/g2p.html\">"
	"its homepage the university of Aachen</a>.</body></html>");
    return false;
  }
  return true;
}

bool GraphemeToPhoneme::createProfile()
{
  if (!findSequitur(sequiturExe)) {
    emit failed();
    return false;
  }
  
  sequitur->setWorkingDirectory(KStandardDirs::locateLocal("tmp", "simon/sequitur/"));
  sequitur->setOutputChannelMode(KProcess::OnlyStderrChannel);
 
  nextStep();
  return true;
}

void GraphemeToPhoneme::nextStep(int finish)
{
  if (finish != 0) {
    //error handling
    error = i18nc("%1 is error message", "An unexpected error occurred when creating the model with sequitur:\n\n%1", QString::fromUtf8(sequitur->readAllStandardError()));
    
    sequitur->terminate(); //if we are not yet done, abort
    m_state = Idle;
    kDebug() << "Error: " << error;
    
    emit failed();
    return;
  }
  
  switch(m_state) {
    case Idle:
      sequitur->setProgram(sequiturExe, QStringList() << "--train" << "train.lex" << "--devel" << "5%" << "--write-model" << "model1");
      m_state = Initial;
      emit state(i18n("Creating initial model..."), 3, 100);
      break;
    case Initial:
      sequitur->setProgram(sequiturExe, QStringList() << "--model" << "model1" << "--ramp-up" << "--train" << "train.lex" << "--devel" << "5%" << "--write-model" << "model2");
      m_state = RampUp1;
      emit state(i18n("Improving model (1/4)..."), 7, 100);
      break;
    case RampUp1:
      sequitur->setProgram(sequiturExe, QStringList() << "--model" << "model2" << "--ramp-up" << "--train" << "train.lex" << "--devel" << "5%" << "--write-model" << "model3");
      m_state = RampUp2;
      emit state(i18n("Improving model (2/4)..."), 15, 100);
      break;
    case RampUp2:
      sequitur->setProgram(sequiturExe, QStringList() << "--model" << "model3" << "--ramp-up" << "--train" << "train.lex" << "--devel" << "5%" << "--write-model" << "model4");
      m_state = RampUp3;
      emit state(i18n("Improving model (3/4)..."), 32, 100);
      break;
    case RampUp3:
      sequitur->setProgram(sequiturExe, QStringList() << "--model" << "model4" << "--ramp-up" << "--train" << "train.lex" << "--devel" << "5%" << "--write-model" << "model5");
      m_state = RampUp4;
      emit state(i18n("Improving model (4/4)..."), 58, 100);
      break;
    case RampUp4: {
      emit state(i18n("Finished."), 100,100);
      emit success(KStandardDirs::locateLocal("tmp", "simon/sequitur/model5"));
      break;
    }
    default:
      kDebug() << "Not implemented";
      return;
  };

   sequitur->start();
}

void GraphemeToPhoneme::abort()
{
  sequitur->terminate();
}

GraphemeToPhoneme::GraphemeToPhonemeState GraphemeToPhoneme::getState()
{
  return m_state;
}

QString GraphemeToPhoneme::getError()
{
  return error;
}

QHash< QString, TranscriptionResult > GraphemeToPhoneme::transcribe(const QStringList& words, const QString& pathToModel)
{
  kDebug() << "Transcribing: " << words;
  QHash<QString, TranscriptionResult> transcribed;
  
  QString sequiturExe;
  if (!findSequitur(sequiturExe))
    return transcribed;

  QString tempFilePath = KStandardDirs::locateLocal("tmp", "simon/sequitur/toTranscribe");
  QFile f(tempFilePath);
  if (!f.open(QIODevice::WriteOnly))
    return transcribed;
  
  QList<QByteArray> wordListPrepared;
  foreach (const QString& word, words) {
    QByteArray w = word.toUtf8().toUpper();
    if (!wordListPrepared.contains(w)) {
      wordListPrepared << w;
      f.write(w+'\n');
    }
  }
  f.close();
  
  KProcess sequitur;
  sequitur.setProgram(sequiturExe, QStringList() << "-m" << pathToModel << "-a" << tempFilePath);
  sequitur.setOutputChannelMode(KProcess::SeparateChannels);
  sequitur.start();
  if (!sequitur.waitForFinished(3000))
    sequitur.terminate();
  if (sequitur.exitStatus() != QProcess::NormalExit)
    return transcribed;
  
  QStringList errors = QString::fromUtf8(sequitur.readAllStandardError()).trimmed().split('\n');
  QStringList outputLines = QString::fromUtf8(sequitur.readAllStandardOutput()).trimmed().split('\n');
  kDebug() << "Errors: " << errors;
  kDebug() << "Output: " << outputLines;
  int errorIndex = 0;
  int outputIndex = 0;
  for (int i=0; i < wordListPrepared.count(); i++) {
    const QString& key = QString::fromUtf8(wordListPrepared[i]);
    if (outputIndex == outputLines.count()) {
      for (int j = errorIndex; j < errors.count() - 1; j++)
        transcribed.insert(key, TranscriptionResult(false, errors[j]));
      break;
    }
    
    QString line = outputLines[outputIndex];
    kDebug() << "Word result: " << key;
    if (line.startsWith(key+'\t')) {
      transcribed.insert(key, TranscriptionResult(true, line.mid(key.length()+1)));
      outputIndex++;
      kDebug() << "Found!" << line;
    } else {
      QString error;
      if (errors.count() > errorIndex) {
        kDebug() << "Error!" << errors[errorIndex];
        transcribed.insert(key, TranscriptionResult(false, errors[errorIndex++]));
      } else
        transcribed.insert(key, TranscriptionResult(false, i18n("Unknown error")));
    }
  }
  
  return transcribed;
}
