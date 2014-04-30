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

#include "../dialogfield.h"
#include "../dialogintegerfield.h"
#include "../dialogdoublefield.h"
#include "../dialogbooleanfield.h"

//TODO: Write custom class to test this with.

class testFields: public QObject
{
  Q_OBJECT
  private slots:
    //void testInitial();
    void testGeneral();
    void testFieldValue();

    //Default Value Tests

    //Integer tests
    void testIntegerToString();

    void testIntegerCreate();
    void testIntegerCreateWithDefaultValue();
    void testIntegerSerialize();
    void testIntegerDeSerialize();

    void testIntegerBadCreate();
    void testIntegerBadDeSerialize();

    //Double tests
    void testDoubleToString();

    void testDoubleCreate();
    void testDoubleCreateWithDefaultValue();
    void testDoubleSerialize();
    void testDoubleDeSerialize();

    void testDoubleBadCreate();
    void testDoubleBadDeSerialize();

    //Boolean tests
    void testBooleanToString();

    void testBooleanCreate();
    void testBooleanCreateWithDefaultValue();
    void testBooleanSerialize();
    void testBooleanDeSerialize();

    void testBooleanBadCreate();
    void testBooleanBadDeSerialize();
};

void testFields::testGeneral()
{
  QSKIP("This test is currently unimplemented", SkipSingle);
}

void testFields::testFieldValue()
{
  DialogIntegerField d("Name",3);
  DialogFieldValue<int> v = d.getVal();
  QCOMPARE(*d.getVal().data(),3);
  *v.data() = 54;
  QCOMPARE(*d.getVal().data(),54);
}

void testFields::testIntegerCreate()
{
  DialogIntegerField* d = dynamic_cast<DialogIntegerField*>(DialogIntegerField::typeInfo.create("Name"));
  QVERIFY(d);
  QCOMPARE(d->getName(),QString("Name"));
  QCOMPARE(*d->getVal().data(),int());
}

void testFields::testIntegerCreateWithDefaultValue()
{
  DialogIntegerField* d = dynamic_cast<DialogIntegerField*>(DialogIntegerField::typeInfo.create("Name","3"));
  QVERIFY(d);
  QCOMPARE(d->getName(),QString("Name"));
  QCOMPARE(*d->getVal().data(),3);
}

void testFields::testIntegerToString()
{
  DialogIntegerField test_field("Name",4);
  QCOMPARE(test_field.toString(),QString("4"));
}


void testFields::testIntegerSerialize()
{
  QDomDocument doc("");
  QDomDocument expected_doc("");
  DialogIntegerField test_field("Name",3);

  expected_doc.setContent(QByteArray(
	"<field>"
	"<name>Name</name>"
	"<type>int</type>"
	"<value>3</value>"
	"</field>"
  ));

  QDomElement serialized = test_field.serialize(&doc);
  doc.appendChild(serialized);

  QCOMPARE(doc.toString(),expected_doc.toString());
}

void testFields::testIntegerDeSerialize()
{
  QDomDocument doc("");

  doc.setContent(QByteArray(
	"<field>"
	"<name>Name</name>"
	"<type>int</type>"
	"<value>3</value>"
	"</field>"
  ));

  QDomElement first_elem = doc.firstChildElement();

  DialogIntegerField* result_field = dynamic_cast<DialogIntegerField*>(DialogIntegerField::typeInfo.deSerialize(first_elem));
  QVERIFY(result_field); // Check to make sure it's not null.
  QCOMPARE(result_field->getName(),QString("Name"));
  QCOMPARE(*(result_field->getVal().data()),3);
  delete result_field;
}

void testFields::testIntegerBadDeSerialize()
{
  QDomDocument doc("");

  doc.setContent(QByteArray(
	"<field>"
	"<name>Name</name>"
	"<type>int</type>"
	"</field>"
  ));

  QDomElement first_elem = doc.firstChildElement();

  //Should be null due to doc not having a value
  DialogIntegerField* result_field = dynamic_cast<DialogIntegerField*>(DialogIntegerField::typeInfo.deSerialize(first_elem));
  QVERIFY(!result_field);
}

void testFields::testIntegerBadCreate()
{
  DialogIntegerField* d = dynamic_cast<DialogIntegerField*>(DialogIntegerField::typeInfo.create("name","value"));
  QVERIFY(!d);
}

//----------------------------
//       Double Tests
//----------------------------

void testFields::testDoubleCreate()
{
  DialogDoubleField* d = dynamic_cast<DialogDoubleField*>(DialogDoubleField::typeInfo.create("Name"));
  QVERIFY(d);
  QCOMPARE(d->getName(),QString("Name"));
  QCOMPARE(*d->getVal().data(),double());
}

void testFields::testDoubleCreateWithDefaultValue()
{
  DialogDoubleField* d = dynamic_cast<DialogDoubleField*>(DialogDoubleField::typeInfo.create("Name",".3"));
  QVERIFY(d);
  QCOMPARE(d->getName(),QString("Name"));
  QCOMPARE(*d->getVal().data(),.3);
}

void testFields::testDoubleToString()
{
  DialogDoubleField test_field("Name",.45);
  QCOMPARE(test_field.toString(),QString("0.45"));
}


void testFields::testDoubleSerialize()
{
  QDomDocument doc("");
  QDomDocument expected_doc("");
  DialogDoubleField test_field("Name",3.5);

  expected_doc.setContent(QByteArray(
	"<field>"
	"<name>Name</name>"
	"<type>double</type>"
	"<value>3.5</value>"
	"</field>"
  ));

  QDomElement serialized = test_field.serialize(&doc);
  doc.appendChild(serialized);

  QCOMPARE(doc.toString(),expected_doc.toString());
}

void testFields::testDoubleDeSerialize()
{
  QDomDocument doc("");

  doc.setContent(QByteArray(
	"<field>"
	"<name>Name</name>"
	"<type>double</type>"
	"<value>3.5</value>"
	"</field>"
  ));

  QDomElement first_elem = doc.firstChildElement();

  DialogDoubleField* result_field = dynamic_cast<DialogDoubleField*>(DialogDoubleField::typeInfo.deSerialize(first_elem));
  QVERIFY(result_field); // Check to make sure it's not null.
  QCOMPARE(result_field->getName(),QString("Name"));
  QCOMPARE(*(result_field->getVal().data()),3.5);
  delete result_field;
}

void testFields::testDoubleBadDeSerialize()
{
  QDomDocument doc("");

  doc.setContent(QByteArray(
	"<field>"
	"<name>Name</name>"
	"<type>double</type>"
	"</field>"
  ));

  QDomElement first_elem = doc.firstChildElement();

  //Should be null due to doc not having a value
  DialogDoubleField* result_field = dynamic_cast<DialogDoubleField*>(DialogDoubleField::typeInfo.deSerialize(first_elem));
  QVERIFY(!result_field);
}

void testFields::testDoubleBadCreate()
{
  DialogDoubleField* d = dynamic_cast<DialogDoubleField*>(DialogDoubleField::typeInfo.create("name","value"));
  QVERIFY(!d);
}

//----------------------------
//      Boolean Tests
//----------------------------

void testFields::testBooleanToString()
{
  DialogBooleanField b("Test",0);
  QCOMPARE(b.toString(),QString("false"));
}

void testFields::testBooleanCreate()
{
  DialogBooleanField* b = dynamic_cast<DialogBooleanField*>(DialogBooleanField::typeInfo.create("Test"));
  QVERIFY(b);
  QCOMPARE(b->getName(),QString("Test"));
  QCOMPARE(*(b->getVal().data()),bool());
}

void testFields::testBooleanCreateWithDefaultValue()
{
  DialogBooleanField* b = dynamic_cast<DialogBooleanField*>(DialogBooleanField::typeInfo.create("Test","true"));
  QVERIFY(b);
  QCOMPARE(b->getName(),QString("Test"));
  QCOMPARE(*(b->getVal().data()),true);
}

void testFields::testBooleanSerialize()
{
  QDomDocument doc("");
  QDomDocument expected_doc("");
  DialogBooleanField test_field("Name",true);

  expected_doc.setContent(QByteArray(
	"<field>"
	"<name>Name</name>"
	"<type>bool</type>"
	"<value>true</value>"
	"</field>"
  ));

  QDomElement serialized = test_field.serialize(&doc);
  doc.appendChild(serialized);

  QCOMPARE(doc.toString(),expected_doc.toString());
}

void testFields::testBooleanDeSerialize()
{
  QDomDocument doc("");

  doc.setContent(QByteArray(
	"<field>"
	"<name>Name</name>"
	"<type>bool</type>"
	"<value>true</value>"
	"</field>"
  ));

  QDomElement elem = doc.firstChildElement();
  DialogBooleanField* result = dynamic_cast<DialogBooleanField*>(DialogBooleanField::typeInfo.deSerialize(elem));
  QVERIFY(result);
  QCOMPARE(result->getName(),QString("Name"));
  QCOMPARE(*(result->getVal().data()),true);
}

void testFields::testBooleanBadDeSerialize()
{
  QDomDocument doc("");

  doc.setContent(QByteArray(
	"<field>"
	"<name>Name</name>"
	"<type>bool</type>"
	"<value>dasdf###agds\0\343\35</value>"
	"</field>"
  ));

  QDomElement elem = doc.firstChildElement();
  DialogBooleanField* result = dynamic_cast<DialogBooleanField*>(DialogBooleanField::typeInfo.deSerialize(elem));
  QVERIFY(!result);
}

void testFields::testBooleanBadCreate()
{
  DialogBooleanField* result = dynamic_cast<DialogBooleanField*>(DialogBooleanField::typeInfo.create("String","cats"));
  QVERIFY(!result);
}


QTEST_MAIN(testFields)

#include "dialogfieldtest.moc"

