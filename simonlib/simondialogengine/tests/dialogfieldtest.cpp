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

class testFields: public QObject
{
  Q_OBJECT
  private slots:
    //void testInitial();
    void testSerialize();
};

void testFields::testSerialize()
{
  QDomDocument doc("testdoc");
  DialogIntegerField test_field("Name",3);

  QDomElement serialized = test_field.serialize(&doc);
  doc.documentElement().appendChild(serialized);

  kDebug() << "Doc is: " << doc.toString();
}

QTEST_MAIN(testFields)

#include "dialogfieldtest.moc"
