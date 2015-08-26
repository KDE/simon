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
#include <simonscenarios/modelmetadata.h>
#include <simonutils/fileutils.h>
#include <simonrecognizer/sphinxrecognitionconfiguration.h>
#include <simonrecognizer/sphinxrecognizer.h>

#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <KLocale>


SphinxControl::SphinxControl(const QString& username, QObject* parent) : RecognitionControl(username, RecognitionControl::SPHINX, parent)
{
  recog = new SphinxRecognizer();
}

bool SphinxControl::initializeRecognition(const QString &modelPath)
{
  qDebug() << "Initializing to " << modelPath << m_lastModel;
  if (modelPath != m_lastModel) { //already initialized / tried to initialize with this exact model
    m_lastModel = modelPath;
    qDebug() << "Initializing";
    uninitialize();
    m_startRequests = 0;

    QString path = QDir::tempPath() + QLatin1Char('/') +  "/simond/"+username+"/sphinx/";
    if(!QDir(path).entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst).isEmpty())
    {
      qDebug() << "Removing old data from working dir";
      FileUtils::removeDirRecursive(path);
      path = QDir::tempPath() + QLatin1Char('/') +  "/simond/"+username+"/sphinx/";
    }

    qDebug() << "Unpacking model to working dir";
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
  }
  qDebug() << "Emitting recognition ready";
  emit recognitionReady();
  return true;
}

RecognitionConfiguration *SphinxControl::setupConfig()
{
  qDebug() << "Setting config up";
  QString dirPath = QDir::tempPath() + QLatin1Char('/') +  "/simond/"+username+"/sphinx/";

  return new SphinxRecognitionConfiguration(dirPath, dirPath+modelName+QLatin1String(".jsgf"),
                                            dirPath+modelName+QLatin1String(".dic"), DEFAULT_SAMPRATE);
}

void SphinxControl::emitError(const QString &error)
{
  emit recognitionError(error, getBuildLog());
}
