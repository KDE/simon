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

#include <QTest>
#include <QDomDocument>
#include <QDomElement>
#include <QByteArray>

#include "../dialogboundvalues.h"
#include "../staticboundvalue.h"

class testDialogBoundValues: public QObject
{
  Q_OBJECT
  private slots:
    void testGeneral();
    void testAdd();
    void testGet();
    void testRemove();

  private:
    DialogBoundValues *values;
    BoundValue *addedValue;
};

void testDialogBoundValues::testGeneral()
{
  //error handling check
  values = DialogBoundValues::createInstance(QDomElement());
  QVERIFY(values); // always return instance
  delete values;

  QDomDocument doc;
  doc.setContent(
      QByteArray(
    "<boundValues>"
     "<boundValue type=\"1\">"
      "<name>time</name>"
      "<value content=\"Test\"/>"
     "</boundValue>"
     ));
  values = DialogBoundValues::createInstance(doc.documentElement());
  QVERIFY(values);


  QCOMPARE(values->rowCount(), 1);
  QCOMPARE(values->columnCount(), 3);

  QCOMPARE(values->data(values->index(0, 0), Qt::DisplayRole), QVariant("time"));
  QCOMPARE(values->data(values->index(0, 2), Qt::DisplayRole), QVariant("Test"));
}

void testDialogBoundValues::testAdd()
{
  addedValue = new StaticBoundValue("add", "added");
  bool ret = values->addBoundValue(addedValue);
  QCOMPARE(values->rowCount(), 2);
  QCOMPARE(ret, true);

  ret = values->addBoundValue(0);
  QCOMPARE(ret, false);

  QCOMPARE(values->rowCount(), 2);
  QCOMPARE(values->columnCount(), 3);
  QCOMPARE(values->data(values->index(0, 0), Qt::DisplayRole), QVariant("time"));
  QCOMPARE(values->data(values->index(0, 2), Qt::DisplayRole), QVariant("Test"));
  QCOMPARE(values->data(values->index(1, 0), Qt::DisplayRole), QVariant("add"));
  QCOMPARE(values->data(values->index(1, 2), Qt::DisplayRole), QVariant("added"));
}

void testDialogBoundValues::testGet()
{
  QVariant stored = values->getBoundValue("add");
  QCOMPARE(stored, QVariant("added"));
}


void testDialogBoundValues::testRemove()
{
  QList<BoundValue*> testRemove;
  testRemove << addedValue << 0;
  bool expected = true;
  foreach (BoundValue *test, testRemove)
  {
    bool ret = values->removeBoundValue(test);
    QCOMPARE(ret, expected);
    expected = !expected;

    QCOMPARE(values->rowCount(), 1);
    QCOMPARE(values->columnCount(), 3);
    QCOMPARE(values->data(values->index(0, 0), Qt::DisplayRole), QVariant("time"));
    QCOMPARE(values->data(values->index(0, 2), Qt::DisplayRole), QVariant("Test"));
  }
}

 
QTEST_MAIN(testDialogBoundValues)

#include "dialogboundvaluestest.moc"

