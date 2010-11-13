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
#include "samxmlhelper.h"

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

ReportParameters* ReportParameters::deSerialize(const QDomElement& elem)
{
  QString title = SamXMLHelper::getText(elem, "title");
  QString tag = SamXMLHelper::getText(elem, "tag");
  QString taskDefinition = SamXMLHelper::getText(elem, "taskDefinition");
  QString outputTemplate = SamXMLHelper::getText(elem, "outputTemplate");
  QString conclusion = SamXMLHelper::getText(elem, "conclusion");
  QString experimentTag = SamXMLHelper::getText(elem, "experimentTag");
  QDate experimentDate = QDate::fromString(SamXMLHelper::getText(elem, "experimentDate"), Qt::ISODate);
  QString experimentDescription = SamXMLHelper::getText(elem, "experimentDescription");
  QString systemTag = SamXMLHelper::getText(elem, "systemTag");
  QString systemDefinition = SamXMLHelper::getText(elem, "systemDefinition");
  QString vocabularyTag = SamXMLHelper::getText(elem, "vocabularyTag");
  QString vocabularyNotes = SamXMLHelper::getText(elem, "vocabularyNotes");
  QString grammarTag = SamXMLHelper::getText(elem, "grammarTag");
  QString grammarNotes = SamXMLHelper::getText(elem, "grammarNotes");

  int wordCount = SamXMLHelper::getInt(elem, "wordCount");
  int pronunciationCount = SamXMLHelper::getInt(elem, "pronunciationCount");

  bool ok = true;
  ReportParameters::OutputOptions options = (ReportParameters::OutputOptions) SamXMLHelper::getInt(elem, "options");

  if (ok)
    return new ReportParameters(title, tag,
        taskDefinition, options, outputTemplate,
        conclusion, experimentTag, experimentDate, experimentDescription,
        systemTag, systemDefinition, vocabularyTag, vocabularyNotes, grammarTag,
        grammarNotes, wordCount, pronunciationCount
        );
  
  return 0;
}

QDomElement ReportParameters::serialize(QDomDocument* doc)
{
  QDomElement elem = doc->createElement("reportParameters");
  
  SamXMLHelper::serializeText(doc, elem, title(), "title");
  SamXMLHelper::serializeText(doc, elem, tag(), "tag");
  SamXMLHelper::serializeText(doc, elem, taskDefinition(), "taskDefinition");
  SamXMLHelper::serializeText(doc, elem, outputTemplate(), "outputTemplate");
  SamXMLHelper::serializeText(doc, elem, conclusion(), "conclusion");
  SamXMLHelper::serializeText(doc, elem, experimentTag(), "experimentTag");
  SamXMLHelper::serializeText(doc, elem, experimentDate().toString(Qt::ISODate), "experimentDate");
  
  SamXMLHelper::serializeText(doc, elem, experimentDescription(), "experimentDescription");
  SamXMLHelper::serializeText(doc, elem, systemTag(), "systemTag");
  SamXMLHelper::serializeText(doc, elem, systemDefinition(), "systemDefinition");
  SamXMLHelper::serializeText(doc, elem, vocabularyTag(), "vocabularyTag");
  SamXMLHelper::serializeText(doc, elem, vocabularyNotes(), "vocabularyNotes");
  SamXMLHelper::serializeText(doc, elem, grammarTag(), "grammarTag");
  SamXMLHelper::serializeText(doc, elem, grammarNotes(), "grammarNotes");
  
  SamXMLHelper::serializeInt(doc, elem, wordCount(), "wordCount");
  SamXMLHelper::serializeInt(doc, elem, pronunciationCount(), "pronunciationCount");
  SamXMLHelper::serializeInt(doc, elem, options(), "options");
  
  return elem;
}
