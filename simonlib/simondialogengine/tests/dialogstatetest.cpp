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
#include <QSignalSpy>
#include <QDomDocument>
#include <QDomElement>
#include <QByteArray>

#include "../dialogturn.h"
#include "../dialogcommand.h"
#include "../dialogtextparser.h"
#include "../dialogtemplateoptions.h"
#include "../dialogboundvalues.h"

class testDialogTurn: public QObject
{
  Q_OBJECT
  private slots:
    void testGeneral();
    
  private:
    DialogTurn *turn;
};

void testDialogTurn::testGeneral()
{
  turn = DialogTurn::createInstance(NULL /*dialog text parser */, QDomElement());
  QVERIFY(!turn);

  //deserialization
  QDomDocument doc;

  //deps
  DialogTemplateOptions *options = DialogTemplateOptions::createInstance(doc.createElement("options"));
  DialogBoundValues *values = DialogBoundValues::createInstance(doc.createElement("boundValues"));
  DialogTextParser *textParser = new DialogTextParser(options, values);
  QVERIFY(options);
  QVERIFY(values);
  QVERIFY(textParser);

  doc.setContent(
      QByteArray(
        "<turn name=\"Welcome\">"
         "<text>Testtext{{bla}} here{{endbla}}</text>"
         "<transitions>"
          "<command>"
           "<name>Yes</name>"
           "<icon>accessories-dictionary</icon>"
           "<description></description>"
           "<turn>4097</turn>"
           "<newTurn>4098</newTurn>"
           "<announce>1</announce>"
           "<presentation>"
            "<text>Yup</text>"
            "<icon enabled=\"1\"/>"
           "</presentation>"
           "<auto>"
            "<active>0</active>"
            "<timeout>1</timeout>"
           "</auto>"
           "<switchTurn enabled=\"1\">2</switchTurn>"
           "<childCommands enabled=\"0\"/>"
          "</command>"
         "</transitions>"
        "</turn>"
       ));
  turn = DialogTurn::createInstance(textParser, doc.documentElement());
  QVERIFY(turn);

  QDomElement elem = turn->serialize(&doc);
  delete turn;
  turn = DialogTurn::createInstance(textParser, elem);
  QVERIFY(turn);

  QSignalSpy changedSpy(turn, SIGNAL(changed()));
  QVERIFY(changedSpy.isValid());
  QCOMPARE(changedSpy.count(), 0);

  QCOMPARE(turn->rowCount(), 1);
  QCOMPARE(turn->columnCount(), 1);
  QCOMPARE(turn->data(turn->index(0, 0), Qt::DisplayRole), QVariant("Yes"));
  QCOMPARE(turn->getName(), QString("Welcome"));

  turn->rename("Renamed");
  QCOMPARE(changedSpy.count(), 1);
  QCOMPARE(turn->getName(), QString("Renamed"));

  QCOMPARE(turn->getText(), QString("<html><head /><body><p>Testtext</p></body></html>"));
  QCOMPARE(turn->getRawText(0), QString("Testtext{{bla}} here{{endbla}}"));
  turn->setRawText(0, "Raw text changed");
  QCOMPARE(changedSpy.count(), 2);
  QCOMPARE(turn->getRawText(0), QString("Raw text changed"));

  QList<DialogCommand*> transitions = turn->getTransitions();
  QCOMPARE(transitions.count(), 1);
  DialogCommand *containedCommand = transitions.at(0);

  QCOMPARE(containedCommand->text(), QString("Yup"));

  containedCommand->update("No", "dialog-cancel", "description", "Really no", false, true, true, 3000, 
      false,  1, false, QStringList() << "testTU", QStringList() << "testCU");

  QCOMPARE(changedSpy.count(), 3);
  QCOMPARE(containedCommand->text(), QString("Really no"));

  //Values are NOT destroyed when not used in conjunction with a dialogcommandmanager instance!
  turn->removeTransition(containedCommand);

  QCOMPARE(turn->rowCount(), 0);
  turn->addTransition(containedCommand);
  QCOMPARE(turn->rowCount(), 1);
  
  DialogCommand *otherCommand =  new DialogCommand("Yes", "accessories-dictionary", "desc", "Yup", true, true, false, 3000, 
      true,  2, true, QStringList() << "testT", QStringList() << "testC");

  turn->addTransition(otherCommand);
  QCOMPARE(turn->rowCount(), 2);
  QCOMPARE(turn->data(turn->index(0, 0), Qt::DisplayRole), QVariant("No"));
  QCOMPARE(turn->data(turn->index(1, 0), Qt::DisplayRole), QVariant("Yes"));
  transitions = turn->getTransitions();
  QCOMPARE(transitions.count(), 2);
  QCOMPARE(transitions.at(0)->getTrigger(), QString("No"));
  QCOMPARE(transitions.at(1)->getTrigger(), QString("Yes"));

  bool success = turn->moveTransitionUp(otherCommand);
  QCOMPARE(success, true);
  QCOMPARE(turn->rowCount(), 2);
  QCOMPARE(turn->data(turn->index(0, 0), Qt::DisplayRole), QVariant("Yes"));
  QCOMPARE(turn->data(turn->index(1, 0), Qt::DisplayRole), QVariant("No"));
  transitions = turn->getTransitions();
  QCOMPARE(transitions.count(), 2);
  QCOMPARE(transitions.at(0)->getTrigger(), QString("Yes"));
  QCOMPARE(transitions.at(1)->getTrigger(), QString("No"));

  success = turn->moveTransitionUp(0);
  QCOMPARE(success, false);

  success = turn->moveTransitionDown(containedCommand);
  QCOMPARE(success, false); // already at end
  success = turn->moveTransitionDown(otherCommand);
  QCOMPARE(success, true); 
  QCOMPARE(turn->rowCount(), 2);
  QCOMPARE(turn->data(turn->index(0, 0), Qt::DisplayRole), QVariant("No"));
  QCOMPARE(turn->data(turn->index(1, 0), Qt::DisplayRole), QVariant("Yes"));
  transitions = turn->getTransitions();
  QCOMPARE(transitions.count(), 2);
  QCOMPARE(transitions.at(0)->getTrigger(), QString("No"));
  QCOMPARE(transitions.at(1)->getTrigger(), QString("Yes"));
  
  
  QCOMPARE(turn->getTextCount(), 1);
  QCOMPARE(turn->getRawText(0), QString("Raw text changed"));
  
  turn->addText("SecondTestText");
  QCOMPARE(turn->getTextCount(), 2);
  QCOMPARE(turn->getRawText(0), QString("Raw text changed"));
  QCOMPARE(turn->getRawText(1), QString("SecondTestText"));
  
  QVERIFY(!turn->removeText(7));
  QVERIFY(turn->removeText(0));
  QCOMPARE(turn->getTextCount(), 1);
  
  QCOMPARE(turn->getRawText(0), QString("SecondTestText"));
  turn->addText("ThirdTestText");
  QCOMPARE(turn->getTextCount(), 2);
  
  QCOMPARE(turn->getRawText(0), QString("SecondTestText"));
  QCOMPARE(turn->getRawText(1), QString("ThirdTestText"));
  
  QDomElement multipleTextsElem = turn->serialize(&doc);
  

  QSignalSpy destroyedSpy(containedCommand, SIGNAL(destroyed()));
  QVERIFY(destroyedSpy.isValid());
  QCOMPARE(destroyedSpy.count(), 0);
  delete turn;
  QCOMPARE(destroyedSpy.count(), 0); //commands are deleted by command manager

  delete containedCommand;
  delete otherCommand;
  
  turn = DialogTurn::createInstance(textParser, multipleTextsElem);
  QCOMPARE(turn->getRawText(0), QString("SecondTestText"));
  QCOMPARE(turn->getRawText(1), QString("ThirdTestText"));
  delete turn;
}

 
QTEST_MAIN(testDialogTurn)

#include "dialogturntest.moc"

