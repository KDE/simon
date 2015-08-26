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


#ifndef SIMON_VADTEST_H_D0C0BA2429B04F65935956A32C79BB09
#define SIMON_VADTEST_H_D0C0BA2429B04F65935956A32C79BB09

#include <QSignalSpy>

class VADSoundProcessor;
class QSignalSpy;

class VADTest: public QObject
{
  Q_OBJECT
  public:
    virtual ~VADTest() {}
  private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCommands();
    void testCommands_data();

  private:
    VADSoundProcessor *vad;
    QSignalSpy *spy;
};

#endif

