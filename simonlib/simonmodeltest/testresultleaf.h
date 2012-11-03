/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_TESTRESULTLEAF_H_8163A007180343A3A027C402EDCE6C93
#define SIMON_TESTRESULTLEAF_H_8163A007180343A3A027C402EDCE6C93

#include "simonmodeltest_export.h"
#include <QString>

class MODELTEST_EXPORT TestResultLeaf
{
  private:
    QString m_label;
    QString m_originalLabel;
    QString m_pronunciation;
    float m_accuracy;
    float m_confidence;
    bool m_insertionError, m_substitutionError, m_deletionError;
    void calcAccuracy();

  public:
    TestResultLeaf(const QString& label, const QString& pronunciation, float confidence);
    TestResultLeaf();

    void setInsertionError(bool f);
    void setSubstitutionError(bool f, const QString&);
    void setDeletionError(bool f);

    void setOriginalLabel(const QString& originalLabel);

    int wordErrorRate() const;
    float wordErrorRateF() const;
    bool correct() const;
    bool insertionError() const { return m_insertionError; }
    bool substitutionError() const { return m_substitutionError; }
    bool deletionError() const { return m_deletionError; }

    float accuracy() const { return m_accuracy; }
    float confidence() const { return m_confidence; }
    QString label() const { return m_label; }
    QString prettyLabel() const;
    QString originalLabel() const { return m_originalLabel; }
    QString prompt() const { return m_pronunciation; }

};
#endif

