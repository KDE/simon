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
#include <stdexcept>
#include <KDebug>
#include <cstdlib>

SphinxRecognizer::SphinxRecognizer()
{
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
    SphinxRecognitionConfiguration sconfig = dynamic_cast<SphinxRecognitionConfiguration> (config);

    decoder = QScopedPointer<ps_decoder_t>(ps_init(sconfig.getSphinxConfig().data()));

    if(decoder.isNull())
      return false;

  } catch (std::runtime_error err)
  {
    //TODO:
    return false;
  }

  return true;
}

QList<RecognitionResult> SphinxRecognizer::recognize(const QString &file)
{
  QList<RecognitionResult> recognitionResults;

  QSharedPointer<FILE> toRecognize;
  QByteArray fName = file.toUtf8();
  toRecognize = QSharedPointer<FILE>(fopen(fName.data(), "rb"));

  if (toRecognize.isNull())
  {
    m_lastError = i18n("Failed to open \"%1\"", file);
    return recognitionResults;
  }

  int rv = ps_decode_raw(decoder.data(), toRecognize.data(), fName.data(), -1);
  if(rv < 0)
  {
    m_lastError = i18n("Failed to decode \"%1\"", file);
    return recognitionResults;
  }

  int score;
  char const *hyp, *uttid;
  hyp = ps_get_hyp(decoder.data(), &score, &uttid);
  if(!hyp)
  {
    m_lastError = i18n("Can't get hypothesis for \"%1\"", file);
    return recognitionResults;
  }

  fclose(toRecognize.data());

  recognitionResults.append(RecognitionResult(QString(hyp), "", "", QList<float>())); //TODO: Find how to get SAMPA, using sphinx..

  return recognitionResults;
}

bool SphinxRecognizer::uninitialize()
{
  kDebug()<<"SPHINX uninitialization";
  log.clear();
  ps_free(decoder.data()); //WARNING: think about necessity of using QSharedPointers
}
