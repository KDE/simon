/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "testresultleaf.h"
#include <KLocale>

TestResultLeaf::TestResultLeaf(const QString& label, const QString& pronunciation, float confidence) :
  m_label(label), m_originalLabel(label), m_pronunciation(pronunciation), m_accuracy(1.0f), m_confidence(confidence),
  m_insertionError(false), m_substitutionError(false), m_deletionError(false)
{
}

void TestResultLeaf::setInsertionError(bool f)
{
  m_insertionError = f;
  calcAccuracy();
  m_originalLabel.clear();
}

void TestResultLeaf::setSubstitutionError(bool f, const QString& originalLabel)
{
  m_substitutionError = f;
  m_originalLabel = originalLabel;
  calcAccuracy();
}

void TestResultLeaf::setDeletionError(bool f)
{
  m_deletionError = f;
  calcAccuracy();
}

bool TestResultLeaf::correct()
{
  return !m_deletionError && !m_substitutionError && !m_insertionError;
}

float TestResultLeaf::wordErrorRateF() const
{
  return wordErrorRate();
}

int TestResultLeaf::wordErrorRate() const
{
  int errorRate = 0;
  if (insertionError()) ++errorRate;
  if (substitutionError()) ++errorRate;
  if (deletionError()) ++errorRate;
  return errorRate;
}

QString TestResultLeaf::prettyLabel() const
{
  if (insertionError())
    return i18nc("Pretty label showing that the current leaf is not present in the transcription", "%1 (mistakenly inserted)", m_label);
  if (deletionError())
    return i18nc("Pretty label showing that a word from the transcription is missing", "(missing: %1)", m_originalLabel);
  if (substitutionError())
    return i18nc("Pretty label showing the user that the transcription and the result mismatch", "%1 (correct: %2)", m_label, m_originalLabel);
  return m_label;
}

void TestResultLeaf::calcAccuracy()
{
  m_accuracy = correct() ? 1.0f : 0.0f;
}

TestResultLeaf::TestResultLeaf() : m_accuracy(0.0f), m_confidence(0.0f), m_insertionError(false), m_substitutionError(false),
  m_deletionError(false)
{
}

