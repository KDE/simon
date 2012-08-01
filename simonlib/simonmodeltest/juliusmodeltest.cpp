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

#include <simonrecognizer/juliusstaticrecognitionconfiguration.h>
#include <simonrecognizer/juliusrecognizer.h>
#include "juliusmodeltest.h"

JuliusModelTest::JuliusModelTest(const QString& userName, QObject *parent):  ModelTest(userName, parent)
{
  recog = new JuliusRecognizer();
}

bool JuliusModelTest::startTest(const QString& samplePath, const QString& promptsPath,
                                int sampleRate, const QHash<QString, QString> params)
{
  abort();
  wait();

  this->hmmDefsPath = params.value("hmmDefsPath");
  this->tiedListPath = params.value("tiedListPath");
  this->dictPath = params.value("dictPath");
  this->dfaPath = params.value("dfaPath");
  this->juliusJConf = params.value("juliusJConf");

  this->samplePath = samplePath;

  this->promptsPath = promptsPath;

  this->sampleRate = sampleRate;

  keepGoing=true;

  buildLog="";

  if (!parseConfiguration())
    return false;

  config = new JuliusStaticRecognitionConfiguration(juliusJConf, dfaPath, dictPath, hmmDefsPath,
                                                    tiedListPath, QString::number(sampleRate));

  start();
  return true;
}
