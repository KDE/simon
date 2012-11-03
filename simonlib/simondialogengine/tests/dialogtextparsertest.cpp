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

#include "../dialogtemplateoptions.h"
#include "../dialogtextparser.h"
#include "../dialogtext.h"
#include "../dialogboundvalues.h"

class testDialogTextParser: public QObject
{
  Q_OBJECT
  private slots:
    void testGeneral();
    void testParse();
    void testParse_data();
    void testText();

  private:
    DialogTemplateOptions *options;
    DialogBoundValues *values;
    DialogTextParser *textParser;
};

void testDialogTextParser::testGeneral()
{
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

  textParser = new DialogTextParser(options, values);
  QVERIFY(textParser);
}

void testDialogTextParser::testParse_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<bool>("success");
  QTest::addColumn<QString>("output");

  QTest::newRow("easy") << QString("Test") << true << QString("Test");
  QTest::newRow("templatesimple1") << QString("Test {{nice}}nice{{endnice}}") << true << QString("Test nice");
  QTest::newRow("templatesimple2") << QString("{{nice}}nice{{endnice}}") << true << QString("nice");
  QTest::newRow("templatesimple3") << QString("{{nice}}nice{{endnice}} ") << true << QString("nice ");

  QTest::newRow("templateelse1") << QString("Test {{nice}}nice{{elsenice}}not nice{{endnice}}") << true << QString("Test nice");
  QTest::newRow("templateelse2") << QString("{{nice}}nice{{elsenice}}not nice{{endnice}}") << true << QString("nice");
  QTest::newRow("templateelse3") << QString("{{nice}}nice{{elsenice}}not nice{{endnice}} ") << true << QString("nice ");
  QTest::newRow("templateelse4") << QString("Test {{formal}}formal{{elseformal}}not formal{{endformal}}") << true << QString("Test not formal");
  QTest::newRow("templateelse5") << QString("{{formal}}formal{{elseformal}}not formal{{endformal}}") << true << QString("not formal");
  QTest::newRow("templateelse6") << QString("{{formal}}formal{{elseformal}}not formal{{endformal}} ") << true << QString("not formal ");

  QTest::newRow("templateadvanced1") << QString("{{formal}}formal{{elseformal}}not formal{{nice}} but nice{{elsenice}} and not nice{{endnice}}{{endformal}}") << true << QString("not formal but nice");
  QTest::newRow("templateadvanced2") << QString("{{formal}}formal{{elseformal}}not formal{{nice}}{{elsenice}} and not nice{{endnice}}{{endformal}}") << true << QString("not formal");
  QTest::newRow("templateadvanced3") << QString("{{formal}}formal{{elseformal}}not formal{{nice}}{{endformal}} test{{endnice}}") << true << QString("not formal test");

  QTest::newRow("templateadvanced4") << QString("{{formal}}{{nice}}{{elsenice}}formal{{elseformal}}not formal{{nice}}{{endformal}} test{{endnice}}") << true << QString("");

  QTest::newRow("templateerror1") << QString("{{formal}}test{{endnice}}") << false << QString("{{formal}}test{{endnice}}");

  //unbound template options will evaluate to false
  QTest::newRow("templatedefaults") << QString("{{unbound}}test{{endunbound}}") << true << QString("");

  QTest::newRow("boundValue1") << QString("Test: $time$") << true << QString("Test: Test");
  QTest::newRow("boundValue2") << QString("$time$") << true << QString("Test");
  QTest::newRow("boundValue3") << QString("{{nice}}$time${{endnice}}") << true << QString("Test");
  QTest::newRow("boundValue4") << QString("$unbound$") << false << QString("$unbound$");
}

void testDialogTextParser::testParse()
{
  QFETCH(QString, input);
  QFETCH(bool, success);
  QFETCH(QString, output);

  bool ret = textParser->parse(input);
  QCOMPARE(input, output);
  QCOMPARE(ret, success);
}

void testDialogTextParser::testText()
{
  QString source = "{{nice}}$time${{endnice}}";
  DialogText *test = new DialogText(textParser, source);
  QCOMPARE(test->parse(), QString("<html><head /><body><p>Test</p></body></html>"));
  QCOMPARE(test->source(), source);
  source = "$unbound$";
  test->setSource(source);
  QCOMPARE(test->source(), source);
  QCOMPARE(test->parse(), QString("<html><head /><body><p>$unbound$</p></body></html>"));
}

 
QTEST_MAIN(testDialogTextParser)

#include "dialogtextparsertext.moc"

