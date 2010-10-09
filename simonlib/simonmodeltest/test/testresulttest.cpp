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

#include "../testresult.h"
#include "../testresultinstance.h"
#include "../testresultleaf.h"

#include <simonrecognitionresult/recognitionresult.h>

#include <QtTest>
#include <QtCore>
#include <QList>

#include <QDebug>

class testTestResult: public QObject
{
  Q_OBJECT
  private slots:
    void testErrorParsing();
    void testErrorParsing_data();

    void testMultiple();
    void testMultiple_data();
};

void testTestResult::testErrorParsing_data()
{
  QList<float> confidenceScores;
  confidenceScores << 1.0 << 0.7 << 0.3 << 0.9;
  QString sampa = "t h i s | i s | ax | t E s t";
  RecognitionResult result("This is a Test", sampa, sampa, confidenceScores);

  QTest::addColumn<RecognitionResult>("recognitionResult");
  //QTest::addColumn<TestResult*>("testResult");
  QTest::addColumn<QString>("label");
  QTest::addColumn<int>("insertionErrors");
  QTest::addColumn<int>("deletionErrors");
  QTest::addColumn<int>("substitutionErrors");

  QTest::newRow("correct") << result << "This is a Test" << 0 << 0 << 0;
  QTest::newRow("del1") << result << "This is test a Test" << 0 << 1 << 0;
  QTest::newRow("del2") << result << "This is the nice a Test" << 0 << 2 << 0;
  QTest::newRow("del3") << result << "This the is nice a Test" << 0 << 2 << 0;
  QTest::newRow("sub1") << result << "This is no Test" << 0 << 0 << 1;
  QTest::newRow("sub2") << result << "This aint no Test" << 0 << 0 << 2;
  QTest::newRow("sub3") << result << "That is no Test" << 0 << 0 << 2;
  QTest::newRow("subins") << result << "That is Test" << 1 << 0 << 1;
  QTest::newRow("ins1") << result << "This is Test" << 1 << 0 << 0;
  QTest::newRow("ins2") << result << "This Test" << 2 << 0 << 0;
  QTest::newRow("ins3") << result << "is Test" << 2 << 0 << 0;
  QTest::newRow("rubbish") << result << "Something completely different" << 1 << 0 << 3;
  QTest::newRow("almostrubbish") << result << "Something completely different that is longer" << 1 << 3 << 2;
  QTest::newRow("rubbishlonger") << result << "Something completely different that isdif longer" << 0 << 2 << 4;
}

void testTestResult::testErrorParsing()
{
  QFETCH(RecognitionResult, recognitionResult);
  QFETCH(QString, label);
  QFETCH(int, insertionErrors);
  QFETCH(int, deletionErrors);
  QFETCH(int, substitutionErrors);

  QList<TestResultLeaf*> leafs = TestResultInstance::parseResult(recognitionResult);

  TestResult *testResult = new TestResult(label);
  bool succ = testResult->registerChildren(leafs);
  QVERIFY(succ);

  qDebug() << "TestResult: I: " << testResult->insertionErrors() << " S: " << 
    testResult->substitutionErrors() << " D: " << testResult->deletionErrors();

  QCOMPARE(testResult->insertionErrors(), insertionErrors);
  QCOMPARE(testResult->deletionErrors(), deletionErrors);
  QCOMPARE(testResult->substitutionErrors(), substitutionErrors);

  delete testResult;
  qDeleteAll(leafs);
}




void testTestResult::testMultiple_data()
{
  QString expectedResult = "This is a Test";
  QTest::addColumn<RecognitionResultList>("recognitionResults");
  QTest::addColumn<QString>("label");
  QTest::addColumn<int>("insertionErrors");
  QTest::addColumn<int>("deletionErrors");
  QTest::addColumn<int>("substitutionErrors");

  QList<float> confidenceScores;
  confidenceScores << 1.0 << 0.7 << 0.3 << 0.9;
  QString sampa = "t h i s | i s | ax | t E s t";
  RecognitionResult correctResult("THIS IS A TEST", sampa, sampa, confidenceScores);

  QTest::newRow("correct") << (RecognitionResultList() << correctResult << correctResult << correctResult) << 
    expectedResult << 0 << 0 << 0;

  QTest::newRow("empty") << RecognitionResultList() << 
    expectedResult << 0 << 0 << 0;

  QList<float> confidenceScores2;
  confidenceScores2 << 1.0 << 0.7 << 0.1 << 0.3 << 0.9;
  QString sampa2 = "t h i s | i s | w oy | ax | t E s t";
  RecognitionResult ins1Result("This is wow a Test", sampa2, sampa2, confidenceScores2);
  QTest::newRow("ins1") << (RecognitionResultList() << correctResult << ins1Result) << 
    expectedResult << 1 << 0 << 0;

  QList<float> confidenceScores3;
  confidenceScores3 << 0.4 << 0.2 << 0.8 << 1.0;
  QString sampa3 = "t h i s | i s | n o: | t E s t";
  RecognitionResult sub1Result("This is no Test", sampa3, sampa3, confidenceScores3);

  QList<float> confidenceScores4;
  confidenceScores4 << 0.4 << 0.2 << 0.8 << 1.0 << 0.2 << 0.5;
  QString sampa4 = "s o: m ey th i n g | c o m p l i t l y: | d i f ax r ay n t | t h a: t | ey s d f | l o: n g ax";
  RecognitionResult rubbishResult("Something completely different that isdif longer", sampa4, sampa4, confidenceScores4);

  QTest::newRow("rubbish") << (RecognitionResultList() << rubbishResult) << 
    expectedResult << 2 << 0 << 4;

  QList<float> confidenceScores5;
  confidenceScores5 << 0.4 << 0.2;
  QString sampa5 = "i s | n o:";
  RecognitionResult del1Result("is no", sampa5, sampa5, confidenceScores5);

  QTest::newRow("advanced") << (RecognitionResultList() << del1Result << ins1Result << correctResult << sub1Result
      << rubbishResult) << expectedResult << 3 << 2 << 6;

}

void testTestResult::testMultiple()
{
  QFETCH(RecognitionResultList, recognitionResults);
  QFETCH(QString, label);
  QFETCH(int, insertionErrors);
  QFETCH(int, deletionErrors);
  QFETCH(int, substitutionErrors);

  TestResult *testResult = new TestResult(label);
  bool succ = true;
  foreach (const RecognitionResult& result, recognitionResults)
  {
    QList<TestResultLeaf*> leafs = TestResultInstance::parseResult(result);
    succ = testResult->registerChildren(leafs) && succ;
  }

  QVERIFY(succ);
  qDebug() << "TestResult: I: " << testResult->insertionErrors() << " S: " << 
    testResult->substitutionErrors() << " D: " << testResult->deletionErrors();

  QCOMPARE(testResult->insertionErrors(), insertionErrors);
  QCOMPARE(testResult->deletionErrors(), deletionErrors);
  QCOMPARE(testResult->substitutionErrors(), substitutionErrors);

  testResult->deleteAll();
  delete testResult;
}

QTEST_MAIN(testTestResult)

#include "testresulttest.moc"
