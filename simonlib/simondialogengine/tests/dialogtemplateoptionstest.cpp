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

#include <QTest>
#include <QDomDocument>
#include <QDomElement>
#include <QByteArray>

#include "../dialogtemplateoptions.h"

class testDialogTemplateOptions: public QObject
{
  Q_OBJECT
  private slots:
    void testGeneral();
    void testAdd();
    void testGet();
    void testRemove();

  private:
    DialogTemplateOptions *options;
};

void testDialogTemplateOptions::testGeneral()
{
  //error handling check
  options = DialogTemplateOptions::createInstance(QDomElement());
  QVERIFY(options); // always return instance
  delete options;

  QDomDocument doc;
  doc.setContent(
      QByteArray(
    "<options>"
     "<option value=\"0\" name=\"formal\"/>"
     "<option value=\"1\" name=\"nice\"/>"
    "</options>"
     ));
  options = DialogTemplateOptions::createInstance(doc.documentElement());
  QVERIFY(options);


  QCOMPARE(options->rowCount(), 2);
  QCOMPARE(options->columnCount(), 2);

  QCOMPARE(options->data(options->index(0, 0), Qt::DisplayRole), QVariant("formal"));
  QCOMPARE(options->data(options->index(0, 1), Qt::DisplayRole), QVariant(false));
  QCOMPARE(options->data(options->index(1, 0), Qt::DisplayRole), QVariant("nice"));
  QCOMPARE(options->data(options->index(1, 1), Qt::DisplayRole), QVariant(true));

  QCOMPARE(options->data(options->index(0, 0), Qt::UserRole), QVariant("formal"));
  QCOMPARE(options->data(options->index(0, 1), Qt::UserRole), QVariant("formal"));
}

void testDialogTemplateOptions::testAdd()
{
  options->addOption("test1", true);
  QCOMPARE(options->rowCount(), 3);
  options->addOption("test2", true);
  QCOMPARE(options->rowCount(), 4);
  options->addOption("test2", false);
  QCOMPARE(options->rowCount(), 4);

  QCOMPARE(options->data(options->index(0, 0), Qt::DisplayRole), QVariant("formal"));
  QCOMPARE(options->data(options->index(0, 1), Qt::DisplayRole), QVariant(false));
  QCOMPARE(options->data(options->index(1, 0), Qt::DisplayRole), QVariant("nice"));
  QCOMPARE(options->data(options->index(1, 1), Qt::DisplayRole), QVariant(true));

  QCOMPARE(options->data(options->index(2, 0), Qt::DisplayRole), QVariant("test1"));
  QCOMPARE(options->data(options->index(2, 1), Qt::DisplayRole), QVariant(true));
  QCOMPARE(options->data(options->index(3, 0), Qt::DisplayRole), QVariant("test2"));
  QCOMPARE(options->data(options->index(3, 1), Qt::DisplayRole), QVariant(false));

  QCOMPARE(options->data(options->index(2, 0), Qt::UserRole), QVariant("test1"));
  QCOMPARE(options->data(options->index(2, 1), Qt::UserRole), QVariant("test1"));
}

void testDialogTemplateOptions::testGet()
{
  QCOMPARE(options->isEnabled("formal"), false);
  QCOMPARE(options->isEnabled("nice"), true);
  QCOMPARE(options->isEnabled("unset"), false);
}


void testDialogTemplateOptions::testRemove()
{
  options->removeOption("test1");
  QCOMPARE(options->rowCount(), 3);
  options->removeOption("test2");
  QCOMPARE(options->rowCount(), 2);
  options->removeOption("unset");
  QCOMPARE(options->rowCount(), 2);
}

 
QTEST_MAIN(testDialogTemplateOptions)

#include "dialogtemplateoptionstest.moc"


