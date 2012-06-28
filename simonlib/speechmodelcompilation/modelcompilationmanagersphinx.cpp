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


#include "modelcompilationmanagersphinx.h"



void ModelCompilationManagerSPHINX::run()
{
  //first, adapt the input to sphinx readable formats using the adapter
  QHash<QString,QString> adaptionArgs;

  QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+userName+"/active/");

  ModelCompilationAdapter::AdaptionType adaptionType = (baseModelType == 0) ?
                                                         (ModelCompilationAdapter::AdaptLanguageModel) :
                                                         (ModelCompilationAdapter::AdaptionType) (ModelCompilationAdapter::AdaptAcousticModel|ModelCompilationAdapter::AdaptLanguageModel);

  QString modelName = "userDefinedModel"; //TODO: move name to config
  adaptionArgs.insert("workingDir", activeDir);
  adaptionArgs.insert("modelName", modelName);

  //then, compile the model using the model compilation manager
  QHash<QString,QString> compilerArgs;

//  compilerArgs.insert("samples",KStandardDirs::locateLocal("appdata", "models/"+userName+"/samples/"));
//  compilerArgs.insert("lexicon", activeDir+"lexicon");
//  compilerArgs.insert("grammar", activeDir+"model.grammar");
//  compilerArgs.insert("vocab", activeDir+"simple.voca");
//  compilerArgs.insert("prompts", activeDir+"prompts");
//  compilerArgs.insert("scriptBase", "simon/scripts");

  adapter->clearPoisonedPhonemes();

  do
  {
    if (!keepGoing) return;

    tryAgain = false;
    if (!adapter->startAdaption(adaptionType, scenarioPaths, promptsPathIn, adaptionArgs))
    {
      kWarning() << "Model adaption failed for user " << userName;
      return;
    }
    if (!keepGoing) return;

//    QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+userName+"/active/");

    QString fetc = activeDir+"/"+modelName+"/etc/"+modelName;

    QFileInfo fiGrammar(fetc+".grammar");
    bool hasGrammar = (fiGrammar.size() > 0);

    if (!hasGrammar)
    {
      kDebug() << "No Grammar!  Model recompilation aborting!";
      emit modelCompilationAborted();
      return;
    }
///////
//    QFileInfo fiPrompts(activeDir+"prompts");
//    bool hasPrompts = (fiPrompts.size() > 0);
//    if (!hasPrompts)
//    {
//      switch (baseModelType)
//      {
//        case 1:
//          kDebug() << "No Prompts!  Switching to static model!";
//          baseModelType = 0;
//          break;
//        case 2:                                     //do not bother creating the model without prompts
//          kDebug() << "No Prompts!  Model recompilation aborting!";
//          emit modelCompilationAborted();
//          return;
//      }
//    }

    ModelCompiler::CompilationType compilationType = getCompilationType(baseModelType);


    //build fingerprint and search cache for it
    uint fingerprint = 0;
    QStringList componentsToParse(QStringList() << ".grammar" << ".phone" << ".dic");
    if (baseModelType > 0)
      componentsToParse << "_train.transcription" << "_train.fileids" ;

    fingerprint = getFingerPrint(fetc, componentsToParse, compilationType);

    bool exists;
    QString outPath = cachedModelPath(fingerprint, &exists);

    if (!keepGoing) return;

    if (exists || compiler->startCompilation(compilationType, outPath, baseModelPath, compilerArgs))
    {
      emit modelReady(fingerprint, outPath);
      return;
    } else
      kWarning() << "Model compilation failed for user " << userName;
  } while (tryAgain);
  emit modelCompilationAborted();
}
