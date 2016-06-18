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

#ifndef SIMON_SENTENCETESTRESULT_H_8163A007180343A3A027C402EDCE6C93
#define SIMON_SENTENCETESTRESULT_H_8163A007180343A3A027C402EDCE6C93

#include "simonmodeltest_export.h"
#include "testresult.h"
#include <QList>
#include <QString>
#include <QMetaType>

class TestResultInstance;
class TestResultLeaf;

class SIMONMODELTEST_EXPORT TestResult
{
  private:
    QList<TestResultInstance*> m_children;
    QString m_label; //expected result
    int m_wordCount;

    static int advanceToNextValidResultAfterSkipping(int lastGood, int skippedCount, QStringList& labels, QList<TestResultLeaf*>& children);

  public:
    TestResult(const QString& label);
    bool registerChild(TestResultLeaf* child);
    bool registerChildren(const QList<TestResultLeaf*>& children);

    static void parseChildren(const QString& label, QList<TestResultLeaf*>& children);

    float accuracy() const;
    float confidence() const;
    float wordErrorRate() const;
    int insertionErrors() const;
    int deletionErrors() const;
    int substitutionErrors() const;
    int count() const;
    int correctCount() const;
    int incorrectCount() const;

    void deleteAll();

    bool matchesLabel(const QString& label) const;
    QString label() const { return m_label; }
    QString recognitionResults() const;
    QString prettyLabel();
    ~TestResult();
};

Q_DECLARE_METATYPE(TestResult*)

#endif

