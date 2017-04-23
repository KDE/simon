/*
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

#include "sphinxrecognizer.h"
#include "sphinxrecognitionconfiguration.h"
#include <simonrecognitionresult/recognitionresult.h>

#include <cstdlib>
#include <stdexcept>

#include <QUuid>
#include <QFile>
#include <KDebug>
#include <KDE/KLocalizedString>
#include <KDE/KStandardDirs>

SphinxRecognizer::SphinxRecognizer():
    logPath(KStandardDirs::locateLocal("tmp", QLatin1String("pocketsphinx_log_")+QUuid::createUuid().toString())),
    decoder(0)
{
}

SphinxRecognizer::~SphinxRecognizer()
{
  uninitialize();
}

QByteArray SphinxRecognizer::getLog()
{
  QFile f(logPath);
  f.open(QIODevice::ReadOnly);
  return f.readAll();
}

bool SphinxRecognizer::init(RecognitionConfiguration *config)
{
  kDebug()<<"SPHINX Initialization";

  if (!uninitialize())
    return false;

  if(!config)
    return false;
  try
  {
    SphinxRecognitionConfiguration *sconfig = dynamic_cast<SphinxRecognitionConfiguration*> (config);
    //old file will be closed and released by sphinxbase automatically
    FILE *logFile = fopen(logPath.toUtf8().constData(), "w");
    if (logFile == 0)
      logFile = stderr;
    err_set_logfp(logFile);

    cmd_ln_t *spconf = sconfig->getSphinxConfig();
    if(!spconf) {
      kDebug() << "Config errenous";
      return false;
    }

    decoder = ps_init(spconf);

    if(!decoder) {
      kDebug() << "Decoder setup failed";
      return false;
    }

  } catch (std::runtime_error err)
  {
    kDebug() << "Caught exception";
    return false;
  }

  return true;
}

QList<RecognitionResult> SphinxRecognizer::recognize(const QString &file)
{
  QList<RecognitionResult> recognitionResults;

  FILE *toRecognize;
  QByteArray fName = file.toUtf8();
  toRecognize = fopen(fName.data(), "rb");

  if (!toRecognize)
  {
    m_lastError = i18n("Failed to open \"%1\"", file);
    return recognitionResults;
  }

  int rv = 
#ifdef POCKETSPHINX_HAS_UTTID_APIS
      ps_decode_raw(decoder, toRecognize, fName.data(), -1);
#else
      ps_decode_raw(decoder, toRecognize, -1);
#endif
  if(rv < 0)
  {
    m_lastError = i18n("Failed to decode \"%1\"", file);
    return recognitionResults;
  }

  kDebug()<<"Recognition checkpoint";

  int score;
  char const *hyp;
#ifdef POCKETSPHINX_HAS_UTTID_APIS
  char const *uttid;
  hyp = ps_get_hyp(decoder, &score, &uttid);
#else
  hyp = ps_get_hyp(decoder, &score);
#endif
  if(!hyp)
  {
    m_lastError = i18n("Cannot get hypothesis for \"%1\"", file);
    return recognitionResults;
  }

  fclose(toRecognize);

  QString sentence = QString::fromUtf8(hyp);
  QString sampa;
  int wordsCount = sentence.split(" ", QString::SkipEmptyParts).count();

  QList<float> tlist;

  for(int i(0); i < wordsCount; i++)
  {
    tlist.append(0.99);
    sampa.append("FIXME |");
  }

  RecognitionResult res = RecognitionResult(sentence, sampa /*"FIXME"*/, sampa /*"FIXME"*/, tlist);

  recognitionResults.append(res); //TODO: Find how to get SAMPA, using sphinx..
  recognitionResults.append(res); //WARNING: some magic

  kDebug()<<"Got hypothesis: " <<QString::fromUtf8(hyp);

  return recognitionResults;
}

bool SphinxRecognizer::uninitialize()
{
  kDebug()<<"SPHINX uninitialization";
  log.clear();

  if(decoder) {
    ps_free(decoder);
    decoder = 0;
  }

  return true;
}
