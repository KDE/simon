/*
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_MODELCOMPILATIONMANAGER_H_18BCB183592A48D3962815FFDEA7C214
#define SIMON_MODELCOMPILATIONMANAGER_H_18BCB183592A48D3962815FFDEA7C214

#include "simonmodelcompilationmanagement_export.h"

#include <speechmodelcompilation/modelcompilationadapter.h>
#include <speechmodelcompilation/modelcompiler.h>

#include <QThread>

/*!
 * \class ModelCompilationManager
 * \brief The ModelCompilationManager class manage model creation process.
 *        It takes data from simon, transform them, using adapter and create model, using compiler.
 *
 *  \version 0.1
 *  \date 14.08.2012
 *  \author Vladislav Sitalo
 */

class MODELCOMPILATIONMANAGEMENT_EXPORT ModelCompilationManager : public QThread
{
Q_OBJECT

signals:
  void modelReady(uint fingerprint, const QString& path);
  void modelCompilationAborted(ModelCompilation::AbortionReason);
  void status(QString mesg, int now, int max);
  void error(QString);

  void wordUndefined(const QString&);
  void classUndefined(const QString&);
  void phonemeUndefined(const QString&);

public:
  explicit ModelCompilationManager ( const QString& userName, QObject* parent = 0 );
  virtual ~ModelCompilationManager();

  QString cachedModelPath( uint fingerprint, bool* exists=0 );

  /*!
   * \brief startModelCompilation Function which starts model compilation process in separate thread.
   * \param baseModelType
   * \param baseModelPath
   * \param scenarioPaths Scenarios list.
   * \param promptsPathIn Path to prompts file.
   */
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
  bool keepGoing;
  QString userName;
  int baseModelType;
  QString baseModelPath;
  QStringList scenarioPaths;
  QString promptsPathIn;
  QString modelPathOut;

  ModelCompiler *compiler;
  ModelCompilationAdapter *adapter;

  // this is set to true when we receive note that a problem during compilation was fixed by changing the adaption
  // parameters. On the next failed compilation, the adaption (and subsequently the compilation) will be triggered
  // again
  bool tryAgain;

  virtual void run()=0;

  uint getFingerPrint(QString dir, QStringList files, ModelCompiler::CompilationType compilationType);
  ModelCompiler::CompilationType getCompilationType(int baseModelType);

protected slots:
  void slotPhonemeUndefined(const QString& phoneme);
};

#endif
