/*
 *   Copyright (C) 2014 Peter Grasch <peter.grasch@bedahr.org>
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

#include "../contextmanager.h"
#include "../processinfo.h"

#include <QTest>
#include <QSignalSpy>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

#include <KProcess>

#include <KDELibs4Support/kapplication.h>
#include <QCommandLineParser>

class processInfoGathererTest: public QObject
{
  Q_OBJECT
  private slots:
    void initTestCase();
    void cleanupTestCase();
    void testProcessTracking();
  private:
    KApplication *app;
    KProcess *proc;
};

void processInfoGathererTest::initTestCase()
{
  char *appName = new char[5];
  strcpy(appName, "test");
  char **argv = new char*[1];
  *argv = appName;
  KAboutData aboutData( QLatin1String("test"), i18n("appname"), QLatin1String("0.1"));
    QApplication app(argc, argv);
    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    //PORTING SCRIPT: adapt aboutdata variable if necessary
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

  app = new KApplication(true);
  proc = new KProcess();
#ifndef Q_OS_MAC
  proc->setProgram(qApp->applicationDirPath() + QDir::separator() + "dummyapplication");
#else
  proc->setProgram(qApp->applicationDirPath() + "/../../../dummyapplication.app/Contents/MacOS/dummyapplication");
#endif
  
  delete[] argv;
  delete[] appName;
}

void processInfoGathererTest::cleanupTestCase()
{
  delete proc;
  delete app;
}

void processInfoGathererTest::testProcessTracking()
{
  QString thisExecutable = QFileInfo(qApp->applicationFilePath()).fileName();
  ProcessInfo::instance(); //setting up

#ifdef Q_OS_MAC
  qWarning() << "If this test fails, make sure that you *disabled* the option to change the terminal's window title "
                "according to the running application in it's preferences as this interferes with the test.";
#endif

  QTest::qWait(1200);

  QSignalSpy spyAdd(ProcessInfo::instance(), SIGNAL(processAdded(QString)));
  QSignalSpy spyRem(ProcessInfo::instance(), SIGNAL(processRemoved(QString)));
  QSignalSpy spyTitleChanged(ProcessInfo::instance(), SIGNAL(activeWindowTitleChanged(QString)));
  QSignalSpy spyProcessChanged(ProcessInfo::instance(), SIGNAL(activeWindowProcessChanged(QString)));
  QVERIFY(spyAdd.isValid() && spyRem.isValid() && spyTitleChanged.isValid() && spyProcessChanged.isValid());

  QVERIFY(ProcessInfo::instance()->getRunningProcesses().contains(thisExecutable));
  QVERIFY(!ProcessInfo::instance()->getRunningProcesses().contains("dummyapplication"));

  proc->start();
  proc->waitForStarted();
  QTest::qWait(1200);

  QVERIFY(ProcessInfo::instance()->getRunningProcesses().contains(thisExecutable));
  QVERIFY(ProcessInfo::instance()->getRunningProcesses().contains("dummyapplication"));

  QCOMPARE(spyAdd.count(), 1);
  QCOMPARE(spyRem.count(), 0);
  QCOMPARE(spyTitleChanged.count(), 1);
  QCOMPARE(spyProcessChanged.count(), 1);
  QList<QVariant> arguments;

  arguments = spyAdd.takeFirst();
  QVERIFY(arguments.at(0).type() == QVariant::String);
  QCOMPARE(arguments.at(0).toString(), QString("dummyapplication"));

  arguments = spyTitleChanged.takeFirst();
  QVERIFY(arguments.at(0).type() == QVariant::String);
  QCOMPARE(arguments.at(0).toString(), QString("Test Dummy Application"));

  arguments = spyProcessChanged.takeFirst();
  QVERIFY(arguments.at(0).type() == QVariant::String);
  QCOMPARE(arguments.at(0).toString(), QString("dummyapplication"));

  proc->terminate();
  proc->waitForFinished();
  QTest::qWait(1200);

  QVERIFY(ProcessInfo::instance()->getRunningProcesses().contains(thisExecutable));
  QVERIFY(!ProcessInfo::instance()->getRunningProcesses().contains("dummyapplication"));
  QCOMPARE(spyAdd.count(), 0);
  QCOMPARE(spyRem.count(), 1);
  QCOMPARE(spyTitleChanged.count(), 1);
  QCOMPARE(spyProcessChanged.count(), 1);

  arguments = spyRem.takeFirst();
  QVERIFY(arguments.at(0).type() == QVariant::String);
  QCOMPARE(arguments.at(0).toString(), QString("dummyapplication"));

  arguments = spyTitleChanged.takeFirst();
  QVERIFY(arguments.at(0).type() == QVariant::String);

  arguments = spyProcessChanged.takeFirst();
  QVERIFY(arguments.at(0).type() == QVariant::String);
}


QTEST_APPLESS_MAIN(processInfoGathererTest)

#include "eventhandlertest.moc"
