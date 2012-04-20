/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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
 *
 *   In addition, as a special exception, the copyright holders give
 *   permission to link this portion of this program with the
 *   Julius library and distribute linked combinations including the two.
 *
 *   You must obey the GNU General Public License in all respects
 *   for all of the code used other than Julius.  If you modify
 *   file(s) with this exception, you may extend this exception to your
 *   version of the file(s), but you are not obligated to do so.  If you
 *   do not wish to do so, delete this exception statement from your
 *   version. 
 *
 *   Powered By:
 *
 *   Large Vocabulary Continuous Speech Recognition Engine Julius
 *   Copyright (c) 1997-2000 Information-technology Promotion Agency, Japan
 *   Copyright (c) 1991-2010 Kawahara Lab., Kyoto University
 *   Copyright (c) 2000-2005 Shikano Lab., Nara Institute of Science and Technology
 *   Copyright (c) 2005-2010 Julius project team, Nagoya Institute of Technology
 */

#include "juliuscontrol.h"
#include <simonrecognizer/recognitionconfiguration.h>
#include <simonrecognizer/juliusrecognitionconfiguration.h>

#include <QFile>
#include <KLocalizedString>
#include <KStandardDirs>
#include <KConfig>
#include <KDebug>
#include <KConfigGroup>
#include <KMimeType>
#include <KTar>
#include <locale.h>

JuliusControl::JuliusControl(const QString& username, QObject* parent) : RecognitionControl(username, parent),
recog(new JuliusRecognizer),
stopping(false),
m_initialized(false),
shouldBeRunning(false)
{
}

bool JuliusControl::initializeRecognition(const QString& modelPath)
{
  kDebug() << "Initializing";
  if (isInitialized()) {
    kDebug() << "Initializing recognition that was already initialized; uninitializing...";
    uninitialize();
    m_startRequests = 0;
  }
  
  QString path = KStandardDirs::locateLocal("tmp", "/simond/"+username+"/julius/");
  if (QFile::exists(path+"hmmdefs") && !QFile::remove(path+"hmmdefs")) return false;
  if (QFile::exists(path+"tiedlist") && !QFile::remove(path+"tiedlist")) return false;
  if (QFile::exists(path+"model.dfa") && !QFile::remove(path+"model.dfa")) return false;
  if (QFile::exists(path+"model.dict") && !QFile::remove(path+"model.dict")) return false;
  
  
  KTar tar(modelPath, "application/x-gzip");
  if (!tar.open(QIODevice::ReadOnly)) return false;
  
  const KArchiveDirectory *d = tar.directory();
  if (!d) return false;
  
  foreach (const QString& file, (QStringList() << "hmmdefs" << "tiedlist" << "model.dfa" << "model.dict")) {
    const KArchiveFile *entry = dynamic_cast<const KArchiveFile*>(d->entry(file));
    if (!entry) return false;
                        
    QFile f(path+file);
    if (!f.open(QIODevice::WriteOnly)) return false;
    f.write(entry->data());
    f.close();
  }

  kDebug() << "Emitting recognition ready";
  emit recognitionReady();
  return true;
}

RecognitionConfiguration* JuliusControl::setupConfig()
{
  if (!QFile::exists(KStandardDirs::locateLocal("appdata", "models/"+username+"/active/julius.jconf"))) {
    if (!QFile::copy(KStandardDirs::locate("appdata", "default.jconf"),
      KStandardDirs::locateLocal("appdata", "models/"+username+"/active/julius.jconf")))
      return 0;
  }

  QString dirPath =  KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
  QByteArray jConfPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/julius.jconf").toUtf8();
  QByteArray gram = dirPath.toUtf8()+"model";
  QByteArray tiedList = dirPath.toUtf8()+"tiedlist";
  QByteArray hmmDefs = dirPath.toUtf8()+"hmmdefs";

  return new JuliusRecognitionConfiguration(jConfPath, gram, hmmDefs, tiedList);
}

bool JuliusControl::startRecognition()
{
  kDebug() << "Starting recognition" << ++m_startRequests;
  if (isInitialized() && (m_startRequests > 1))  {
    emit recognitionStarted();
    return true;
  }
  kDebug() << "Starting recognition: Continuing";
  return startRecognitionPrivate();
}

bool JuliusControl::startRecognitionPrivate()
{
  start();
  
  emit recognitionStarted();
  return true;
}


void JuliusControl::recognize(const QString& fileName)
{
  kDebug() << "Recognizing " << fileName;
  
  queueLock.lock();
  toRecognize.enqueue(fileName);
  queueLock.unlock();
}

void JuliusControl::run()
{
  Q_ASSERT(recog);
  shouldBeRunning=true;
  
  RecognitionConfiguration *cfg = setupConfig();
  bool success = recog->init(cfg);
  delete cfg;
  if (!success) {
    emitError(i18n("Failed to setup recognition: %1", recog->getLastError()));
    return;
  }
  
  touchLastSuccessfulStart();

  m_initialized=true;
  
  while (shouldBeRunning) {
    QString file;
    queueLock.lock();
    if (!toRecognize.isEmpty())
      file = toRecognize.dequeue();
    queueLock.unlock();
    if (file.isNull()) {
      QThread::msleep(100);
    } else {
      emit recognitionResult(file, recog->recognize(file));
      emit recognitionDone(file);
    }
  }
}

bool JuliusControl::stop()
{
  kDebug() << "Stopping recognition" << m_startRequests;
  if (--m_startRequests > 0) 
    return true;
  
  if (m_startRequests < 0)
    m_startRequests = 0;
  
  kDebug() << "Stopping recognition: Continuing";
  return stopPrivate();
}

bool JuliusControl::stopPrivate()
{
  shouldBeRunning=false;
  
  if (!isRunning()) return true;
  
  if (!wait(1000)) {
    while (isRunning()) {
      kDebug() << "Forcefully terminating";
      terminate();
      wait(500);
    }
  }
  
  return true;
}

void JuliusControl::uninitialize()
{
  kDebug() << "Uninitializing julius control";
  if (!m_initialized) return;
  
  recog->uninitialize();
  stopPrivate();

  m_initialized=false;
}

QByteArray JuliusControl::getBuildLog()
{
  return "<html><head /><body><p>"+recog->getLog().replace('\n', "<br />")+"</p></body></html>";
}


void JuliusControl::emitError(const QString& error)
{
  QString specificError = error;
  QByteArray buildLog = getBuildLog();

  int indexStartVocaError = buildLog.indexOf("Error: voca_load_htkdict");
  if (indexStartVocaError != -1) {
    QByteArray findPhonesLog = buildLog;
    findPhonesLog  = findPhonesLog.replace("<br />", "\n");
    indexStartVocaError = findPhonesLog.indexOf("Error: voca_load_htkdict");
    int indexEndMissingPhones = findPhonesLog.indexOf("end missing phones");

    QList<QByteArray> lines = findPhonesLog.mid(indexStartVocaError, indexEndMissingPhones - indexStartVocaError).split('\n');

    QStringList missingPhones;
    QStringList affectedWords;

    bool thisLineMoreInfoForMissingTriphone = false;
    bool thisLineMoreMissingPhones = false;
    foreach (const QByteArray& lineByte, lines) {
      QString line = QString::fromUtf8(lineByte);
      if (line.contains(QRegExp("line [0-9]+: triphone \".*\" (or biphone \".*\" )?not found"))) {
        //Error: voca_load_htkdict: line 33: triphone "*-dh+ix" or biphone "dh+ix" not found
        //Error: voca_load_htkdict: line 33: triphone "dh-ix+s" not found
        thisLineMoreInfoForMissingTriphone = true;
      }
      else {
        if (thisLineMoreInfoForMissingTriphone) {
          //Error: voca_load_htkdict: the line content was: 2       [Towel] t aw ax l
          line = line.mid(line.lastIndexOf("       ")).trimmed();
          QString word = line.mid(line.indexOf('[')+1);
          word = word.left(word.indexOf(']'));
          affectedWords << word;

          thisLineMoreInfoForMissingTriphone = false;
        }
        else {
          if (thisLineMoreMissingPhones) {
            if (line.contains("end missing phones"))
              break;

            //Error: voca_load_htkdict: ax-m+b
            missingPhones << line.mid(26).trimmed();
          } else
          {
            if (line.contains("begin missing phones")) {
              thisLineMoreMissingPhones = true;
            }
          }
        }
      }
    }

    QString missingPhonesStr = missingPhones.join(", ");
    QString affectedWordsStr = affectedWords.join(", ");
    if (missingPhonesStr.length() > 200)
      missingPhonesStr = missingPhonesStr.left(200)+"...";
    if (affectedWordsStr.length() > 200)
      affectedWordsStr = affectedWordsStr.left(200)+"...";

    specificError = i18nc("%1 is a list of affected words, %2 is a list of phonemes", "The recognition could not be started because your model contains words that consists of sounds that are not covered by your acoustic model.\n\nYou need to either remove those words, transcribe them differently or train them.\n\nWarning: The latter will not work if you are using static base models!\n\nThis could also be a sign of a base model that uses a different phoneme set than your scenario vocabulary.\n\nThe following words are affected (list may not be complete):\n%1\n\nThe following phonemes are affected (list may not be complete):\n%2", affectedWordsStr, missingPhonesStr);
  }

  emit recognitionError(specificError, buildLog);
}

JuliusControl::~JuliusControl()
{
  uninitialize();
}
