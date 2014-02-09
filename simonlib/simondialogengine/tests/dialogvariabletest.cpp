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
#include <QBuffer>
#include <QDomDocument>
#include <QDomElement>
 
#include "../dialogvariablestore.h"

class testVariables: public QObject
{
  Q_OBJECT
  private slots:
    void testInitial();
};

void testVariables::testInitial()
{
  int * i = new int();
  *i = 3;
  QCOMPARE(*i,3);
  *i += 2;
  QCOMPARE(*i,5);
  //QCOMPARE(*i,4);
  delete i;
}

QTEST_MAIN(testVariables)

#include "dialogvariabletest.moc"