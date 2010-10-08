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

#ifndef SIMON_REPORTPARAMETERS_H_4002119636CC42C68FE07273F9000A73
#define SIMON_REPORTPARAMETERS_H_4002119636CC42C68FE07273F9000A73

#include <QFlags>
#include <QString>
#include <QDate>

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
    QString m_trainingTag;
    QString m_trainingNotes;
    int m_trainingSpeakers;
    int m_trainingSamples;
    QString m_developmentTag;
    QString m_developmentNotes;
    int m_developmentSpeakers;
    int m_developmentSamples;
    QString m_testTag;
    QString m_testNotes;
    int m_testSpeakers;
    int m_testSamples;

  public:
    ReportParameters(const QString& title, const QString& tag,
      const QString& taskDefinition,const OutputOptions options, 
      const QString& outputTemplate, const QString& conclusion, 
      const QString& experimentTag, const QDate& experimentDate,
      const QString& experimentDescription, const QString& systemTag,
      const QString& systemDefinition, const QString& vocabularyTag,
      const QString& vocabularyNotes, const QString& grammarTag,
      const QString& grammarNotes, const QString& trainingTag,
      const QString& trainingNotes, int trainingSpeakers, int trainingSamples,
      const QString& developmentTag,
      const QString& developmentNotes, int developmentSpeakers, 
      int developmentSamples,
      const QString& testTag, const QString& testNotes, 
      int testSpeakers, int testSamples);

    QString title() { return m_title; }
    QString tag() { return m_tag; }
    QString taskDefinition() { return m_taskDefinition; }
    QString systemDefinition() { return m_systemDefinition; }
    QString outputTemplate() { return m_outputTemplate; }
    QString conclusion() { return m_conclusion; }
    QString vocabularyNotes() { return m_vocabularyNotes; }
    QString vocabularyTag() { return m_vocabularyTag; }
    QString grammarTag() { return m_grammarTag; }
    QString grammarNotes() { return m_grammarNotes; }
    QString trainingTag() { return m_trainingTag; }
    QString trainingNotes() { return m_trainingNotes; }
    QString developmentTag() { return m_developmentTag; }
    QString developmentNotes() { return m_developmentNotes; }
    QString testTag() { return m_testTag; }
    QString testNotes() { return m_testNotes; }
    OutputOptions options() { return m_options; }

    QString experimentTag() { return m_experimentTag; }
    QDate experimentDate() { return m_experimentDate; }
    QString experimentDescription() { return m_experimentDescription; }
    QString systemTag() { return m_systemTag; }

    int trainingSpeakers() { return m_trainingSpeakers; }
    int trainingSamples() { return m_trainingSamples; }
    int developmentSpeakers() { return m_developmentSpeakers; }
    int developmentSamples() { return m_developmentSamples; }
    int testSpeakers() { return m_testSpeakers; }
    int testSamples() { return m_testSamples; }
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ReportParameters::OutputOptions);

#endif

