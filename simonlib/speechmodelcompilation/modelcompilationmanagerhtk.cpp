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


#include "modelcompilationmanagerhtk.h"
#include "modelcompilerhtk.h"
#include "modelcompilationadapterhtk.h"
#include <simonutils/fileutils.h>
#include <QFileInfo>

#include <KLocale>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <KAboutData>

ModelCompilationManagerHTK::ModelCompilationManagerHTK(const QString& userName, QObject *parent) : ModelCompilationManager(userName, parent)
{
  compiler = new ModelCompilerHTK(userName, this);
  adapter = new ModelCompilationAdapterHTK(userName, this);

  tryAgain = false;

  connect(adapter, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
  connect(adapter, SIGNAL(adaptionAborted(ModelCompilation::AbortionReason)), this, SIGNAL(modelCompilationAborted(ModelCompilation::AbortionReason)));
  connect(adapter, SIGNAL(status(QString,int,int)), this, SIGNAL(status(QString,int,int)));

  connect(compiler, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
  connect(compiler, SIGNAL(status(QString,int,int)), this, SIGNAL(status(QString,int,int)));
  connect(compiler, SIGNAL(activeModelCompilationAborted(ModelCompilation::AbortionReason)), this, SIGNAL(modelCompilationAborted(ModelCompilation::AbortionReason)));

  connect(compiler, SIGNAL(wordUndefined(QString)), this, SIGNAL(wordUndefined(QString)));
  connect(compiler, SIGNAL(classUndefined(QString)), this, SIGNAL(classUndefined(QString)));
  connect(compiler, SIGNAL(phonemeUndefined(QString)), this, SLOT(slotPhonemeUndefined(QString)), Qt::DirectConnection);
}

void ModelCompilationManagerHTK::run()
{
  //first, adapt the input to htk readable formats using the adapter
  QHash<QString,QString> adaptionArgs;

  QString activeDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "models/"+userName+"/active/";

  ModelCompilationAdapter::AdaptionType adaptionType = (baseModelType == 0) ?
                                                         (ModelCompilationAdapter::AdaptLanguageModel) :
                                                         (ModelCompilationAdapter::AdaptionType) (ModelCompilationAdapter::AdaptAcousticModel|ModelCompilationAdapter::AdaptLanguageModel);

  adaptionArgs.insert("lexicon", activeDir+"lexicon");
  adaptionArgs.insert("grammar", activeDir+"model.grammar");
  adaptionArgs.insert("simpleVocab", activeDir+"simple.voca");
  adaptionArgs.insert("prompts", activeDir+"prompts");

  //then, compile the model using the model compilation manager
  QHash<QString,QString> compilerArgs;

  compilerArgs.insert("samples",QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "models/"+userName+"/samples/");
  compilerArgs.insert("lexicon", activeDir+"lexicon");
  compilerArgs.insert("grammar", activeDir+"model.grammar");
  compilerArgs.insert("vocab", activeDir+"simple.voca");
  compilerArgs.insert("prompts", activeDir+"prompts");

  compilerArgs.insert("scriptBase", "simon/scripts");

  adapter->clearPoisonedPhonemes();

  QStringList additionalOutPaths;
  do
  {
    if (!keepGoing) return;

    if (baseModelType < 2)
    {
      QString baseModelFolder = QDir::tempPath() + QLatin1Char('/') + KAboutData::applicationData().productName()+'/'+userName+"/compile/base/";
      //base model needed - unpack it and fail if its not here
      if (!FileUtils::unpack(baseModelPath, baseModelFolder, (QStringList() << "hmmdefs" << "tiedlist" << "macros" << "stats")))
      {
        emit error(i18nc("%1 is path to the base model", "Could not open base model at \"%1\".", baseModelPath));
        return;
      }
      adaptionArgs.insert("base/hmmdefs", baseModelFolder+"hmmdefs");
      adaptionArgs.insert("base/tiedlist", baseModelFolder+"tiedlist");
      adaptionArgs.insert("base/macros", baseModelFolder+"macros");
      adaptionArgs.insert("base/stats", baseModelFolder+"stats");
      compilerArgs.insert("base/hmmdefs", baseModelFolder+"hmmdefs");
      compilerArgs.insert("base/tiedlist", baseModelFolder+"tiedlist");
      compilerArgs.insert("base/macros", baseModelFolder+"macros");
      compilerArgs.insert("base/stats", baseModelFolder+"stats");
    }

    tryAgain = false;
    if (!adapter->startAdaption(adaptionType, scenarioPaths, promptsPathIn, adaptionArgs))
    {
      qWarning() << "Model adaption failed for user " << userName;
      return;
    }
    if (!keepGoing) return;

    QString activeDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "models/"+userName+"/active/";

    QFileInfo fiGrammar(activeDir+"model.grammar");
    bool hasGrammar = (fiGrammar.size() > 0);

    if (!hasGrammar)
    {
      qDebug() << "No Grammar!  Model recompilation aborting!";
      emit modelCompilationAborted(ModelCompilation::InsufficientInput);
      return;
    }

    QFileInfo fiPrompts(activeDir+"prompts");
    bool hasPrompts = (fiPrompts.size() > 0);
    if (!hasPrompts)
    {
      switch (baseModelType)
      {
        case 1:
          qDebug() << "No Prompts!  Switching to static model!";
          baseModelType = 0;
          break;
        case 2:                                     //do not bother creating the model without prompts
          qDebug() << "No Prompts!  Model recompilation aborting!";
          emit modelCompilationAborted(ModelCompilation::InsufficientInput);
          return;
      }
    }

    ModelCompiler::CompilationType compilationType = getCompilationType(baseModelType);

    //build fingerprint and search cache for it
    uint fingerprint = 0;
    QStringList componentsToParse(QStringList() << activeDir + "lexicon" << activeDir + "model.grammar" << activeDir + "simple.voca");
    if (baseModelType > 0)
      componentsToParse << activeDir + "prompts";
    if (baseModelType < 2)
      componentsToParse << baseModelPath;

    fingerprint = getFingerPrint(componentsToParse, compilationType);

    bool exists;
    QString outPath = cachedModelPath(fingerprint, &exists);

    if (!keepGoing) return;

    if (exists) qDebug() << "Pulling compiled model from cache";

    if (exists || compiler->startCompilation(compilationType, outPath, adapter->getDroppedTranscriptions(),
                                             baseModelPath, compilerArgs))
    {
      //we had to re-try; all old, intermediate fingerprints are going to end up here; short circuit this for the next time
      foreach (const QString& copy, additionalOutPaths)
        QFile::copy(outPath, copy);
      emit modelReady(fingerprint, outPath);
      keepGoing = false;
      return;
    } else
      qWarning() << "Model compilation failed for user " << userName;
    additionalOutPaths << outPath;
  } while (tryAgain);
  keepGoing = false;
}
