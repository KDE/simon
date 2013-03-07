/*
 *   Copyright (C) 2013 Peter Grasch <peter.grasch@bedahr.org>
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
#include <QVariant>
#include <QWidget>
#include <KApplication>
#include <KCmdLineArgs>
#include <QSignalSpy>
#include <QDebug>
#include <QTimer>
#include "../atspiscanner.h"
#include "accessibleapp.h"


class testATSPIScanner: public QObject
{
  Q_OBJECT
private slots:
  void initTestCase();
  void testGeneral();
  void testMenu();
  void testBuddy();
  void testDialog();
  void cleanupTestCase();

private:
  ATSPIScanner *scanner;
  AccessibleApp *app;
  QSignalSpy *spy;

  static const int maxWaitIterations = 1000;
  static const int waitIterationDuration = 100;

  QStringList currentCommands();
  void blockingInvoke(void (AccessibleApp::*func)(), int minEvents = 1);
};

void testATSPIScanner::initTestCase()
{
  scanner = new ATSPIScanner;
  app = new AccessibleApp;
  spy = new QSignalSpy(scanner, SIGNAL(commandsShown(QStringList, bool)));
  QVERIFY(spy->isValid());

  QTimer::singleShot(waitIterationDuration, qApp, SLOT(quit()));
  qApp->exec();
}

void testATSPIScanner::cleanupTestCase()
{
  delete scanner;
  delete app;
  delete spy;
}

QStringList testATSPIScanner::currentCommands()
{
  QStringList c;
  while (!spy->isEmpty()) {
    QList<QVariant> reply = spy->takeFirst();
    Q_ASSERT(reply.count() == 2);
    if (reply.last().toBool() /* reset */)
      c.clear();
    foreach (const QString& command, reply.first().toStringList())
      if (!c.contains(command))
        c.append(command);
  }
  qSort(c);
  qDebug() << "Current commands: " << c;
  return c;
}

void testATSPIScanner::blockingInvoke(void (AccessibleApp::*func)(), int minEvents)
{
  spy->clear();
  (app->*func)();
  for (int i = 0; (i < maxWaitIterations) && (spy->count() < minEvents); ++i) {
    QTimer::singleShot(waitIterationDuration, qApp, SLOT(quit()));
    qApp->exec();
  }
}

void testATSPIScanner::testGeneral()
{
  blockingInvoke(&AccessibleApp::setupSingleButton, 1);
  QCOMPARE(currentCommands(), QStringList() << QLatin1String("Testbutton"));
  blockingInvoke(&AccessibleApp::changeButtonText, 1);
  QCOMPARE(currentCommands(), QStringList() << QLatin1String("Alternate Button"));
  blockingInvoke(&AccessibleApp::hideButton, 1);
  QCOMPARE(currentCommands(), QStringList());
  blockingInvoke(&AccessibleApp::showButton, 1);
  QCOMPARE(currentCommands(), QStringList() << QLatin1String("Alternate Button"));

  blockingInvoke(&AccessibleApp::clear, 1);
  QCOMPARE(currentCommands(), QStringList());
}


void testATSPIScanner::testMenu()
{
  //qDebug() << "========================";
  blockingInvoke(&AccessibleApp::setupMenu, 1);

  QEXPECT_FAIL("", "Bug # 314464", Continue);
  QCOMPARE(currentCommands(), QStringList() << QLatin1String("File") << QLatin1String("Settings"));

  //qDebug() << "========================";
  blockingInvoke(&AccessibleApp::clear, 1);
  QCOMPARE(currentCommands(), QStringList());

}

void testATSPIScanner::testBuddy()
{
  blockingInvoke(&AccessibleApp::setupBuddy, 1);
  QCOMPARE(currentCommands(), QStringList() << QLatin1String("Buddy"));

  blockingInvoke(&AccessibleApp::clear, 1);
  QCOMPARE(currentCommands(), QStringList());
}

void testATSPIScanner::testDialog()
{
  blockingInvoke(&AccessibleApp::setupDialog, 1);
  QCOMPARE(currentCommands(), QStringList() << QLatin1String("Ok") << QLatin1String("Text"));

  blockingInvoke(&AccessibleApp::clear, 1);
  QCOMPARE(currentCommands(), QStringList());

  blockingInvoke(&AccessibleApp::setupSingleButton, 1);
  QCOMPARE(currentCommands(), QStringList() << QLatin1String("Testbutton"));
  blockingInvoke(&AccessibleApp::setupDialog, 1);
  QCOMPARE(currentCommands(), QStringList() << QLatin1String("Ok") << QLatin1String("Text"));
  blockingInvoke(&AccessibleApp::acceptDialog, 1);
  QCOMPARE(currentCommands(), QStringList() << QLatin1String("Testbutton"));

  blockingInvoke(&AccessibleApp::clear, 1);
  QCOMPARE(currentCommands(), QStringList());
}

QTEST_MAIN(testATSPIScanner)

#include "atspiscannertest.moc"


