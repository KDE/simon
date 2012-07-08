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

#ifndef SIMON_MODELCOMPILER_H_18BCB183592A48D3962815FFDEA7C214
#define SIMON_MODELCOMPILER_H_18BCB183592A48D3962815FFDEA7C214

#include <QObject>
#include <QString>
#include <QHash>
#include "simonmodelcompilationmanagement_export.h"
#include <QMutex>
#include <QProcess>

class MODELCOMPILATIONMANAGEMENT_EXPORT ModelCompiler : public QObject
{
  Q_OBJECT
signals:
  void status(QString, int progressNow, int progressTotal=2600);
  void error(QString);

  void wordUndefined(const QString&);
  void classUndefined(const QString&);
  void phonemeUndefined(const QString&);

  void modelCompiled();
  void activeModelCompilationAborted();

public:
  enum CompilationType
  {
    CompileLanguageModel=1,
    CompileSpeechModel=2,
    AdaptSpeechModel=4
  };

  virtual bool startCompilation(ModelCompiler::CompilationType compilationType, const QString& modelDestination,
                                const QString& baseModelPath, const QHash<QString, QString>& args)=0;

  explicit ModelCompiler(const QString& userName, QObject *parent=0) : QObject(parent), userName(userName) {}

  virtual bool hasBuildLog() const;
  virtual QString getGraphicBuildLog() const;
  virtual QString getBuildLog() const;

  virtual void abort();

  virtual QString information(bool condensed=false) const=0;

protected:
  bool keepGoing;
  
  QString userName;
  QString tempDir;

  QMutex buildLogMutex;
  QByteArray buildLog;
  
  CompilationType compilationType;

  QList<QProcess*> activeProcesses;

  virtual bool processError()=0;
  virtual bool parseConfiguration()=0;

  virtual bool pack( const QString& targetArchive, const QString& name )=0;
//  virtual bool unpack(const QString& archive, const QString& targetDir)=0;

  virtual QString getMetaData(const QString &name, const QString &type);
  
  bool execute(const QString& command, const QString &wDir);
  
  virtual void clearLog();
  void analyseError(QString readableError);

protected slots:
   void addStatusToLog(const QString &status);
};
#endif
