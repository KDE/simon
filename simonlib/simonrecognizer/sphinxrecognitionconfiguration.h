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
#include <QString>
#include <QSharedPointer>
#include "simonrecognizer_export.h"
#include <pocketsphinx.h>


class SIMONRECOGNIZER_EXPORT SphinxRecognitionConfiguration : public RecognitionConfiguration
{
public:
  SphinxRecognitionConfiguration();

  QString getModelDir() { return m_ModelDir; }
  QString getGrammar() { return m_Grammar; }
  QString getDictionary() { return m_Dictionary; }

  QStringList toArgs();
  QSharedPointer<cmd_ln_t> getSphinxConfig();


private:
  QString m_ModelDir; //-hmm
  QString m_Grammar; //-jsgf
  QString m_Dictionary; //-dict
};

#endif // SPHINXRECOGNITIONCONFIGURATION_H
