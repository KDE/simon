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

#include "testresult.h"
#include "testresultinstance.h"
#include "testresultleaf.h"
#include <simonrecognitionresult/recognitionresult.h>
#include <KDebug>

TestResult::TestResult(const QString& label) : m_label(label), m_wordCount(label.count(' ') + 1)
{
}

void TestResult::parseChildren(const QString& label, QList<TestResultLeaf*>& children)
{
  QStringList labels = label.split(' ', QString::SkipEmptyParts);
  int lastGood = 0;
  for (int i=0; i < children.count(); i++)
  {
    TestResultLeaf *t = children[i];
    if (labels.isEmpty())
    {
      t->setInsertionError(true);
      //kDebug() << "No labels left for: " << t->label();
    } else {
      bool found = false;
      int j = 0;
      for (; j < labels.count(); j++)
      {
        if (labels[j].compare(t->label(), Qt::CaseInsensitive) == 0)
        {
          found = true;
          break;
        }
      }
      //qDebug() << "Found: " << found << j;
      if (!found)
      {
        //qDebug() << "Insertion error";
        t->setInsertionError(true);
      }
      else
      {
        //if j != 0 there were other labels that are missing in the result
        labels.removeAt(j);
        advanceToNextValidResultAfterSkipping(lastGood, j, labels, children);
        lastGood = i;
      }
    }
  }

  //processing leftover tokens
  advanceToNextValidResultAfterSkipping(lastGood, labels.count(), labels, children);
}

void TestResult::advanceToNextValidResultAfterSkipping(int lastGood, int skippedCount, QStringList& labels, QList<TestResultLeaf*>& children)
{
  //before adding them as new deletion errors try to redeem any
  //incorrectly as insertion errors regarded tokens
  //kDebug() << "Skipped" << skippedCount << " Last good: " << lastGood << " children count: " << children.count();
  int l = children.count() - 1;
  for (; ((l >= lastGood) && (skippedCount > 0)); l--)
  {
    if (children[l]->insertionError()) {
      children[l]->setInsertionError(false);
      children[l]->setSubstitutionError(true, labels[0]);
      //kDebug() << "Switching child to substitution: " << children[l]->label() << " for " << labels[0];
      labels.removeAt(0);
      --skippedCount;
    }
  }

  for (int k=0; ((k < skippedCount) && labels.count()); k++)
  {
    TestResultLeaf *l = new TestResultLeaf();
    //kDebug() << "Deletion error " << labels[0];
    l->setDeletionError(true);
    l->setOriginalLabel(labels.takeAt(0));
    children << l;
  }
}

bool TestResult::registerChild(TestResultLeaf* child)
{
  return registerChildren(QList<TestResultLeaf*>() << child);
}

bool TestResult::registerChildren(const QList<TestResultLeaf*>& children)
{
  TestResultInstance *instance = TestResultInstance::createInstance(children);
  if (instance) {
    m_children << instance;
    return true;
  }
  return false;
}

float TestResult::confidence() const
{
  float conf = 0.0;
  foreach (TestResultInstance *t, m_children)
    conf += t->confidence();
  conf /= (float) m_children.count();
  return conf;
}

float TestResult::accuracy() const
{
  float acc = 0.0;
  foreach (TestResultInstance *t, m_children)
    acc += t->accuracy();
  acc /= (float) m_children.count();
  return acc;
}

float TestResult::wordErrorRate() const
{
  return ((float) (insertionErrors()+deletionErrors()+substitutionErrors())) / (m_children.count() * m_wordCount);
}

int TestResult::insertionErrors() const
{
  int insertionErrors = 0;
  foreach (TestResultInstance *t, m_children)
    insertionErrors += t->insertionErrors();
  return insertionErrors;
}

int TestResult::deletionErrors() const
{
  int deletionErrors = 0;
  foreach (TestResultInstance *t, m_children)
    deletionErrors += t->deletionErrors();
  return deletionErrors;
}

int TestResult::substitutionErrors() const
{
  int substitutionErrors = 0;
  foreach (TestResultInstance *t, m_children)
    substitutionErrors += t->substitutionErrors();
  return substitutionErrors;
}

void TestResult::deleteAll()
{
  foreach (TestResultInstance *t, m_children)
    t->deleteAll();
  qDeleteAll(m_children);
  m_children.clear();
}

int TestResult::count() const
{
  return m_children.count();
}

int TestResult::correctCount() const
{
  int count = 0;
  foreach (TestResultInstance *t, m_children)
    if (t->correct())
      ++count;

  return count;
}

int TestResult::incorrectCount() const
{
  return count() - correctCount();
}

QString TestResult::prettyLabel()
{
  foreach (TestResultInstance *t, m_children)
    if (t->correct())
      return t->recognizedText();

  return label();
}

QString TestResult::recognitionResults() const
{
  QString results;
  foreach (TestResultInstance *t, m_children)
    results += t->recognizedText()+'\n';
  return results.trimmed();
}

bool TestResult::matchesLabel(const QString& label) const
{
  return (m_label.compare(label, Qt::CaseInsensitive) == 0);
}

