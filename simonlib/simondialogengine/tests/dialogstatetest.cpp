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

#include "../dialogstate.h"
#include "../dialogcommand.h"
#include "../dialogtextparser.h"
#include "../dialogtemplateoptions.h"
#include "../dialogboundvalues.h"

class testDialogState: public QObject
{
  Q_OBJECT
  private slots:
    void testGeneral();
    
  private:
    DialogState *state;
};

void testDialogState::testGeneral()
{
  state = DialogState::createInstance(NULL /*dialog text parser */, QDomElement());
  QVERIFY(!state);

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
        "<state name=\"Welcome\">"
         "<text>Testtext{{bla}} here{{endbla}}</text>"
         "<transitions>"
          "<command>"
           "<name>Yes</name>"
           "<icon>accessories-dictionary</icon>"
           "<description></description>"
           "<state>4097</state>"
           "<newState>4098</newState>"
           "<announce>1</announce>"
           "<presentation>"
            "<text>Yup</text>"
            "<icon enabled=\"1\"/>"
           "</presentation>"
           "<auto>"
            "<active>0</active>"
            "<timeout>1</timeout>"
           "</auto>"
           "<switchState enabled=\"1\">2</switchState>"
           "<childCommands enabled=\"0\"/>"
          "</command>"
         "</transitions>"
        "</state>"
       ));
  state = DialogState::createInstance(textParser, doc.documentElement());
  QVERIFY(state);

  QDomElement elem = state->serialize(&doc);
  delete state;
  state = DialogState::createInstance(textParser, elem);
  QVERIFY(state);

  QSignalSpy changedSpy(state, SIGNAL(changed()));
  QVERIFY(changedSpy.isValid());
  QCOMPARE(changedSpy.count(), 0);

  QCOMPARE(state->rowCount(), 1);
  QCOMPARE(state->columnCount(), 1);
  QCOMPARE(state->data(state->index(0, 0), Qt::DisplayRole), QVariant("Yes"));
  QCOMPARE(state->getName(), QString("Welcome"));

  state->rename("Renamed");
  QCOMPARE(changedSpy.count(), 1);
  QCOMPARE(state->getName(), QString("Renamed"));

  QCOMPARE(state->getText(), QString("<html><head /><body><p>Testtext</p></body></html>"));
  QCOMPARE(state->getRawText(0), QString("Testtext{{bla}} here{{endbla}}"));
  state->setRawText(0, "Raw text changed");
  QCOMPARE(changedSpy.count(), 2);
  QCOMPARE(state->getRawText(0), QString("Raw text changed"));

  QList<DialogCommand*> transitions = state->getTransitions();
  QCOMPARE(transitions.count(), 1);
  DialogCommand *containedCommand = transitions.at(0);

  QCOMPARE(containedCommand->text(), QString("Yup"));

  containedCommand->update("No", "dialog-cancel", "description", "Really no", false, true, true, 3000, 
      false,  1, false, QStringList() << "testTU", QStringList() << "testCU");

  QCOMPARE(changedSpy.count(), 3);
  QCOMPARE(containedCommand->text(), QString("Really no"));

  //Values are NOT destroyed when not used in conjunction with a dialogcommandmanager instance!
  state->removeTransition(containedCommand);

  QCOMPARE(state->rowCount(), 0);
  state->addTransition(containedCommand);
  QCOMPARE(state->rowCount(), 1);
  
  DialogCommand *otherCommand =  new DialogCommand("Yes", "accessories-dictionary", "desc", "Yup", true, true, false, 3000, 
      true,  2, true, QStringList() << "testT", QStringList() << "testC");

  state->addTransition(otherCommand);
  QCOMPARE(state->rowCount(), 2);
  QCOMPARE(state->data(state->index(0, 0), Qt::DisplayRole), QVariant("No"));
  QCOMPARE(state->data(state->index(1, 0), Qt::DisplayRole), QVariant("Yes"));
  transitions = state->getTransitions();
  QCOMPARE(transitions.count(), 2);
  QCOMPARE(transitions.at(0)->getTrigger(), QString("No"));
  QCOMPARE(transitions.at(1)->getTrigger(), QString("Yes"));

  bool success = state->moveTransitionUp(otherCommand);
  QCOMPARE(success, true);
  QCOMPARE(state->rowCount(), 2);
  QCOMPARE(state->data(state->index(0, 0), Qt::DisplayRole), QVariant("Yes"));
  QCOMPARE(state->data(state->index(1, 0), Qt::DisplayRole), QVariant("No"));
  transitions = state->getTransitions();
  QCOMPARE(transitions.count(), 2);
  QCOMPARE(transitions.at(0)->getTrigger(), QString("Yes"));
  QCOMPARE(transitions.at(1)->getTrigger(), QString("No"));

  success = state->moveTransitionUp(0);
  QCOMPARE(success, false);

  success = state->moveTransitionDown(containedCommand);
  QCOMPARE(success, false); // already at end
  success = state->moveTransitionDown(otherCommand);
  QCOMPARE(success, true); 
  QCOMPARE(state->rowCount(), 2);
  QCOMPARE(state->data(state->index(0, 0), Qt::DisplayRole), QVariant("No"));
  QCOMPARE(state->data(state->index(1, 0), Qt::DisplayRole), QVariant("Yes"));
  transitions = state->getTransitions();
  QCOMPARE(transitions.count(), 2);
  QCOMPARE(transitions.at(0)->getTrigger(), QString("No"));
  QCOMPARE(transitions.at(1)->getTrigger(), QString("Yes"));
  
  
  QCOMPARE(state->getTextCount(), 1);
  QCOMPARE(state->getRawText(0), QString("Raw text changed"));
  
  state->addText("SecondTestText");
  QCOMPARE(state->getTextCount(), 2);
  QCOMPARE(state->getRawText(0), QString("Raw text changed"));
  QCOMPARE(state->getRawText(1), QString("SecondTestText"));
  
  QVERIFY(!state->removeText(7));
  QVERIFY(state->removeText(0));
  QCOMPARE(state->getTextCount(), 1);
  
  QCOMPARE(state->getRawText(0), QString("SecondTestText"));
  state->addText("ThirdTestText");
  QCOMPARE(state->getTextCount(), 2);
  
  QCOMPARE(state->getRawText(0), QString("SecondTestText"));
  QCOMPARE(state->getRawText(1), QString("ThirdTestText"));
  
  QDomElement multipleTextsElem = state->serialize(&doc);
  

  QSignalSpy destroyedSpy(containedCommand, SIGNAL(destroyed()));
  QVERIFY(destroyedSpy.isValid());
  QCOMPARE(destroyedSpy.count(), 0);
  delete state;
  QCOMPARE(destroyedSpy.count(), 0); //commands are deleted by command manager

  delete containedCommand;
  delete otherCommand;
  
  state = DialogState::createInstance(textParser, multipleTextsElem);
  QCOMPARE(state->getRawText(0), QString("SecondTestText"));
  QCOMPARE(state->getRawText(1), QString("ThirdTestText"));
  delete state;
}

 
QTEST_MAIN(testDialogState)

#include "dialogstatetest.moc"

