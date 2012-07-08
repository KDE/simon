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

#include "sphinxcontrol.h"
#include <KDebug>

SphinxControl::SphinxControl(const QString& username, QObject* parent) : RecognitionControl(username, parent)
{
}

bool SphinxControl::initializeRecognition(const QString &modelPath)
{
  if (modelPath == m_lastModel) return true; //already initialized / tried to initialize with this exact model

  m_lastModel = modelPath;
  kDebug() << "Initializing";
  if (isInitialized())
  {
    kDebug() << "Initializing recognition that was already initialized; uninitializing...";
    uninitialize();
    m_startRequests = 0;
  }

  //TODO: copy from model path| unpackfrom archive?

  kDebug() << "Emitting recognition ready";
  emit recognitionReady();
  return true;
}

RecognitionConfiguration *SphinxControl::setupConfig()
{
//  QByteArray dirPath = KStandardDirs::locateLocal("tmp", "/simond/"+username+"/sphinx/").toUtf8();

  //TODO: set proper paramets & create sphinxrecognition

  return NULL;
}

void SphinxControl::emitError(const QString &error)
{
}
