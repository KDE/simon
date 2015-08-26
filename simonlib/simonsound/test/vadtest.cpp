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

#include "vadtest.h"
#include "../vadsoundprocessor.h"
#include "../soundserver.h"
#include "../simonsound.h"

#include <QPair>
#include <QList>
#include <QDebug>
#include <QFile>
#include <QSignalSpy>

static const QString dataFolder = KDESRCDIR "data/";
static const SimonSound::DeviceConfiguration deviceConfiguration(QString(), 1, 16000, false, 16000);
static const qint64 chunkLength = 10 /* 10 msecs */;
static const qint64 chunkSize = SoundServer::getInstance()->lengthToByteSize(chunkLength, deviceConfiguration);
typedef QPair<double, double> Segment;

void VADTest::initTestCase()
{
  vad = new VADSoundProcessor(deviceConfiguration, false);
  spy = new QSignalSpy(vad, SIGNAL(complete(qint64, qint64)));
}

void VADTest::cleanupTestCase()
{
  delete spy;
  delete vad;
}

void VADTest::testCommands()
{
  QFETCH(QString, rawData);
  QFETCH(QString, labelFile);

  QFile labels(labelFile);
  QVERIFY(labels.open(QIODevice::ReadOnly));

  QList<Segment> segments;
  while (!labels.atEnd()) {
    QString line(QString::fromUtf8(labels.readLine()));
    QStringList parts = line.split('\t');

    QCOMPARE(parts.count(), 3);

    double startTime = qRound(parts[0].toDouble() * 1000.0);
    double endTime = qRound(parts[1].toDouble() * 1000.0);
    segments << qMakePair(startTime, endTime);
    qDebug() << startTime << "-" << endTime;
  }
  labels.close();
  qDebug() << "Read" << segments.count() << "segments";

  QFile data(rawData);
  QVERIFY(data.open(QIODevice::ReadOnly));
  qint64 currentTime = 0;
  qint64 totalSize = 0;
  while (!data.atEnd()) {
    QByteArray thisBuffer(data.read(chunkSize));
    totalSize += thisBuffer.length();
    qint64 thisCurrentTime = currentTime;
    vad->process(thisBuffer, thisCurrentTime);
    currentTime += chunkLength;
  }
  qDebug() << "Current time: " << currentTime;
  qDebug() << "Total size: " << totalSize;
  qDebug() << "Chunk size: " << chunkSize;
  qDebug() << "Chunk length: " << chunkLength;
  qDebug() << "Total size / chunkSize: " << totalSize / chunkSize;
  qDebug() << "(Total size / chunkSize) * chunkLength: " << (totalSize / chunkSize) * chunkLength;

  QList<Segment> detectedSegments;
  while (spy->count() > 0) {
    QList<QVariant> arguments = spy->takeFirst();
    QCOMPARE(arguments.count(), 2);
    double startTime = arguments[0].toDouble();
    double endTime   = arguments[1].toDouble();
    detectedSegments << qMakePair(startTime, endTime);
    qDebug() << startTime << "-" << endTime;
  }
  qDebug() << "Detected" << segments.count() << "segments";

  QCOMPARE(segments.count(), detectedSegments.count());

  for (int i = 0; i < segments.count(); ++i) {
    QVERIFY(qAbs(segments[i].first - detectedSegments[i].first) <= 100);
    QVERIFY(qAbs(segments[i].second - detectedSegments[i].second) <= 100);
    qDebug() << "Okay: " << i;
  }
}

void VADTest::testCommands_data()
{
  QTest::addColumn<QString>("rawData");
  QTest::addColumn<QString>("labelFile");

  QTest::newRow("testCommand0") << dataFolder + "simple.raw" << dataFolder + "simple.labels";
  QTest::newRow("testCommand1") << dataFolder + "commands.raw" << dataFolder + "commands.labels";
}

QTEST_MAIN(VADTest)
