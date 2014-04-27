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

//TODO: Write custom class to test this with.

class testFields: public QObject
{
  Q_OBJECT
  private slots:
    //void testInitial();
    void testGeneral();

    //Default Value Tests

    //Integer tests
    void testIntegerToString();

    void testIntegerCreate();
    void testIntegerSerialize();
    void testIntegerDeSerialize();

    void testIntegerBadCreate();
    void testIntegerBadDeSerialize();
    //void testIntegerBadDoc();
};

void testFields::testGeneral()
{
  QSKIP("This test is currently unimplemented", SkipSingle);
}

void testFields::testIntegerCreate()
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

QTEST_MAIN(testFields)

#include "dialogfieldtest.moc"

