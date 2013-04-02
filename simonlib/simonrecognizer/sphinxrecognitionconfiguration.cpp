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

#include "sphinxrecognitionconfiguration.h"
#include <KDebug>

QStringList SphinxRecognitionConfiguration::toArgs()
{
  QStringList args;
  args << "-hmm " << m_ModelDir
       << "-dict " << m_Dictionary
       << "-samprate" << QString::number(m_Samprate);
  if (!m_Grammar.isEmpty())
       args << "-jsgf " << m_Grammar;
  else if (!m_LanguageModel.isEmpty())
       args << "-lm " << m_LanguageModel;
  return args;
}

cmd_ln_t* SphinxRecognitionConfiguration::getSphinxConfig()
{
  kDebug()<<"Creating sphinx configuration";
  kDebug()<<m_ModelDir;
  kDebug()<<m_Grammar;
  kDebug()<<m_LanguageModel;
  kDebug()<<m_Dictionary;
  QByteArray model = m_ModelDir.toUtf8();
  QByteArray grammar = m_Grammar.toUtf8();
  QByteArray lm = m_LanguageModel.toUtf8();
  QByteArray dict = m_Dictionary.toUtf8();
  QByteArray samprate = QString::number(m_Samprate).toUtf8();

  cmd_ln_t *config = cmd_ln_init(NULL, ps_args(), TRUE,
                               "-hmm", model.data(),
                               (!grammar.isEmpty()) ? "-jsgf" : "-lm", (!grammar.isEmpty()) ? grammar.data() : lm.data(),
                               "-dict", dict.data(),
                               "-samprate", samprate.data(),
                               NULL);
  return config;
}

