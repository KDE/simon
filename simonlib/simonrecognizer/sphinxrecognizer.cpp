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

#include <KDebug>
#include <KDE/KLocalizedString>
#include <cstdlib>
#include <stdexcept>

SphinxRecognizer::SphinxRecognizer():decoder(0)
{
}

SphinxRecognizer::~SphinxRecognizer()
{
  uninitialize();
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

    decoder = ps_init(sconfig->getSphinxConfig());

    if(!decoder)
      return false;

  } catch (std::runtime_error err)
  {
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

  int rv = ps_decode_raw(decoder, toRecognize, fName.data(), -1);
  if(rv < 0)
  {
    m_lastError = i18n("Failed to decode \"%1\"", file);
    return recognitionResults;
  }

  kDebug()<<"Recognition checkpoint";

  int score;
  char const *hyp, *uttid;
  hyp = ps_get_hyp(decoder, &score, &uttid);
  if(!hyp)
  {
    m_lastError = i18n("Can't get hypothesis for \"%1\"", file);
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
  ps_free(decoder); //WARNING: think about necessity of using QSharedPointers

  return true;
}
