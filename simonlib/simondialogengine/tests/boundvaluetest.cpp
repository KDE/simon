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
 
#include "../staticboundvalue.h"
#include "../scriptboundvalue.h"
#include "../plasmaboundvalue.h"

class testBoundValue: public QObject
{
  Q_OBJECT
  private slots:
    void testGeneral();
    void testStatic();
    void testScript();
    void testPlasma();
};

void testBoundValue::testGeneral()
{
  //error handling check
  BoundValue *boundValue = BoundValue::createInstance(QDomElement());
  QVERIFY(!boundValue);

  QDomDocument *doc = new QDomDocument;
  QDomElement elem = doc->createElement("boundValue");
  elem.setAttribute("type", "100");
  boundValue = BoundValue::createInstance(elem);
  QVERIFY(!boundValue);

  delete doc;
}
void testBoundValue::testStatic()
{
  //basic value check
  StaticBoundValue *staticValue = new StaticBoundValue("test", "value");
  QCOMPARE(staticValue->getValue(), QVariant("value"));

  //storing
  QDomDocument *doc = new QDomDocument;
  QDomElement elem = staticValue->serialize(doc);
  QCOMPARE(elem.attribute("type"), QString("1"));

  delete staticValue;

  //restoring
  staticValue = static_cast<StaticBoundValue*>(BoundValue::createInstance(elem));

  QVERIFY(staticValue);

  QCOMPARE(staticValue->getValue(), QVariant("value"));
  delete staticValue;

  //some error handling checks
  elem = doc->createElement("boundValue");
  elem.setAttribute("type", "1");
  staticValue = static_cast<StaticBoundValue*>(BoundValue::createInstance(elem));
  QVERIFY(!staticValue);

  delete doc;
}

void testBoundValue::testScript()
{
  //basic value check
  ScriptBoundValue *scriptValue = new ScriptBoundValue("test", "4+6");
  QCOMPARE(scriptValue->getValue(), QVariant(10));

  //storing
  QDomDocument *doc = new QDomDocument;
  QDomElement elem = scriptValue->serialize(doc);
  QCOMPARE(elem.attribute("type"), QString("2"));

  delete scriptValue;

  //restoring
  scriptValue = static_cast<ScriptBoundValue*>(BoundValue::createInstance(elem));
  QVERIFY(scriptValue);
  QCOMPARE(scriptValue->getValue(), QVariant(10));
  delete scriptValue;

  //some error handling checks
  elem = doc->createElement("boundValue");
  elem.setAttribute("type", "2");
  scriptValue = static_cast<ScriptBoundValue*>(BoundValue::createInstance(elem));
  QVERIFY(!scriptValue);

  delete doc;
}

void testBoundValue::testPlasma()
{
  //basic value check
    //PlasmaBoundValue(const QString& name, const QString& dataEngine, const QString& dataEngineName, 
        //const QString& dataSource, const QString& key);
  PlasmaBoundValue *plasmaValue = new PlasmaBoundValue("test", "time", "Time", "local", "Time");
  QVERIFY((plasmaValue->getValue() != QVariant()));
  QCOMPARE(plasmaValue->getValueDescription(), QString("Time/local/Time"));

  //storing
  QDomDocument *doc = new QDomDocument;
  QDomElement elem = plasmaValue->serialize(doc);
  QCOMPARE(elem.attribute("type"), QString("3"));

  delete plasmaValue;

  //restoring
  plasmaValue = static_cast<PlasmaBoundValue*>(BoundValue::createInstance(elem));
  QVERIFY(plasmaValue);
  QVERIFY(plasmaValue->getValue() != QVariant());

  delete plasmaValue;

  //some error handling checks
  elem = doc->createElement("boundValue");
  elem.setAttribute("type", "3");
  plasmaValue = static_cast<PlasmaBoundValue*>(BoundValue::createInstance(elem));
  QVERIFY(!plasmaValue);

  delete doc;
}

  
QTEST_MAIN(testBoundValue)

#include "boundvaluetest.moc"
