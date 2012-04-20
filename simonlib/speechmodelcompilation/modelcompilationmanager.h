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

#ifndef SIMON_MODELCOMPILATIONMANAGER_H_18BCB183592A48D3962815FFDEA7C214
#define SIMON_MODELCOMPILATIONMANAGER_H_18BCB183592A48D3962815FFDEA7C214

#include <QThread>
#include <speechmodelcompilation/modelcompilationadapter.h>
#include <speechmodelcompilation/modelcompiler.h>
#include "simonmodelcompilationmanagement_export.h"

class MODELCOMPILATIONMANAGEMENT_EXPORT ModelCompilationManager : public QThread
{
Q_OBJECT

signals:
  void modelReady(uint fingerprint, const QString& path);
  void modelCompilationAborted();
  void status(QString mesg, int now, int max);
  void error(QString);
  
  void wordUndefined(const QString&);
  void classUndefined(const QString&);
  void phonemeUndefined(const QString&);

public:
  explicit ModelCompilationManager ( const QString& userName, QObject* parent = 0 );
  
  QString cachedModelPath( uint fingerprint, bool* exists=0 );
  
  void startModelCompilation(int baseModelType, const QString& baseModelPath,
                             const QStringList& scenarioPaths, const QString& promptsPathIn);
  virtual void abort();
  
  bool hasBuildLog() const;
  QString getGraphicBuildLog() const;
  QString getBuildLog() const;
  
  int wordCount() const;
  int pronunciationCount() const;
  int sampleCount() const;
  
protected:
  QString userName;
  bool keepGoing;
  int baseModelType;
  QString baseModelPath;
  QStringList scenarioPaths;
  QString promptsPathIn;
  QString modelPathOut;
  
  ModelCompiler *compiler;
  ModelCompilationAdapter *adapter;
  
  virtual void run()=0;
};

#endif
