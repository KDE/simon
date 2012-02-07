/*
 *   Copyright (C) 2012 Peter Grasch <grasch@simon-listens.org>
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


#include "juliusstaticrecognitionconfiguration.h"

JuliusStaticRecognitionConfiguration::JuliusStaticRecognitionConfiguration(const QString& jconf, const QString& dfa, const QString& dict, const QString& hmmDefs, const QString& tiedlist, const QString& sampleRate): 
  m_jconf(jconf),
  m_dfa(dfa),
  m_dict(dict),
  m_hmmDefs(hmmDefs),
  m_tiedlist(tiedlist),
  m_sampleRate(sampleRate)
{
}

QStringList JuliusStaticRecognitionConfiguration::toArgs()
{
  QStringList args;
  args << "-C" << getJconf() 
       << "-dfa" << getDfa() 
       << "-v" << getDict()
       << "-h" << getHmmDefs()
       << "-hlist" << getTiedlist()
       << "-input" << "rawfile"
       << "-smpFreq" << getSampleRate();
  return args;
}
