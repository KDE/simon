/*
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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

#ifndef JULIUSMODELTEST_H
#define JULIUSMODELTEST_H

#include "modeltest.h"

class SIMONMODELTEST_EXPORT JuliusModelTest : public ModelTest
{
  Q_OBJECT
public:
  explicit JuliusModelTest(const QString &userName, QObject *parent = 0);
  
  bool startTest(const QString& samplePath, const QString& promptsPath,
                 int sampleRate, const QHash<QString, QString> params);
signals:
  
public slots:

protected:
  QString hmmDefsPath, tiedListPath, dictPath, dfaPath;
  QString juliusJConf;
  
};

#endif // JULIUSMODELTEST_H
