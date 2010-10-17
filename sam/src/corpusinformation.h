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

#ifndef SIMON_CORPUSINFORMATION_H_4002119636CC42C68FE07273F9000A73
#define SIMON_CORPUSINFORMATION_H_4002119636CC42C68FE07273F9000A73

#include <QString>

class CorpusInformation
{
  private:
    QString m_tag;
    QString m_notes;
    int m_speakers;
    int m_samples;

    //optional fields
    int m_samplesTotal;
    
  public:
    CorpusInformation() : m_speakers(0), m_samples(0) {}
    CorpusInformation(const QString& tag, const QString& notes, int speakers, int samples,
        int samplesTotal) :
      m_tag(tag), m_notes(notes), m_speakers(speakers), m_samples(samples), 
      m_samplesTotal(samplesTotal)
    {}

    QString tag() { return m_tag; }
    QString notes() { return m_notes; }
    int speakers() { return m_speakers; }
    int samples() { return m_samples; }

    int samplesTotal() { return m_samplesTotal; }
    //float averagePronunciationsPerWord() { return ((float) m_pronunciationCount) / ((float) m_wordCount); }

    void setTag(const QString& tag) { m_tag = tag; }
    void setNotes(const QString& notes) { m_notes = notes; }
    void setSpeakers(int speakers) { m_speakers = speakers; }
    void setSamples(int samples) { m_samples = samples; }

    void setTotalSampleCount(int count) { m_samplesTotal = count; }
};

#endif

