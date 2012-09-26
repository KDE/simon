/*
 *   Copyright (C) 2012 Peter Grasch <grasch@simon-listens.org>
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
#include <KStandardDirs>
#include <KLocale>
#include <KDebug>
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
  connect(compiler, SIGNAL(phonemeUndefined(QString)), this, SIGNAL(phonemeUndefined(QString)));
}

void ModelCompilationManagerHTK::slotPhonemeUndefined ( const QString& phoneme )
{
  adapter->poisonPhoneme(phoneme);
  tryAgain = true;
}

void ModelCompilationManagerHTK::run()
{
  //first, adapt the input to htk readable formats using the adapter
  QHash<QString,QString> adaptionArgs;
  
  QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+userName+"/active/");

  ModelCompilationAdapter::AdaptionType adaptionType = (baseModelType == 0) ?
                                                         (ModelCompilationAdapter::AdaptLanguageModel) :
                                                         (ModelCompilationAdapter::AdaptionType) (ModelCompilationAdapter::AdaptAcousticModel|ModelCompilationAdapter::AdaptLanguageModel);
  
  adaptionArgs.insert("lexicon", activeDir+"lexicon");
  adaptionArgs.insert("grammar", activeDir+"model.grammar");
  adaptionArgs.insert("simpleVocab", activeDir+"simple.voca");
  adaptionArgs.insert("prompts", activeDir+"prompts");
  
  //then, compile the model using the model compilation manager
  QHash<QString,QString> compilerArgs;

  compilerArgs.insert("samples",KStandardDirs::locateLocal("appdata", "models/"+userName+"/samples/"));
  compilerArgs.insert("lexicon", activeDir+"lexicon");
  compilerArgs.insert("grammar", activeDir+"model.grammar");
  compilerArgs.insert("vocab", activeDir+"simple.voca");
  compilerArgs.insert("prompts", activeDir+"prompts");

  compilerArgs.insert("scriptBase", "simon/scripts");
  
  adapter->clearPoisonedPhonemes();
  
  do
  {
    if (!keepGoing) return;

    kDebug() << "Unpacking";
    if (baseModelType < 2) {
      QString baseModelFolder = KStandardDirs::locateLocal("tmp", KGlobal::mainComponent().aboutData()->appName()+'/'+userName+"/compile/base/");
      //base model needed - unpack it and fail if its not here
      if (!FileUtils::unpack(baseModelPath, baseModelFolder, (QStringList() << "hmmdefs" << "tiedlist" << "macros" << "stats"))) {
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
      kWarning() << "Model adaption failed for user " << userName;
      return;
    }
    if (!keepGoing) return;

    QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+userName+"/active/");

    QFileInfo fiGrammar(activeDir+"model.grammar");
    bool hasGrammar = (fiGrammar.size() > 0);

    if (!hasGrammar)
    {
      kDebug() << "No Grammar!  Model recompilation aborting!";
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
          kDebug() << "No Prompts!  Switching to static model!";
          baseModelType = 0;
          break;
        case 2:                                     //do not bother creating the model without prompts
          kDebug() << "No Prompts!  Model recompilation aborting!";
          emit modelCompilationAborted(ModelCompilation::InsufficientInput);
          return;
      }
    }
    
    ModelCompiler::CompilationType compilationType = getCompilationType(baseModelType);
    
    //build fingerprint and search cache for it
    uint fingerprint = 0;
    QStringList componentsToParse(QStringList() << "lexicon" << "model.grammar" << "simple.voca");
    if (baseModelType > 0)
      componentsToParse << "prompts";

    fingerprint = getFingerPrint(activeDir, componentsToParse, compilationType);

    bool exists;
    QString outPath = cachedModelPath(fingerprint, &exists);
    
    if (!keepGoing) return;
    
    if (exists) kDebug() << "Pulling compiled model from cache";

    if (exists || compiler->startCompilation(compilationType, outPath, adapter->getDroppedTranscriptions(), 
                                             baseModelPath, compilerArgs))
    {
      emit modelReady(fingerprint, outPath);
      keepGoing = false;
      return;
    } else
      kWarning() << "Model compilation failed for user " << userName;
  } while (tryAgain);
  keepGoing = false;
}
