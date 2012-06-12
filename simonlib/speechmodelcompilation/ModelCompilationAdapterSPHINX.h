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

class MODELCOMPILATIONMANAGEMENT_EXPORT ModelCompilationAdapterSPHINX : public ModelCompilationAdapter
{
protected:
    QString m_workingDir;
    QString m_modelName;

    bool storeModel(AdaptionType adaptionType,
                    const QString& promptsPathIn, const QString &workingDirPath, const QString &mName,
                    QSharedPointer<Vocabulary> vocabulary, QSharedPointer<Grammar> grammar);

    bool storeDictionary(const QString &dictionaryPathOut, QStringList &trainedVocabulary,
                         QStringList &definedVocabulary, QSharedPointer<Vocabulary> vocabulary);

public:
    ModelCompilationAdapterSPHINX(const QString& userName, QObject *parent=0);

    QString workingDir() { return m_workingDir; }
    QString modelName() { return m_modelName; }

    bool startAdaption(AdaptionType adaptionType, const QStringList &scenarioPathsIn,
                       const QString &promptsIn, const QHash<QString, QString> &args);
};

#endif // MODELCOMPILATIONADAPTERSPHINX_H
