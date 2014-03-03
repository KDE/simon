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

#define private public

#include "../dialogvariablestore.h"
#include "../dialogvariable.h"

//TODO: Separate this into testDialogVariableStore and testDialogVariable

class testVariables: public QObject
{
  Q_OBJECT
  private slots:
    void testInitial();
    //void testGeneral();
    void testAdd();
    void testRemove();
    void testSet();
    void testTypedGet();
    void testInvalidTypedGet(); //TODO: Modify this test when method throws bad cast exception
    void testInvalidSet();
    void testCustomType(); //TODO:  Fix dialog variable so custom types can be used.
    
};

void testVariables::testInitial()
{
  //Sanity test
  int * i = new int();
  *i = 3;
  QCOMPARE(*i,3);
  *i += 2;
  QCOMPARE(*i,5);
  delete i;
}

void testVariables::testAdd()
{
  DialogVariableStore v;
  QString name = QString("attack");
  QString result = QString("3.01");
  v.addVariable<double>(name,3.01);
  QCOMPARE(v.getValue(name),result);
}

void testVariables::testRemove()
{
  DialogVariableStore v;
  v.addVariable<double>("attack",3.4);
  QCOMPARE(v.getValue("attack"),QString("3.4"));
  QVERIFY(v.dialogVariables.contains("attack"));
  v.removeVariable("attack");
  QVERIFY(!v.dialogVariables.contains("attack"));
}

void testVariables::testSet()
{
  DialogVariableStore v;
  v.addVariable<double>("attack",3.4);
  QCOMPARE(v.getValue("attack"),QString("3.4"));
  v.setVariable<double>("attack",3.5);
  QCOMPARE(v.getValue("attack"),QString("3.5"));
}

void testVariables::testTypedGet()
{
  DialogVariableStore v;
  v.addVariable<int>("defense",3);
  int i = v.getTypedValue<int>("defense");
  v.setVariable<int>("defense",i+1);
  QCOMPARE(v.getTypedValue<int>("defense"),i+1);
}

void testVariables::testInvalidTypedGet()
{
  DialogVariableStore v;
  v.addVariable<int>("oh noes",4);
  double d = v.getTypedValue<double>("oh noes");
  QCOMPARE(d,double());
}

void testVariables::testInvalidSet()
{
  DialogVariableStore v;
  
  v.addVariable<int>("mistakes",1);
  v.setVariable<double>("mistakes",3.5); //Should be OK. Set is really remove+add
  QCOMPARE(v.getTypedValue<double>("mistakes"),3.5);
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