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

#include "../eventhandler.h"

#include <QTest>
#include <QSignalSpy>
#include <QDebug>
#include <QMouseEvent>
#include <QLineEdit>

#include <KPushButton>
#include <KCmdLineArgs>
#include <KApplication>

class eventHandlerTest: public QObject
{
  Q_OBJECT
  private slots:
    void initTestCase();
    void cleanupTestCase();
    void testClick();
    void testDragAndDrop();
    void testSendWord();
    void testSendShortcut();
    
  private:
    KApplication *app;
    
};

class MMBButton : public KPushButton
{
  Q_OBJECT
signals:
  void rightClicked();
  void middleClicked();
public:
  MMBButton() : KPushButton("Test", 0) {}
  void mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton)
      emit rightClicked();
    else if (event->button() == Qt::MiddleButton)
      emit middleClicked();
    KPushButton::mousePressEvent(event);
  }
};

class DragTester : public QWidget
{
  Q_OBJECT
private:
  enum DragTestState {
    Initial,
    Dragging,
    Dropped
  };
public:
  DragTester() : QWidget(0), currentState(Initial) {
    setAcceptDrops(true);
  }
  void dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        event->acceptProposedAction();
    } else {
      event->ignore();
    }
  }
  void dragMoveEvent(QDragMoveEvent *event) {
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        event->acceptProposedAction();
    } else {
      event->ignore();
    }
  }
  void dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        event->acceptProposedAction();
      currentState = Dropped;
    } else {
      event->ignore();
    }
  }
  void mousePressEvent(QMouseEvent *event) {
    QByteArray itemData = "test";
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);;
    drag->setHotSpot(event->pos());
    currentState = Dragging;
    drag->exec();
  }
  bool completed() const {
    return currentState == Dropped;
  }
private:
  DragTestState currentState;
  
};
  

void eventHandlerTest::initTestCase()
{
  char *appName = new char[5];
  strcpy(appName, "test");
  char **argv = new char*[1];
  *argv = appName;
  KCmdLineArgs::init(1, argv, "test", "test", ki18n("appname"), "0.1");

  app = new KApplication(true);
  
  delete[] argv;
  delete[] appName;
}

void eventHandlerTest::cleanupTestCase()
{
  delete app;
}

void eventHandlerTest::testClick()
{
  MMBButton *btn = new MMBButton();
  btn->setWindowFlags(Qt::FramelessWindowHint);
  btn->move(0,0);
  btn->resize(100,100);
  btn->show();
  QTest::qWait(200);
  QSignalSpy spyL(btn, SIGNAL(clicked()));
  QSignalSpy spyR(btn, SIGNAL(rightClicked()));
  QSignalSpy spyM(btn, SIGNAL(middleClicked()));
  QVERIFY(spyL.isValid() && spyR.isValid() && spyM.isValid());
  
  //left click
  EventHandler::getInstance()->click(50, 50, EventSimulation::LMB);
  QTest::qWait(200);
  QCOMPARE(spyL.count(), 1);
  QCOMPARE(spyR.count(), 0);
  QCOMPARE(spyM.count(), 0);
  
  //left double click
  EventHandler::getInstance()->click(50, 50, EventSimulation::LMBDouble);
  QTest::qWait(200);
  QCOMPARE(spyL.count(), 3);
  QCOMPARE(spyR.count(), 0);
  QCOMPARE(spyM.count(), 0);
  
  //right click
  EventHandler::getInstance()->click(50, 50, EventSimulation::RMB);
  QTest::qWait(200);
  QCOMPARE(spyR.count(), 1);
  QCOMPARE(spyL.count(), 3);
  QCOMPARE(spyM.count(), 0);
  
  //middle click
  EventHandler::getInstance()->click(50, 50, EventSimulation::MMB);
  QTest::qWait(200);
  QCOMPARE(spyM.count(), 1);
  QCOMPARE(spyL.count(), 3);
  QCOMPARE(spyR.count(), 1);
  
  delete btn;
}

void eventHandlerTest::testDragAndDrop()
{
  DragTester *dragTester = new DragTester();
  dragTester->setWindowFlags(Qt::FramelessWindowHint);
  dragTester->move(0,0);
  dragTester->resize(500,500);
  dragTester->show();
  QTest::qWait(200);
  EventHandler::getInstance()->dragAndDrop(40, 40, 80, 80);
  QTest::qWait(1000);
  QVERIFY(dragTester->completed());
  delete dragTester;
}

void eventHandlerTest::testSendShortcut()
{
  QPushButton *btn = new QPushButton("&Foo");
  btn->setWindowFlags(Qt::FramelessWindowHint);
  btn->move(0,0);
  btn->resize(100,100);
  btn->show();
  QTest::qWait(200);
  QSignalSpy spy(btn, SIGNAL(clicked()));
  int fireCount = 0;
  
  QStringList shortcuts;
  shortcuts << "Alt+F" << "Ctrl+T" << "Ctrl+Shift+P";
  
  foreach (const QString& shortcut, shortcuts) {
    QKeySequence keys(shortcut);
    btn->setShortcut(keys);
    EventHandler::getInstance()->sendShortcut(keys);
    QTest::qWait(500);
    ++fireCount;
    QCOMPARE(spy.count(), fireCount);
  }
  delete btn;
}

void eventHandlerTest::testSendWord()
{
  QLineEdit *leTest = new QLineEdit();
  leTest->show();
  QStringList words;
  words << "Test" << "!@#$%&*()_";
  QTest::qWait(300);
  
  foreach (const QString& word, words) {
    leTest->clear();
    EventHandler::getInstance()->sendWord(word);
    QTest::qWait(300);
    QCOMPARE(leTest->text(), word);
  }
  delete leTest;
}

QTEST_APPLESS_MAIN(eventHandlerTest)

#include "eventhandlertest.moc"
