/*
 *   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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


#include "juliusrecognizer.h"
#include "recognitionconfiguration.h"
#include "juliusrecognitionconfiguration.h"

#include <simonrecognitionresult/recognitionresult.h>

#include <QMutexLocker>
#include <QRegExp>

#include <KDE/KLocalizedString>
#include <KDE/KStandardDirs>
#include <KProcess>
#include <KDebug>
#include <QThread>

JuliusRecognizer::JuliusRecognizer() : m_juliusProcess(0)
{
}

QByteArray JuliusRecognizer::readData()
{
  QByteArray t = m_juliusProcess->readAll();
//   kDebug() << "Just read: " << t;
  log += t+'\n';
  return t;
}


bool JuliusRecognizer::init(RecognitionConfiguration* config)
{
  if (!uninitialize())
    return false;
  
  m_juliusProcess = new KProcess;
  m_juliusProcess->setOutputChannelMode(KProcess::MergedChannels);
  QString exe = KStandardDirs::findExe("julius");
  if (exe.isNull())
  {
    m_lastError = i18n("Failed to find executable \"julius\". Please make sure that Julius is installed and that it can be found in your path.");
    return false;
  }
  
  m_juliusProcess->setProgram(exe, config->toArgs());
  kDebug() << "Starting process " << exe << " with arguments " << config->toArgs();
  return startProcess();
}

bool JuliusRecognizer::startProcess()
{
  m_juliusProcess->start();
  if (!m_juliusProcess->waitForStarted())
  {
    m_lastError = i18n("Failed to start Julius with given model");
    m_juliusProcess->kill();
    return false;
  }
  kDebug() << "Julius started - let's block for the prompt now";

  if (!blockTillPrompt())
  {
    m_lastError = i18n("Julius did not initialize correctly");
    return false;
  }

  return true;
}


bool JuliusRecognizer::blockTillPrompt(QByteArray *data)
{
  //wait until julius is ready
  QByteArray currentData;
  while (m_juliusProcess->bytesAvailable() || m_juliusProcess->waitForReadyRead())
  {
    currentData = readData();
    if (data) *data += currentData;
    if (currentData.contains("enter filename->"))
      return true;
  }
  kDebug() << "Failed to block for prompt: " << m_juliusProcess->exitCode() << m_juliusProcess->state() << m_juliusProcess->error();
  kDebug() << "Last data: " << currentData;
  return false;
}

QList< RecognitionResult > JuliusRecognizer::recognize(const QString& file)
{
  QList<RecognitionResult> recognitionResults;
  
  kDebug() << "Recognizing on file: " << file;
  QMutexLocker l(&recognitionLock);
  
  if (m_juliusProcess->state() == QProcess::NotRunning)
  {
    kDebug() << "Recognition requested even though julius was not running. Restarting";
    if (!startProcess())
      return recognitionResults;
  }
  
  m_juliusProcess->write(file.toUtf8()+'\n');
  
  /*
    Typical output:
    
    Stat: adin_sndfile: input speechfile: foo.wav
    Stat: adin_sndfile: input format = Microsoft WAV
    Stat: adin_sndfile: input type = Signed 16 bit PCM
    Stat: adin_sndfile: endian = file native endian
    Stat: adin_sndfile: 16000 Hz, 1 channels
    STAT: 38400 samples (2.40 sec.)
    STAT: ### speech analysis (waveform -> MFCC)
    ### Recognition: 1st pass (LR beam)
    ..............................................................................................................................................................................................................................................pass1_best: <s> Ok </s>
    pass1_best_wordseq: 0 7 1
    pass1_best_phonemeseq: sil | ow k ey | sil
    pass1_best_score: -5488.479980
    ### Recognition: 2nd pass (RL heuristic best-first)
    STAT: 00 _default: 34 generated, 34 pushed, 33 nodes popped in 238
    sentence1: <s> Ok </s>
    wseq1: 0 7 1
    phseq1: sil | ow k ey | sil
    cmscore1: 1.000 0.948 1.000
    score1: -5443.478516
    sentence2: <s> Stop window </s>
    wseq2: 0 2 3 1
    phseq2: sil | s t aa p | w ih n d ow | sil
    cmscore2: 1.000 0.817 0.023 1.000
    score2: -5460.654297
    sentence3: <s> Start Music </s>
    wseq3: 0 9 10 1
    phseq3: sil | s t aa r t | m y uw z ix k | sil
    cmscore3: 1.000 0.552 0.013 1.000
    score3: -5464.158203

    ------
    
    */ 
  
  QByteArray result;
  
  //block till next prompt - the results will be in the std out stream afterwards
  if (!blockTillPrompt(&result))
  {
    m_lastError = i18n("Julius did not process the sample correctly");
    return recognitionResults;
  }
  
  QStringList results = QString::fromUtf8(result).split("\nsentence");
  results.takeFirst(); // remove preamble
  
  foreach (const QString& r, results)
  {
    QString result = r.mid(r.indexOf(" <s> ")+5);
    result = result.left(result.indexOf(" </s>"));

    QString sampa = r.mid(r.indexOf("sil | "));
    sampa = sampa.left(sampa.indexOf(" | sil")+6);

    QList<float> confidenceScores;
    QRegExp cmScore("cmscore[0-9]*:");
    QString confStr = r.mid(r.indexOf(cmScore));
    confStr = confStr.left(confStr.indexOf('\n'));
    confStr.remove(cmScore);
    bool ok = true;
    foreach (const QString& scoreStr, confStr.split(' ', QString::SkipEmptyParts))
    {
      confidenceScores.append(scoreStr.toFloat(&ok));
      if (!ok)
      {
        m_lastError = i18nc("%1 is the confidence score string that failed to parse",
                            "Failed to read confidence score: %1", scoreStr);
        break;
      }
    }
    if (confidenceScores.size() > 1)
    { // otherwise we have no sentence marker
      confidenceScores.takeFirst();
      confidenceScores.takeLast(); // remove confidence scores for <s> and </s>
    }
    if (!ok)
      continue;
    recognitionResults.append(RecognitionResult(result,
                              sampa, sampa, confidenceScores));
    //kDebug() <<result <<"\t"<<
  }
  
  return recognitionResults;
}

bool JuliusRecognizer::uninitialize()
{
  if (!m_juliusProcess) return true; // already uninitialized

  kDebug() << "Uninitializing";
  log.clear();
  if (m_juliusProcess && (m_juliusProcess->state() != QProcess::NotRunning))
  {
    m_juliusProcess->terminate();
    if (!m_juliusProcess->waitForFinished())
    {
      m_juliusProcess->kill();
      if (!m_juliusProcess->waitForFinished())
      {
        m_lastError = i18n("Failed to stop running Julius process");
        return false;
      }
    }
  }
  delete m_juliusProcess;
  m_juliusProcess = 0;
  return true;
}

JuliusRecognizer::~JuliusRecognizer()
{
  uninitialize();
}
