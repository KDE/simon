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

#include "testresultinstance.h"
#include "testresultleaf.h"
#include <simonrecognitionresult/recognitionresult.h>
#include <QDebug>
#include <KDebug>

TestResultInstance::TestResultInstance()
{
}

TestResultInstance* TestResultInstance::createInstance(const QList<TestResultLeaf*>& children)
{
  TestResultInstance *t = new TestResultInstance();
  t->registerChildren(children);
  return t;
}

void TestResultInstance::registerChildren(const QList<TestResultLeaf*>& children)
{
  m_children = children;
}

float TestResultInstance::accuracy() const
{
  float acc = 0.0;
  foreach (TestResultLeaf *t, m_children)
    acc += t->accuracy();
  acc /= (float) m_children.count();
  return acc;
}

float TestResultInstance::confidence() const
{
  float conf = 0.0;
  foreach (TestResultLeaf *t, m_children)
    conf += t->confidence();
  conf /= (float) m_children.count();
  return conf;
}

float TestResultInstance::wordErrorRate() const
{
  float rate = 0.0;
  foreach (TestResultLeaf *t, m_children)
    rate += t->wordErrorRate();
  return rate / ((float) m_children.count());
  //return ((float) (insertionErrors()+deletionErrors()+substitutionErrors())) / m_children.count();
}

int TestResultInstance::insertionErrors() const
{
  int insertionErrors = 0;
  foreach (TestResultLeaf *t, m_children)
    insertionErrors += t->insertionError();
  return insertionErrors;
}

int TestResultInstance::deletionErrors() const
{
  int deletionErrors = 0;
  foreach (TestResultLeaf *t, m_children)
    deletionErrors += t->deletionError();
  return deletionErrors;
}

int TestResultInstance::substitutionErrors() const
{
  int substitationErrors = 0;
  foreach (TestResultLeaf *t, m_children)
    substitationErrors += t->substitutionError();
  return substitationErrors;
}

void TestResultInstance::deleteAll()
{
  qDeleteAll(m_children);
  m_children.clear();
}

QList<TestResultLeaf*> TestResultInstance::parseResult(const RecognitionResult& result)
{
  QStringList words = result.words();
  QStringList pronunciations = result.sampas();
  QList<float> scores = result.confidenceScores();

  pronunciations.removeAll(" sil");
  pronunciations.removeAll("sil");
  Q_ASSERT(pronunciations.count() == words.count());
  Q_ASSERT(words.count() == scores.count());

  QList<TestResultLeaf*> leaves;
  for (int i=0; i < words.count(); i++)
    leaves << new TestResultLeaf(words[i], pronunciations[i].trimmed(), scores[i]);

  return leaves;
}

bool TestResultInstance::correct() const
{
  return (accuracy() == 1.0f);
}

QString TestResultInstance::recognizedText() const
{
  QString recognizedSentence;
  
  foreach (TestResultLeaf *t, m_children)
    recognizedSentence += t->prettyLabel()+' ';

  return recognizedSentence.trimmed();
}

TestResult::~TestResult()
{
  qDeleteAll(m_children);
}


