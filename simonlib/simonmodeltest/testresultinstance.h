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

#ifndef SIMON_SENTENCEINSTANCETESTRESULT_H_8163A007180343A3A027C402EDCE6C93
#define SIMON_SENTENCEINSTANCETESTRESULT_H_8163A007180343A3A027C402EDCE6C93

#include "simonmodeltest_export.h"
#include "testresult.h"
#include <QList>
#include <QString>
#include <QMetaType>

class TestResultLeaf;
class RecognitionResult;

class MODELTEST_EXPORT TestResultInstance 
{
  private:
    QList<TestResultLeaf*> m_children;

    void advanceToNextValidResultAfterSkipping(int skippedCount, QStringList& labels);
    TestResultInstance();

  public:
    static QList<TestResultLeaf*> parseResult(const RecognitionResult& result);

    static TestResultInstance* createInstance(const QString& label, const QList<TestResultLeaf*>& children);

    bool registerChildren(const QString& label, const QList<TestResultLeaf*>& children);

    float accuracy();
    float wordErrorRate();
    int insertionErrors();
    int deletionErrors();
    int substitutionErrors();
    bool correct(const QString& label);

    void deleteAll();
};

Q_DECLARE_METATYPE(TestResultInstance*);

#endif


