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


#ifndef JULIUSRECOGNITIONCONFIGURATION_H
#define JULIUSRECOGNITIONCONFIGURATION_H

#include "recognitionconfiguration.h"
#include <QString>
#include "simonrecognizer_export.h"

class SIMONRECOGNIZER_EXPORT JuliusRecognitionConfiguration : public RecognitionConfiguration
{
private:
  QString m_jconf;
  QString m_gram;
  QString m_hmmDefs;
  QString m_tiedlist;
  
public:
  JuliusRecognitionConfiguration(const QString& jconf, const QString& gram, const QString& hmmDefs, const QString& tiedlist);
  QString getJconf() { return m_jconf; }
  QString getGram() { return m_gram; }
  QString getHmmDefs() { return m_hmmDefs; }
  QString getTiedlist() { return m_tiedlist; }
    
};

#endif // JULIUSRECOGNITIONCONFIGURATION_H
