/*
 *   Copyright (C) 2009-2011 Peter Grasch <grasch@simon-listens.org>
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

#include "recognitionresult.h"
#include <QObject>

QString RecognitionResult::toString() const
{
  QString out;
  out += QObject::tr("Sentence: %1\n"
    "SAMPA: %2\n"
    "Raw SAMPA: %3\n"
    "Average Confidence: %4\n").arg(
    m_sentence).arg(m_sampa).arg(m_sampaRaw).arg(
    averageConfidenceScore()*100.0f);

  out += QObject::tr("Confidence Scores: ");
  foreach (float score, m_confidenceScores) {
    out += QString::number(score*100.0f)+' ';
  }

  out += '\n';
  return out;
}


float RecognitionResult::averageConfidenceScore() const
{
  float avg=0;
  foreach (float score, m_confidenceScores)
    avg += score;
  avg /= ((float) m_confidenceScores.count());
  return avg;
}


bool RecognitionResult::matchesTrigger(const QString& trigger)
{
  if (trigger.isEmpty()) return true;
  return m_sentence.contains(QRegExp('^'+trigger+"( |$)"));
}


void RecognitionResult::removeTrigger(const QString& trigger)
{
  if (!trigger.isEmpty())
    m_sentence.remove(QRegExp('^'+trigger+"( |$)"));
}
