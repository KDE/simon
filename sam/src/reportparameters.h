/*
 *  Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_REPORTPARAMETERS_H_4002119636CC42C68FE07273F9000A73
#define SIMON_REPORTPARAMETERS_H_4002119636CC42C68FE07273F9000A73

#include <QString>
#include <QDate>
#include <QDomElement>

#include "corpusinformation.h"

class QDomDocument;

class ReportParameters
{
  public:
    enum OutputOption
    {
      None=0,
      Tables=1,
      Graphs=2
    };

    Q_DECLARE_FLAGS(OutputOptions, OutputOption);

  private:
    QString m_title;
    QString m_tag;
    QString m_taskDefinition;
    OutputOptions m_options;
    QString m_outputTemplate;
    QString m_conclusion;
    QString m_experimentTag;
    QDate m_experimentDate;
    QString m_experimentDescription;
    QString m_systemTag;
    QString m_systemDefinition;
    QString m_vocabularyTag;
    QString m_vocabularyNotes;
    QString m_grammarTag;
    QString m_grammarNotes;
    int m_wordCount;
    int m_pronunciationCount;
    

  public:
    ReportParameters() {}
    ReportParameters(const QString& title, const QString& tag,
      const QString& taskDefinition,const OutputOptions options, 
      const QString& outputTemplate, const QString& conclusion, 
      const QString& experimentTag, const QDate& experimentDate,
      const QString& experimentDescription, const QString& systemTag,
      const QString& systemDefinition, const QString& vocabularyTag,
      const QString& vocabularyNotes, const QString& grammarTag,
      const QString& grammarNotes,
      int wordCount, int pronunciationCount
      );

    QString title() const { return m_title; }
    QString tag() const { return m_tag; }
    QString taskDefinition() const { return m_taskDefinition; }
    QString systemDefinition() const { return m_systemDefinition; }
    QString outputTemplate() const { return m_outputTemplate; }
    QString conclusion() const { return m_conclusion; }
    QString vocabularyNotes() const { return m_vocabularyNotes; }
    QString vocabularyTag() const { return m_vocabularyTag; }
    QString grammarTag() const { return m_grammarTag; }
    QString grammarNotes() const { return m_grammarNotes; }
    OutputOptions options() const { return m_options; }

    QString experimentTag() const { return m_experimentTag; }
    QDate experimentDate() const { return m_experimentDate; }
    QString experimentDescription() const { return m_experimentDescription; }
    QString systemTag() const { return m_systemTag; }

    int wordCount() const { return m_wordCount; }
    int pronunciationCount() const { return m_pronunciationCount; }
    void setWordCount(int count) { m_wordCount = count; }
    void setPronunciationCount(int count) { m_pronunciationCount = count; }

    ~ReportParameters();
    
    QDomElement serialize(QDomDocument* doc);
    static ReportParameters* deSerialize(const QDomElement& elem);

    bool operator!=(const ReportParameters& other) const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ReportParameters::OutputOptions);

#endif

