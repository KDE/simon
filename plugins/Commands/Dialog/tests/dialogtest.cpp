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

/*********************
 * Integration test
*********************/

#include <QtTest>
#include <QtCore>
#include <QDomDocument>
#include <QDomElement>
#include <QByteArray>
#include <QLabel>
#include <QTimer>
#include <QVariant>
#include <QWidget>
#include <KApplication>
#include <KCmdLineArgs>
#include <KInputDialog>
#include <KPushButton>
#include <KLineEdit>
#include <KIntSpinBox>
#include <KComboBox>

#include "../dialogcommandmanager.h"
#include "../visualdialogview.h"
#include "../dialogstate.h"
#include "../dialogconfiguration.h"
#include "../dialogcommand.h"
#include "../dialogview.h"


class testDialog: public QObject
{
  Q_OBJECT
  private slots:
    void initTestCase();

    void testGeneral();
    void testStates();
    void testCommands();
    void testView();

    void cleanupTestCase();

    void cancelDialog();
    void okDialog();

    void fillInNewStateDialog();
    void fillInEditStateDialog();
    void fillInMessage();
    void removeStateAbort();
    void removeState();
    void checkStates1();
    void checkStates2();
    void checkStates3();
    void checkStates4();
    void checkStates5();
    void checkStates6();
    void checkStates7();

    void createTransition();
    void editTransition();
    void checkTransitions1();
    void checkTransitions2();
    void checkTransitions3();
    void checkTransitions4();
    void checkTransitions5();
    void removeTransitionAbort();
    void removeTransition();

    void checkView1();
    void checkView2();

  private:
    KApplication *app;
    DialogCommandManager *dialog;

    template <class T>
    T findChild(QObject *object, const QString& name, int* skip=0);

    void clickConfigButton(const QString& buttonName, int delay, const QString& slot);
    void clickDialogOption(const QString& triggerName, int delay, const QString& slot);
    QString currentlyDisplayedDialogText();
    VisualDialogView *getDialogView();
};

void testDialog::initTestCase()
{
  char *appName = new char[5];
  strcpy(appName, "test");
  char **argv = new char*[1];
  *argv = appName;
  KCmdLineArgs::init(1, argv, "test", "test", ki18n("appname"), "0.1");
  delete[] argv;
  delete[] appName;

  app = new KApplication(true);
}

void testDialog::cleanupTestCase()
{
  delete dialog;
  delete app;
}

void testDialog::testGeneral()
{
  QDomDocument doc;
  doc.setContent(
      QByteArray(
   "<plugin trigger=\"\" name=\"simondialogcommandplugin.desktop\">"
   "<config>"
    "<options>"
     "<option value=\"1\" name=\"formal\"/>"
    "</options>"
    "<boundValues>"
     "<boundValue type=\"1\">"
      "<name>time</name>"
      "<value content=\"Test\"/>"
     "</boundValue>"
    "</boundValues>"
   "</config>"
   "<commands>"
    "<voiceInterfaceCommand>"
     "<name>Dialog</name>"
     "<icon>im-user</icon>"
     "<description>Start dialog</description>"
     "<state>0</state>"
     "<newState>4096</newState>"
     "<announce>1</announce>"
     "<id>startDialog</id>"
     "<visibleTrigger>Dialog</visibleTrigger>"
     "<showIcon>1</showIcon>"
    "</voiceInterfaceCommand>"
    "<state name=\"Welcome\">"
     "<text>Welcome, {{formal}}Sir{{elseformal}}you{{endformal}}: $time$</text>"
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
      "<command>"
       "<name>CloseStock</name>"
       "<icon>dialog-close</icon>"
       "<description>Closes this dialog</description>"
       "<state>4098</state>"
       "<newState>4096</newState>"
       "<announce>1</announce>"
       "<presentation>"
        "<text>Close</text>"
        "<icon enabled=\"1\"/>"
       "</presentation>"
       "<auto>"
        "<active>1</active>"
        "<timeout>2500</timeout>"
       "</auto>"
       "<switchState enabled=\"1\">0</switchState>"
       "<childCommands enabled=\"0\"/>"
      "</command>"

     "</transitions>"
    "</state>"
    "<state name=\"Good bye\">"
     "<text>Thanks</text>"
     "<transitions>"
      "<command>"
       "<name>Back</name>"
       "<icon>go-left</icon>"
       "<description>Goes back to page 1</description>"
       "<state>4098</state>"
       "<newState>4096</newState>"
       "<announce>1</announce>"
       "<presentation>"
        "<text>Back</text>"
        "<icon enabled=\"1\"/>"
       "</presentation>"
       "<auto>"
        "<active>0</active>"
        "<timeout>1500</timeout>"
       "</auto>"
       "<switchState enabled=\"1\">1</switchState>"
       "<childCommands enabled=\"0\"/>"
      "</command>"
     "</transitions>"
    "</state>"
   "</commands>"
   "<pluginCompatibility>"
    "<minimumVersion>"
     "<version>0.1.0</version>"
    "</minimumVersion>"
    "<maximumVersion/>"
   "</pluginCompatibility>"
  "</plugin>"
     ));
  dialog = new DialogCommandManager(0, QVariantList());
  dialog->deSerialize(doc.documentElement());
  QVERIFY(dialog);


  QDomElement elem = dialog->serialize(&doc);
  delete dialog;
  dialog = new DialogCommandManager(0, QVariantList());
  dialog->deSerialize(elem);
  QVERIFY(dialog);
  QVERIFY(dialog->getConfigurationPage());

  QList<DialogState*> states = dialog->getStates();
  QCOMPARE(states.count(), 2);
  QCOMPARE(states.at(0)->getName(), QString("Welcome"));
  QCOMPARE(states.at(0)->getText(), QString("<html><head /><body><p>Welcome, Sir: Test</p></body></html>"));
}

void testDialog::testStates()
{
  //if (!sender()) QSKIP("Make it fast", SkipAll);

  clickConfigButton("pbAddState", 500, SLOT(fillInNewStateDialog()));
  QTest::qWait(1000);

  DialogConfiguration *config = static_cast<DialogConfiguration*>(dialog->getConfigurationPage());
  QListWidget *lwStates = findChild<QListWidget*>(config, "lwStates");
  QVERIFY(lwStates);
  lwStates->setCurrentRow(2);
  clickConfigButton("pbRenameState", 500, SLOT(fillInEditStateDialog()));

  QTest::qWait(1000);

  clickConfigButton("pbEditText", 500, SLOT(fillInMessage()));
  QTest::qWait(1000);

  clickConfigButton("pbMoveStateUp", 500, SLOT(checkStates4()));
  QTest::qWait(1000);

  clickConfigButton("pbMoveStateDown", 500, SLOT(checkStates5()));
  QTest::qWait(1000);

  clickConfigButton("pbRemoveState", 500, SLOT(removeStateAbort()));
  QTest::qWait(1000);

  QSignalSpy destroyedSpy(dialog->getStates().at(2), SIGNAL(destroyed()));
  QVERIFY(destroyedSpy.isValid());
  QCOMPARE(destroyedSpy.count(), 0);
  clickConfigButton("pbRemoveState", 500, SLOT(removeState()));
  QTest::qWait(1000);
  QCOMPARE(destroyedSpy.count(), 1); // make sure it got destroyed

  lwStates->setCurrentRow(1);
}

void testDialog::fillInNewStateDialog()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QWidget *dlg = app->activeModalWidget();
  QVERIFY(dlg);
  KLineEdit *leTest = findChild<KLineEdit*>(dlg, "");
  QVERIFY(leTest);
  QTest::keyClicks(leTest, "newState");

  KPushButton *okButton = findChild<KPushButton*>(dlg, "");
  QVERIFY(okButton);
  QTest::mouseClick(okButton, Qt::LeftButton);

  QTimer::singleShot(150, this, SLOT(checkStates1()));
}


void testDialog::fillInEditStateDialog()
{
  if (!sender())
    QSKIP("Internal slot, not a test by itself", SkipAll);

  QWidget *dlg = app->activeModalWidget();
  QVERIFY(dlg);
  KLineEdit *leTest = findChild<KLineEdit*>(dlg, "");
  QVERIFY(leTest);
  QTest::keyClicks(leTest, "newStateNewName");

  KPushButton *okButton = findChild<KPushButton*>(dlg, "");
  QVERIFY(okButton);
  okButton->click();

  QTimer::singleShot(150, this, SLOT(checkStates2()));
}

void testDialog::fillInMessage()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QWidget *dlg = app->activeModalWidget();
  QVERIFY(dlg);
  KTextEdit *teText = findChild<KTextEdit*>(dlg, "");
  QVERIFY(teText);
  QTest::keyClicks(teText, "Message");

  int skip = 1;
  KPushButton *okButton = findChild<KPushButton*>(dlg, "", &skip);
  QVERIFY(okButton);
  okButton->click();

  QTimer::singleShot(150, this, SLOT(checkStates3()));
}

void testDialog::removeStateAbort()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);
  cancelDialog();
  QTimer::singleShot(150, this, SLOT(checkStates6()));
}


void testDialog::removeState()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  okDialog();
  QTimer::singleShot(150, this, SLOT(checkStates7()));
}


void testDialog::checkStates1()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QList<DialogState*> states = dialog->getStates();
  QCOMPARE(states.count(), 3);
  QCOMPARE(states.at(2)->getName(), QString("newState"));
}

void testDialog::checkStates2()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QList<DialogState*> states = dialog->getStates();
  QCOMPARE(dialog->getStates().count(), 3);

  QCOMPARE(states.at(0)->getName(), QString("Welcome"));
  QCOMPARE(states.at(2)->getName(), QString("newStateNewName"));
}

void testDialog::checkStates3()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QList<DialogState*> states = dialog->getStates();
  QCOMPARE(dialog->getStates().count(), 3);

  QCOMPARE(states.at(0)->getText(), QString("<html><head /><body><p>Welcome, Sir: Test</p></body></html>"));
  QCOMPARE(states.at(2)->getText(), QString("<html><head /><body><p>Message</p></body></html>"));
}

void testDialog::checkStates4()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QList<DialogState*> states = dialog->getStates();
  QCOMPARE(dialog->getStates().count(), 3);

  QCOMPARE(states.at(0)->getName(), QString("Welcome"));
  QCOMPARE(states.at(1)->getName(), QString("newStateNewName"));
  QCOMPARE(states.at(2)->getName(), QString("Good bye"));
}

void testDialog::checkStates5()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QList<DialogState*> states = dialog->getStates();
  QCOMPARE(dialog->getStates().count(), 3);

  QCOMPARE(states.at(0)->getName(), QString("Welcome"));
  QCOMPARE(states.at(1)->getName(), QString("Good bye"));
  QCOMPARE(states.at(2)->getName(), QString("newStateNewName"));
}

void testDialog::checkStates6()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QCOMPARE(dialog->getStates().count(), 3);
}

void testDialog::checkStates7()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QCOMPARE(dialog->getStates().count(), 2);
}


////////////////////////

void testDialog::testCommands()
{
  //if (!sender()) QSKIP("Make it fast", SkipAll);

  clickConfigButton("pbAddTransition", 500, SLOT(createTransition()));

  QTest::qWait(1000);

  DialogConfiguration *config = static_cast<DialogConfiguration*>(dialog->getConfigurationPage());
  QListView *lvTransitions = findChild<QListView*>(config, "lvTransitions");
  QVERIFY(lvTransitions);

  QTest::mouseClick(lvTransitions, Qt::LeftButton);
  QTest::keyClick(lvTransitions, Qt::Key_Down);
  QTest::keyClick(lvTransitions, Qt::Key_Down);
  QTest::qWait(200);

  clickConfigButton("pbEditTransition", 500, SLOT(editTransition()));
  QTest::qWait(1000);
  clickConfigButton("pbMoveTransitionUp", 500, SLOT(checkTransitions3()));
  QTest::qWait(1000);
  clickConfigButton("pbMoveTransitionDown", 500, SLOT(checkTransitions4()));
  QTest::qWait(1000);

  clickConfigButton("pbRemoveTransition", 500, SLOT(removeTransitionAbort()));
  QTest::qWait(1000);
  clickConfigButton("pbRemoveTransition", 500, SLOT(removeTransition()));

  QTest::qWait(1000);

  CommandList* commands = dialog->getCommands();
  QCOMPARE(commands->count(), 4); 
  QCOMPARE(dialog->rowCount(), 1); // transitions are hidden internal commands

  QCOMPARE(dialog->trigger("bla"), false);
  QCOMPARE(dialog->trigger("Dialog"), true);
}

void testDialog::createTransition()
{
  if (!sender())
    QSKIP("Internal slot, not a test by itself", SkipAll);

  QWidget *dlg = app->activeModalWidget();
  QVERIFY(dlg);
  KLineEdit *leTrigger = findChild<KLineEdit*>(dlg, "leTrigger");
  QVERIFY(leTrigger);
  QTest::keyClicks(leTrigger, "Close");

  QGroupBox *gbAutomatic = findChild<QGroupBox*>(dlg, "gbAutomatic");
  QVERIFY(gbAutomatic);
  QTest::keyClick(gbAutomatic, ' ');

  KIntSpinBox *sbTimeout = findChild<KIntSpinBox*>(dlg, "sbAutoTimeout");
  QVERIFY(sbTimeout);
  QTest::keyClick(sbTimeout, Qt::Key_Delete);
  QTest::keyClick(sbTimeout, Qt::Key_Delete);
  QTest::keyClick(sbTimeout, Qt::Key_Delete);
  QTest::keyClick(sbTimeout, Qt::Key_Delete);
  QTest::keyClick(sbTimeout, Qt::Key_Delete);
  QTest::keyClicks(sbTimeout, "150");

  KComboBox *cbNextState = findChild<KComboBox*>(dlg, "cbNextState");
  QVERIFY(cbNextState);
  QTest::keyClick(cbNextState, '0');

  QTest::keyClick(dlg, Qt::Key_Return);

  QTimer::singleShot(150, this, SLOT(checkTransitions1()));
}

void testDialog::editTransition()
{
  if (!sender())
    QSKIP("Internal slot, not a test by itself", SkipAll);

  QWidget *dlg = app->activeModalWidget();
  QVERIFY(dlg);
  KLineEdit *leTrigger = findChild<KLineEdit*>(dlg, "leTrigger");
  QVERIFY(leTrigger);
  QTest::keyClicks(leTrigger, "2");

  QGroupBox *gbAutomatic = findChild<QGroupBox*>(dlg, "gbAutomatic");
  QVERIFY(gbAutomatic);
  QTest::keyClick(gbAutomatic, ' ');

  QGroupBox *gbCommands = findChild<QGroupBox*>(dlg, "gbCommands");
  QVERIFY(gbCommands);
  QTest::keyClick(gbCommands, ' ');

  KComboBox *cbNextState = findChild<KComboBox*>(dlg, "cbNextState");
  QVERIFY(cbNextState);
  QTest::keyClick(cbNextState, '1');

  QTest::keyClick(dlg, Qt::Key_Return);

  QTimer::singleShot(150, this, SLOT(checkTransitions2()));
}


void testDialog::removeTransitionAbort()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);
  cancelDialog();
  QTimer::singleShot(150, this, SLOT(checkTransitions4()));
}


void testDialog::removeTransition()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);
  okDialog();
  QTimer::singleShot(150, this, SLOT(checkTransitions5()));
}

void testDialog::checkTransitions1()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QList<DialogState*> states = dialog->getStates();
  QCOMPARE(dialog->getStates().count(), 2);
  QList<DialogCommand*> commands = states.at(1)->getTransitions();
  QCOMPARE(commands.count(), 2);
  DialogCommand *newCommand = commands.at(1);
  QCOMPARE(newCommand->getTrigger(), QString("Close"));
  QCOMPARE(newCommand->activateAutomatically(), true);
  QCOMPARE(newCommand->activationTimeout(), 150);
}

void testDialog::checkTransitions2()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QList<DialogState*> states = dialog->getStates();
  QCOMPARE(dialog->getStates().count(), 2);
  QList<DialogCommand*> commands = states.at(1)->getTransitions();
  QCOMPARE(commands.count(), 2);
  DialogCommand *newCommand = commands.at(1);
  QCOMPARE(newCommand->getTrigger(), QString("Close2"));
  QCOMPARE(newCommand->activateAutomatically(), false);
  QCOMPARE(newCommand->executeCommands(), true);
  QCOMPARE(newCommand->activationTimeout(), 150);
  QCOMPARE(newCommand->nextDialogState(), 1);
}

void testDialog::checkTransitions3()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QList<DialogState*> states = dialog->getStates();
  QList<DialogCommand*> commands = states.at(1)->getTransitions();
  QCOMPARE(commands.count(), 2);
  QCOMPARE(commands.at(0)->getTrigger(), QString("Close2"));
  QCOMPARE(commands.at(1)->getTrigger(), QString("Back"));
}

void testDialog::checkTransitions4()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QList<DialogState*> states = dialog->getStates();
  QList<DialogCommand*> commands = states.at(1)->getTransitions();
  QCOMPARE(commands.count(), 2);
  QCOMPARE(commands.at(0)->getTrigger(), QString("Back"));
  QCOMPARE(commands.at(1)->getTrigger(), QString("Close2"));
}

void testDialog::checkTransitions5()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QList<DialogState*> states = dialog->getStates();
  QList<DialogCommand*> commands = states.at(1)->getTransitions();
  QCOMPARE(commands.count(), 1);
  QCOMPARE(commands.at(0)->getTrigger(), QString("Back"));
}

//////////////////////

void testDialog::testView()
{
  dialog->activate();
  QTest::qWait(1000);
  QWidget *visualView = getDialogView();
  kDebug() << visualView;

  QVERIFY(visualView);

  checkView1();

  //activate first option to go to page 2 (faster than the close timeout)
  clickDialogOption("Yes", 500, SLOT(checkView2()));
  kDebug() << "here3";
  QTest::qWait(1000);
  kDebug() << "here4";

  //activate first option to go back to page 1
  QTimer::singleShot(500, this, SLOT(checkView1()));
  dialog->trigger("Back");
  //clickDialogOption("Back", 500, SLOT(checkView1()));

  //let timeout run out
  QTest::qWait(4000);

  QCOMPARE(visualView->isVisible(), false);
}

QString testDialog::currentlyDisplayedDialogText()
{
  QLabel *label = findChild<QLabel*>(getDialogView(), "lbText");

  if (!label) {
    kDebug() << "COULDN'T FIND LABEL";
    return QString();
  }

  return label->text();
}

void testDialog::checkView1()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);
  QCOMPARE(currentlyDisplayedDialogText(), QString("<html><head /><body><p>Welcome, Sir: Test</p></body></html>"));
}

void testDialog::checkView2()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);
  QCOMPARE(currentlyDisplayedDialogText(), QString("<html><head /><body><p>Thanks</p></body></html>"));
}

///////////////////////

VisualDialogView* testDialog::getDialogView()
{
  QWidgetList dialogs = app->topLevelWidgets();
  if (dialogs.isEmpty()) return 0;

  foreach (QWidget* w, dialogs)
  {
    VisualDialogView *view = dynamic_cast<VisualDialogView*>(w);
    if (view)
      return view;
  }
  return 0;
}

#include <QDebug>
template <class T>
T testDialog::findChild(QObject *object, const QString& name, int* skip)
{
  bool created = false;
  if (skip == 0)
  {
    skip = new int;
    *skip = 0;
    created = true;
  }
  QObjectList children = object->children();

  //if (children.isEmpty())
    //qDebug() << "Empty: " << object;

  foreach (QObject *child, children)
  {
    //qDebug() << "Child: " << child->objectName() << "Right type: " << dynamic_cast<T>(child) << child;
    //if (dynamic_cast<QAbstractButton*>(child))
      //qDebug() << dynamic_cast<QAbstractButton*>(child)->text();

    if ((child->objectName() == name) && (dynamic_cast<T>(child)))
    {
      //kDebug() << "Returning child: " << child;
      if (*skip == 0)
      {
        if (created) delete skip;
        return dynamic_cast<T>(child);
      }
      else kDebug() << "SKIPPING!";

      --(*skip);
    }
    else 
    {
      //qDebug() << "calling find child on: " << child;
      T ret = findChild<T>(child, name, skip);
      if (ret)
        return ret;
    }
  }
  if (created) delete skip;
  return 0;
}

void testDialog::clickConfigButton(const QString& buttonName, int delay, const QString& slot)
{
  DialogConfiguration *config = static_cast<DialogConfiguration*>(dialog->getConfigurationPage());
  KPushButton *button = findChild<KPushButton*>(config, buttonName);
  QVERIFY(button);

  QTimer::singleShot(delay, this, slot.toAscii().constData());
  QTest::mouseClick(button, Qt::LeftButton);
}

void testDialog::clickDialogOption(const QString& triggerName, int delay, const QString& slot)
{
  QWidget *visualView = getDialogView();
  KPushButton *button = findChild<KPushButton*>(visualView, QString("dialogOption%1").arg(triggerName));
  QVERIFY(button);

  QTimer::singleShot(delay, this, slot.toAscii().constData());
  QTest::mouseClick(button, Qt::LeftButton);
}


void testDialog::okDialog()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);

  QWidget *dlg = app->activeModalWidget();
  QVERIFY(dlg);
  int skip = 1;
  KPushButton *okButton = findChild<KPushButton*>(dlg, "", &skip);
  QVERIFY(okButton);
  QTest::mouseClick(okButton, Qt::LeftButton);
}

void testDialog::cancelDialog()
{
  if (!sender()) QSKIP("Internal slot, not a test by itself", SkipAll);
  QWidget *dlg = app->activeModalWidget();
  QVERIFY(dlg);
  QTest::keyClick(dlg, Qt::Key_Escape);
}

 
QTEST_APPLESS_MAIN(testDialog)

#include "dialogtest.moc"


