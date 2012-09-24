/*
 *   Copyright (C) 2012 Peter Grasch <grasch@simon-listens.org>
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

#include "modelcompilationmanager.h"
#include <KStandardDirs>
#include <KDebug>
#include <QFile>

ModelCompilationManager::ModelCompilationManager ( const QString& userName, QObject* parent ) : 
    QThread ( parent ), keepGoing(false), userName(userName), compiler(0), adapter(0)
{
}

ModelCompilationManager::~ModelCompilationManager()
{
  abort();
  delete adapter;
  delete compiler;
}


QString ModelCompilationManager::cachedModelPath ( uint fingerprint, bool* exists )
{
  QString path = KStandardDirs::locateLocal("appdata", "models/"+userName+"/active/"+QString::number(fingerprint)+".sbm");
  if (exists) 
    *exists = QFile::exists(path);
  return path;
}

void ModelCompilationManager::startModelCompilation ( int baseModelType, const QString& baseModelPath, const QStringList& scenarioPaths, const QString& promptsPathIn )
{
  this->baseModelType = baseModelType;
  this->baseModelPath = baseModelPath;
  this->scenarioPaths = scenarioPaths;
  this->promptsPathIn = promptsPathIn;
  
  keepGoing = true;
  
  start();
}

void ModelCompilationManager::abort()
{  
  if (!keepGoing) return;
  keepGoing = false;
  
  if (adapter) adapter->abort();
  if (compiler) compiler->abort();

  if (!wait(3000)) {
    terminate();
    wait();
  }
  emit modelCompilationAborted(ModelCompilation::Manual);
}

QString ModelCompilationManager::getBuildLog() const
{
  Q_ASSERT(compiler);
  return compiler->getBuildLog();
}
QString ModelCompilationManager::getGraphicBuildLog() const
{
  Q_ASSERT(compiler);
  return compiler->getGraphicBuildLog();
}
bool ModelCompilationManager::hasBuildLog() const
{
  Q_ASSERT(compiler);
  return compiler->hasBuildLog();
}

int ModelCompilationManager::pronunciationCount() const
{
  return adapter->pronunciationCount();
}
int ModelCompilationManager::sampleCount() const
{
  return adapter->sampleCount();
}
int ModelCompilationManager::wordCount() const
{
  return adapter->wordCount();
}
