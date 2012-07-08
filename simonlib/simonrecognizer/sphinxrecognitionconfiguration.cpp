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

QStringList SphinxRecognitionConfiguration::toArgs()
{
  QStringList args;
  args << "-hmm " << m_ModelDir
       << "-jsgf " << m_Grammar
       << "-dict " << m_Dictionary;
//       << "-input" << "file";
  return args;
}

QSharedPointer<cmd_ln_t> SphinxRecognitionConfiguration::getSphinxConfig() //WARNING: move to recognizer?
{
  QByteArray model = m_ModelDir.toUtf8();
  QByteArray grammar = m_Grammar.toUtf8();
  QByteArray dict = m_Dictionary.toUtf8();

  QSharedPointer<cmd_ln_t> config ;/*= QSharedPointer<cmd_ln_t>(cmd_ln_init(NULL, ps_args(), TRUE,
                               "-hmm", model.data(),
                               "-jsgf", grammar.data(),
                               "-dict", dict.data(),
                               NULL));*/
  return config;
}

