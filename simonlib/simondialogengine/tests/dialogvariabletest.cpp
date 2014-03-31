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

#include "../dialogvariablestore.h"

//TODO: Separate this into testDialogVariableStore and testDialogVariable

class testVariables: public QObject
{
  Q_OBJECT
  private slots:
    //void testInitial();
    //void testGeneral();
    void testAdd();
    void testRemove();
    //void testSet();
    //void testTypedGet();
    void testInvalidTypedGet();
    //void testInvalidSet();
    void testCustomType(); //TODO:  Fix dialog variable so custom types can be used.
};

void testVariables::testAdd()
{
  DialogVariableStore v;
  QString name = "attack";
  double result = 3.01;
  v.addVariable<double>(name,3.01);
  QCOMPARE(*(v.getValue<double>(name).data()),result);
}

void testVariables::testRemove()
{
  DialogVariableStore v;
  v.addVariable<double>("attack",3.4);
  DialogVariableValue<double> result = v.getValue<double>("attack");
  QCOMPARE(*result.data(),3.4);
  QVERIFY(v.contains("attack"));
  v.removeVariable("attack");
  QVERIFY(!v.contains("attack"));
}

/*void testVariables::testTypedGet()
{
  DialogVariableStore v;
  v.addVariable<int>("defense",3);
  int i = v.getTypedValue<int>("defense");
  v.setVariable<int>("defense",i+1);
  QCOMPARE(v.getTypedValue<int>("defense"),i+1);
}*/

void testVariables::testInvalidTypedGet()
{
  DialogVariableStore v;
  v.addVariable<int>("oh noes",4);
  DialogVariableValue<double> d = v.getValue<double>("oh noes");
  bool result = !d.isValid();
  QVERIFY(result);
}


void testVariables::testCustomType()
{
  //UNIMPLIMENTED
  QSKIP("This test is currently unimplemented", SkipSingle);
  DialogVariableStore v;
  //DialogDecimalVaraible d(QString("name"),3.4);
  //v.addVariable<DialogDecimalVaraible>("attack",d);
}

QTEST_MAIN(testVariables)

#include "dialogvariabletest.moc"