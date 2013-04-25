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

#ifndef SPHINXRECOGNITIONCONFIGURATION_H
#define SPHINXRECOGNITIONCONFIGURATION_H

#include "recognitionconfiguration.h"
#include "simonrecognizer_export.h"
#include <pocketsphinx/pocketsphinx.h>
#include <QString>


class SIMONRECOGNIZER_EXPORT SphinxRecognitionConfiguration : public RecognitionConfiguration
{
public:
  SphinxRecognitionConfiguration(const QString &modelDir, const QString &grammar, const QString& lm, const QString &dictionary, const QString& mllr, const int &samprate):
    m_ModelDir(modelDir),
    m_Grammar(grammar),
    m_LanguageModel(lm),
    m_Dictionary(dictionary),
    m_MLLR(mllr),
    m_Samprate(samprate){}

  QString getModelDir() { return m_ModelDir; }
  QString getLanguageModel() { return m_LanguageModel; }
  QString getGrammar() { return m_Grammar; }
  QString getDictionary() { return m_Dictionary; }
  QString getMLLR() { return m_MLLR; }
  int getSamprate() { return m_Samprate; }

  QStringList toArgs();
  cmd_ln_t *getSphinxConfig();


private:
  QString m_ModelDir; //-hmm
  QString m_Grammar; //-jsgf
  QString m_LanguageModel; //-lm
  QString m_Dictionary; //-dict
  QString m_MLLR; //-mllr
  int m_Samprate; //-samprate
};

#endif // SPHINXRECOGNITIONCONFIGURATION_H
