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


#ifndef MODELCOMPILATIONADAPTERSPHINX_H
#define MODELCOMPILATIONADAPTERSPHINX_H

#include "modelcompilationadapter.h"
#include "simonmodelcompilationmanagement_export.h"

#define GRAMMAR_EXT ".jsgf"
#define PHONE_EXT ".phone"
#define DICT_EXT ".dic"
#define TRAIN_TRANSCRIPTION "_train.transcription"
#define TRAIN_FIELDS "_train.fileids"
#define TEST_TRANSCRIPTION "_test.transcription"
#define TEST_FIELDS "_test.fileids"
/*
 *This extension just an simply an agreement that must be met in the rest of the code.
 */

class MODELCOMPILATIONMANAGEMENT_EXPORT ModelCompilationAdapterSPHINX : public ModelCompilationAdapter
{
  Q_OBJECT
protected:
  QString m_workingDir;
  QString m_modelName;

  bool storeModel(AdaptionType adaptionType,
                  const QString& promptsPathIn, const QString &workingDirPath, const QString &mName,
                  QSharedPointer<Vocabulary> vocabulary, QSharedPointer<Grammar> grammar);

  bool storeDictionary(AdaptionType adaptionType, const QString &dictionaryPathOut, QStringList &trainedVocabulary,
                       QStringList &definedVocabulary, QSharedPointer<Vocabulary> vocabulary);

  bool storeFiller(AdaptionType adaptionType, const QString &fillerPathOut);

  bool storePhonesList(AdaptionType adaptionType, const QString &phonesListPathOut, QSharedPointer<Vocabulary> vocabulary, const QStringList &trainedVocabulary);

  bool storeTranscriptionAndFields(AdaptionType adaptionType, const QString& promptsPathIn, const QString& transcriptionPathOut, const QString &fieldsPathOut,
                                   QStringList &definedVocabulary, QSharedPointer<Vocabulary> vocabulary);

  bool storeGrammar(AdaptionType adaptionType, const QString &grammarPathOut, QSharedPointer<Vocabulary> vocabulary,
                    QStringList &definedVocabulary, QSharedPointer<Grammar> grammar);


public:
  ModelCompilationAdapterSPHINX(const QString& userName, QObject *parent=0);

  QString workingDir() { return m_workingDir; }
  QString modelName() { return m_modelName; }

  bool startAdaption(AdaptionType adaptionType, const QStringList &scenarioPathsIn,
                     const QString &promptsIn, const QHash<QString, QString> &args);
};

#endif // MODELCOMPILATIONADAPTERSPHINX_H
