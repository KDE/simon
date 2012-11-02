/*
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
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


#ifndef JULIUSSTATICRECOGNITIONCONFIGURATION_H
#define JULIUSSTATICRECOGNITIONCONFIGURATION_H

#include "recognitionconfiguration.h"
#include "simonrecognizer_export.h"

class SIMONRECOGNIZER_EXPORT JuliusStaticRecognitionConfiguration : public RecognitionConfiguration
{
public:
  JuliusStaticRecognitionConfiguration(const QString& jconf, const QString& dfa, const QString& dict, const QString& hmmDefs, const QString& tiedlist, const QString& sampleRate);
  QString getJconf() { return m_jconf; }
  QString getDfa() { return m_dfa; }
  QString getDict() { return m_dict; }
  QString getHmmDefs() { return m_hmmDefs; }
  QString getTiedlist() { return m_tiedlist; }
  QString getSampleRate() { return m_sampleRate; }
  
  QStringList toArgs();
  
private:
  QString m_jconf;
  QString m_dfa;
  QString m_dict;
  QString m_hmmDefs;
  QString m_tiedlist;
  QString m_sampleRate;
};

#endif // JULIUSSTATICRECOGNITIONCONFIGURATION_H
