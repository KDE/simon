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
//#include <QDebug>

TestResultInstance::TestResultInstance()
{
}

TestResultInstance* TestResultInstance::createInstance(const QString& label, const QList<TestResultLeaf*>& children)
{
  TestResultInstance *t = new TestResultInstance();
  if (!t->registerChildren(label, children))
  {
    delete t;
    t = 0;
  }
  return t;
}

bool TestResultInstance::registerChildren(const QString& label, 
    const QList<TestResultLeaf*>& children)
{
  QStringList labels = label.split(" ", QString::SkipEmptyParts);
  m_children.clear();
  for (int i=0; i < children.count(); i++)
  {
    TestResultLeaf *t = children[i];
    //qDebug() <<"Labels: " << labels;
    //qDebug() <<"Analyzing: " << children[i]->label();
    if (labels.isEmpty())
    {
      t->setInsertionError(true);
      //qDebug() << "No labels left for: " << t->label();
    } else {
      bool found = false;
      int j = 0;
      for (; j < labels.count(); j++)
      {
        if (labels[j] == t->label())
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
        labels.removeAt(0);
        advanceToNextValidResultAfterSkipping(j, labels);
      }
    }
    
    m_children << t;
  }

  //processing leftover tokens
  advanceToNextValidResultAfterSkipping(labels.count(), labels);
  return true;
}

void TestResultInstance::advanceToNextValidResultAfterSkipping(int skippedCount, QStringList& labels)
{
  //before adding them as new deletion errors try to redeem any
  //incorrectly as insertion errors regarded tokens
  while (skippedCount > 0)
  {
    bool foundRedeemableChild = false;
    int l;
    for (l = m_children.count() -1; l >= 0; l--)
    {
      if (m_children[l]->insertionError())
        foundRedeemableChild = true;
      else
        break;
    }
    if (foundRedeemableChild) 
    {
      m_children[++l]->setInsertionError(false);
      m_children[l]->setSubstitutionError(true);
      //qDebug() << "Switching child to substitution: " << m_children[l]->label();
      labels.removeAt(0);
      --skippedCount;
    } else 
     break;
  }

  for (int k=0; ((k < skippedCount) && labels.count()); k++)
  {
    TestResultLeaf *l = new TestResultLeaf();
    //qDebug() << "Deletion error";
    l->setDeletionError(true);
    m_children << l;
    labels.removeAt(0);
  }
}

float TestResultInstance::accuracy()
{
  float acc = 0.0;
  foreach (TestResultLeaf *t, m_children)
    acc += t->accuracy();
  acc /= (float) m_children.count();
  return acc;
}

float TestResultInstance::wordErrorRate()
{
  return ((float) (insertionErrors()+deletionErrors()+substitutionErrors())) / m_children.count();
}

int TestResultInstance::insertionErrors()
{
  int insertionErrors = 0;
  foreach (TestResultLeaf *t, m_children)
    insertionErrors += t->insertionError();
  return insertionErrors;
}

int TestResultInstance::deletionErrors()
{
  int deletionErrors = 0;
  foreach (TestResultLeaf *t, m_children)
    deletionErrors += t->deletionError();
  return deletionErrors;
}

int TestResultInstance::substitutionErrors()
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

  Q_ASSERT(pronunciations.count() == words.count());
  Q_ASSERT(words.count() == scores.count());

  QList<TestResultLeaf*> leafs;
  for (int i=0; i < words.count(); i++)
    leafs << new TestResultLeaf(words[i], pronunciations[i], scores[i]);
  return leafs;
}

bool TestResultInstance::correct(const QString& label)
{
  QString recognizedSentence;
  
  foreach (TestResultLeaf *t, m_children)
    recognizedSentence += t->label()+' ';

  return (label == recognizedSentence.trimmed());
}



