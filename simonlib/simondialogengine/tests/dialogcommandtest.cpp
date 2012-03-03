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
#include <QSignalSpy>
#include <QDomDocument>
#include <QDomElement>
#include <QByteArray>
#include <KApplication>
#include <KCmdLineArgs>

#include "../dialogcommand.h"

class testDialogCommand: public QObject
{
  Q_OBJECT
  private slots:
    void testGeneral();
    void testExecution();

  private:
    DialogCommand *command;
};

void testDialogCommand::testGeneral()
{
  //error handling check
  command = DialogCommand::createInstance(QDomElement());
  QVERIFY(!command);

  command = new DialogCommand("Yes", "accessories-dictionary", "desc", "Yup", true, true, false, 3000, 
      true,  2, true, QStringList() << "testT", QStringList() << "testC");
  QVERIFY(command);

  QCOMPARE(command->getTrigger(), QString("Yes"));
  QCOMPARE(command->getIconSrc(), QString("accessories-dictionary"));
  QCOMPARE(command->getDescription(), QString("desc"));
  QCOMPARE(command->text(), QString("Yup"));
  QCOMPARE(command->showIcon(), true);
  QCOMPARE(command->silent(), true);
  QCOMPARE(command->activateAutomatically(), false);
  QCOMPARE(command->activationTimeout(), 3000);
  QCOMPARE(command->changeDialogState(), true);
  QCOMPARE(command->nextDialogState(), 2);
  QCOMPARE(command->executeCommands(), true);
  QCOMPARE(command->getCommands(), QStringList() << "testT");
  QCOMPARE(command->getCommandTypes(), QStringList() << "testC");

  QSignalSpy changedSpy(command, SIGNAL(changed()));
  QVERIFY(changedSpy.isValid());
  QCOMPARE(changedSpy.count(), 0);

  command->update("YesU", "accessories-dictionaryU", "descU", "YupU", false, false, true, 2500, 
      false,  1, false, QStringList() << "testTU", QStringList() << "testCU");

  QCOMPARE(changedSpy.count(), 1);

  QCOMPARE(command->getTrigger(), QString("YesU"));
  QCOMPARE(command->getIconSrc(), QString("accessories-dictionaryU"));
  QCOMPARE(command->getDescription(), QString("descU"));
  QCOMPARE(command->text(), QString("YupU"));
  QCOMPARE(command->showIcon(), false);
  QCOMPARE(command->silent(), false);
  QCOMPARE(command->activateAutomatically(), true);
  QCOMPARE(command->activationTimeout(), 2500);
  QCOMPARE(command->changeDialogState(), false);
  QCOMPARE(command->nextDialogState(), 1);
  QCOMPARE(command->executeCommands(), false);
  QCOMPARE(command->getCommands(), QStringList() << "testTU");
  QCOMPARE(command->getCommandTypes(), QStringList() << "testCU");

  delete command;

  //deserialization
  QDomDocument doc;
  doc.setContent(
      QByteArray(
      "<command>"
       "<name>Yes</name>"
       "<icon>accessories-dictionary</icon>"
       "<description>desc</description>"
       "<state>4097</state>"
       "<newState>4098</newState>"
       "<announce>1</announce>"
       "<presentation>"
        "<text>Yup</text>"
        "<icon enabled=\"1\"/>"
       "</presentation>"
       "<auto>"
        "<active>0</active>"
        "<timeout>3000</timeout>"
       "</auto>"
       "<switchState enabled=\"1\">2</switchState>"
       "<childCommands enabled=\"0\">"
        "<childCommand>"
         "<trigger>testT</trigger>"
         "<category>testC</category>"
        "</childCommand>"
       "</childCommands>"
      "</command>"
     ));
  command = DialogCommand::createInstance(doc.documentElement());
  QVERIFY(command);

  QDomElement elem = command->serialize(&doc);
  delete command;
  command = DialogCommand::createInstance(elem);
  QVERIFY(command);

  QCOMPARE(command->getTrigger(), QString("Yes"));
  QCOMPARE(command->getIconSrc(), QString("accessories-dictionary"));
  QCOMPARE(command->getDescription(), QString("desc"));
  QCOMPARE(command->text(), QString("Yup"));
  QCOMPARE(command->showIcon(), true);
  QCOMPARE(command->activateAutomatically(), false);
  QCOMPARE(command->activationTimeout(), 3000);
  QCOMPARE(command->changeDialogState(), true);
  QCOMPARE(command->nextDialogState(), 2);
  QCOMPARE(command->executeCommands(), false);
  QCOMPARE(command->getCommands(), QStringList() << "testT");
  QCOMPARE(command->getCommandTypes(), QStringList() << "testC");
}


void testDialogCommand::testExecution()
{
  char *appName = new char[5];
  strcpy(appName, "test");
  char **argv = new char*[1];
  argv[0] = appName;
  KCmdLineArgs::init(1, argv, argv[0], "", ki18n("test"), "0.1");

  KApplication *app = new KApplication(true);

  QSignalSpy dialogStateSpy(command, SIGNAL(requestDialogState(int)));
  QVERIFY(dialogStateSpy.isValid());
  QCOMPARE(dialogStateSpy.count(), 0);

  int fake;
  bool succ = command->trigger(&fake);

  QCOMPARE(succ, true);
  QCOMPARE(dialogStateSpy.count(), 1);
  QList<QVariant> arguments = dialogStateSpy.takeFirst();

  QCOMPARE(arguments.count(), 1);
  QCOMPARE(arguments.at(0), QVariant(2));

  command->update("YesU", "accessories-dictionaryU", "descU", "YupU", false, false, true, 2500, 
      false,  1, true, QStringList() << "testTUNBOUND", QStringList() << "testCU");
  succ = command->trigger(&fake);
  QCOMPARE(dialogStateSpy.count(), 0);
  QCOMPARE(succ, false);

  delete app;
  delete command;
  delete[] argv;
  delete[] appName;
}

 
QTEST_APPLESS_MAIN(testDialogCommand)

#include "dialogcommandtest.moc"



