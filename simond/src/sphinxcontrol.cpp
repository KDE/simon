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
#include <speechmodelcompilation/modelmetadata.h>
#include <simonutils/fileutils.h>
#include <simonrecognizer/sphinxrecognitionconfiguration.h>
#include <simonrecognizer/sphinxrecognizer.h>

#include <KDebug>
#include <QDir>
#include <KStandardDirs>
#include <KLocale>


SphinxControl::SphinxControl(const QString& username, QObject* parent) : RecognitionControl(username, parent)
{
  recog = new SphinxRecognizer();
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

  QString path = KStandardDirs::locateLocal("tmp", "/simond/"+username+"/sphinx/");
  if(!QDir(path).entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst).isEmpty())
  {
    kDebug() << "Removing old data from working dir";
    FileUtils::removeDirRecursive(path);
    path = KStandardDirs::locateLocal("tmp", "/simond/"+username+"/sphinx/");
  }

  kDebug() << "Unpacking model to working dir";
  if (!FileUtils::unpackAll(modelPath, path))
  {
    return false;
  }

  QFile metadataFile(path+QLatin1String("metadata.xml"));
  if(!metadataFile.open(QIODevice::ReadOnly))
  {
    emit recognitionError(i18n("Failed to read metadata from \"%1\"", path+QLatin1String("metadata.xml")),
                          getBuildLog());
    return NULL;
  }
  ModelMetadata metadata;
  QDomDocument DomDocument;
  DomDocument.setContent(&metadataFile);
  metadata.deserializeXml(DomDocument.documentElement());

  metadataFile.close();

  modelName = metadata.name();

  kDebug() << "Emitting recognition ready";
  emit recognitionReady();
  return true;
}

RecognitionConfiguration *SphinxControl::setupConfig()
{
  kDebug() << "Setting config up";
  QString dirPath = KStandardDirs::locateLocal("tmp", "/simond/"+username+"/sphinx/");

  return new SphinxRecognitionConfiguration(dirPath, dirPath+modelName+QLatin1String(".jsgf"),
                                            dirPath+modelName+QLatin1String(".dic"), DEFAULT_SAMPRATE);
}

void SphinxControl::emitError(const QString &error)
{
  emit recognitionError(error, getBuildLog());
}
