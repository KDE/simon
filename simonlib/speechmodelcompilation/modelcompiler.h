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
#include "modelcompilation.h"

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
    void activeModelCompilationAborted(ModelCompilation::AbortionReason);

  public:
    enum CompilationType
    {
      CompileLanguageModel=1,
      CompileSpeechModel=2,
      AdaptSpeechModel=4
    };
    
    virtual bool startCompilation(ModelCompiler::CompilationType compilationType, const QString& modelDestination,
                                  const QString& baseModelPath, const QHash<QString, QString>& args)=0;

    explicit ModelCompiler(const QString& userName, QObject *parent=0);

    virtual bool hasBuildLog() const=0;
    virtual QString getGraphicBuildLog() const=0;
    virtual QString getBuildLog() const=0;

    virtual void abort()=0;

    virtual QString information(bool condensed=false) const=0;

  protected:
    QString userName;
};
#endif
