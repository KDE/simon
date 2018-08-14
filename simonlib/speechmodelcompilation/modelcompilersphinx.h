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


#ifndef MODELCOMPILERSPHINX_H
#define MODELCOMPILERSPHINX_H

#include "modelcompiler.h"
#include "simonmodelcompilationmanagement_export.h"

#define MODEL_POSTFIX ".cd_semi_" //depends on sphinxtrain config
#define MODEL_TYPE ".semi."
#define SENONES_COUNT "200"       //TODO: think about reading this values from that config
#define DEFAULT_SAMPRATE "16000"
#define DENSITIES_NUM "64" //look at senons

class MODELCOMPILATIONMANAGEMENT_EXPORT ModelCompilerSPHINX: public ModelCompiler
{
  Q_OBJECT
public:
  explicit ModelCompilerSPHINX(const QString& userName, QObject *parent=0);

  bool startCompilation(ModelCompiler::CompilationType compilationType, const QString& modelDestination,
                                const QStringList& droppedTranscriptions, const QString& baseModelPath, 
				const QHash<QString, QString>& args);

  
  QString information(bool condensed=false) const;
  
protected:
  bool parseConfiguration();
  bool processError();
  bool pack(const QString& targetArchive, const QString& name);
  
private:
  //programs
  QString m_SphinxTrain;
  QString m_Bw;
  QString m_Sphinx_fe;
  QString m_Map_adapt;
  QString m_Pocketsphinx_mdef_convert;
  QString m_Mllr_solve;

  QString m_ModelDir;
  QString m_ModelName;
  QString m_WavPath;
  QString m_ConfigPath;
  QString m_BaseModelPath;
  QString m_ModelDestination;
  QString m_AdaptingModelName;

  bool setupModel(const QString &modelDir, const QString &modelName);
  bool compileModel(const QString &modelDir, const QString &modelName);

  QHash<QString, QString> readFeatParams(const QString&);
  bool copyModelForAdapting(const QString &source, const QString &destination);
  bool generateAcousticFeatureFiles();
  bool convertMdef();
  bool getStatistics();
  bool mapUpdate();

  bool modifyConfig(const QString &filename, const QHash<QString, QString>& args);

  bool CompileLanguageModel();
  bool AdaptBaseModel();
  bool CompileWholeModel();

};

#endif // MODELCOMPILERSPHINX_H
