/*
 *   Copyright (C) 2014 Benjamin Bowley-Bryant <benbb@utexas.edu>
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
#include <QBuffer>
#include <QString>
#include <QDomDocument>
#include <QDomElement>
#include <KDebug>

#include "../dialogfieldstore.h"

class testFieldStore: public QObject
{
  Q_OBJECT
  private slots:
    //void testInitial();
    //void testGeneral();
    void testAdd();
    void testRemove();
    void testTypedGet();
    void testRegisterFactory();
    void testUnregisterFactory();

    void testDoubleRegister();
    void testInvalidTypeKeyAdd();
    void testInvalidValueAdd();
    void testNonExistantRemove();
    void testInvalidTypedGet();

    void testCustomType(); //TODO:  Fix dialog variable so custom types can be used.
};


void testFieldStore::testAdd()
{
  DialogFieldStore v;
  v.addField("int","attack","3");
  QCOMPARE(*(v.getValue<int>("attack").data()),3);
}

void testFieldStore::testRemove()
{
  DialogFieldStore v;
  v.addField("int","attack","3");
  DialogFieldValue<int> result = v.getValue<int>("attack");
  QCOMPARE(*result.data(),3);
  QVERIFY(v.contains("attack"));
  v.removeField("attack");
  QVERIFY(!v.contains("attack"));
}

void testFieldStore::testTypedGet()
{
  DialogFieldStore v;
  v.addField("int","yay","3535");
  DialogFieldValue<int> i = v.getValue<int>("yay");
  QVERIFY(i.isValid());
  QCOMPARE(*i.data(),3535);
}

void testFieldStore::testRegisterFactory()
{
  DialogFieldStore v;
  QSKIP("This test is currently unimplemented", SkipSingle);
  //DialogFieldTypeInfo f = DialogFieldTypeInfo("nope",
  //v.registerFactory();
}

void testFieldStore::testUnregisterFactory()
{
  DialogFieldStore v;
  v.unregisterFactory(DialogIntegerField::typeInfo._id);
  QVERIFY(!v.addField("int","name","3"));
}

void testFieldStore::testDoubleRegister()
{
  DialogFieldStore v;
  QVERIFY(!v.registerFactory(DialogIntegerField::typeInfo));
}

void testFieldStore::testInvalidTypeKeyAdd()
{
  DialogFieldStore v;
  QVERIFY(!v.addField("cat","name","3"));
}

void testFieldStore::testInvalidValueAdd()
{
  DialogFieldStore v;
  QVERIFY(!v.addField("int","name","foolish"));
}

void testFieldStore::testNonExistantRemove()
{
  DialogFieldStore v;
  QVERIFY(!v.removeField("help"));
}

void testFieldStore::testInvalidTypedGet()
{
  DialogFieldStore v;
  v.addField("int","oh noes","4");
  DialogFieldValue<double> d = v.getValue<double>("oh noes");
  bool result = !d.isValid();
  QVERIFY(result);
}


void testFieldStore::testCustomType()
{
  //UNIMPLIMENTED
  QSKIP("This test is currently unimplemented", SkipSingle);
  DialogFieldStore v;
}

QTEST_MAIN(testFieldStore)

#include "dialogfieldstoretest.moc"
