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

#ifndef SIMON_TESTRESULTINSTANCE_H_8163A007180343A3A027C402EDCE6C93
#define SIMON_TESTRESULTINSTANCE_H_8163A007180343A3A027C402EDCE6C93

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

    TestResultInstance();

  public:
    static QList<TestResultLeaf*> parseResult(const RecognitionResult& result);

    static TestResultInstance* createInstance(const QList<TestResultLeaf*>& children);

    void registerChildren(const QList<TestResultLeaf*>& children);

    float accuracy() const;
    float confidence() const;
    float wordErrorRate() const;
    int insertionErrors() const;
    int deletionErrors() const;
    int substitutionErrors() const;
    bool correct() const;
    QString recognizedText() const;

    void deleteAll();
};

Q_DECLARE_METATYPE(TestResultInstance*);

#endif


