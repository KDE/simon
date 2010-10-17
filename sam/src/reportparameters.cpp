/*
 *  Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "reportparameters.h"

ReportParameters::ReportParameters(const QString& title, const QString& tag,
      const QString& taskDefinition,const OutputOptions options, 
      const QString& outputTemplate, const QString& conclusion, 
      const QString& experimentTag, const QDate& experimentDate,
      const QString& experimentDescription, const QString& systemTag,
      const QString& systemDefinition, const QString& vocabularyTag,
      const QString& vocabularyNotes, const QString& grammarTag,
      const QString& grammarNotes,
      int wordCount, int pronunciationCount
      ) :
  m_title(title), m_tag(tag), m_taskDefinition(taskDefinition),
  m_options(options),  m_outputTemplate(outputTemplate), 
  m_conclusion(conclusion), m_experimentTag(experimentTag),
  m_experimentDate(experimentDate), m_experimentDescription(experimentDescription),
  m_systemTag(systemTag), m_systemDefinition(systemDefinition), 
  m_vocabularyTag(vocabularyTag), m_vocabularyNotes(vocabularyNotes), 
  m_grammarTag(grammarTag), m_grammarNotes(grammarNotes),
  m_wordCount(wordCount), m_pronunciationCount(pronunciationCount)
{
}

ReportParameters::~ReportParameters()
{
}
