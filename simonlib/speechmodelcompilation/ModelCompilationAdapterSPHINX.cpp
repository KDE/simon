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


#include "ModelCompilationAdapterSPHINX.h"
#include<KLocalizedString>

ModelCompilationAdapterSPHINX::ModelCompilationAdapterSPHINX(const QString &userName, QObject *parent)
                                                            : ModelCompilationAdapter(userName, parent)
{
}

bool ModelCompilationAdapterSPHINX::startAdaption(AdaptionType adaptionType, const QStringList& scenarioPathsIn,
                                               const QString& promptsIn, const QHash<QString, QString>& args)
{
    abort();

    m_adaptionType = adaptionType;
    m_scenarioPathsIn = scenarioPathsIn;
    m_promptsPathIn = promptsIn;

    m_etcPathOut = args.value("etc");
    m_audioPathOut = args.value("audio");

    if (args.value("stripContext") == "true")
    {
        //remove context additions for prompts file
        if(!removeContextAdditions())
            return false;
    }

    keepGoing=true;

//    emit  status(i18n("Adapting model..."), 0, 100);
//    if (!adaptModel(m_adaptionType, m_scenarioPathsIn, m_promptsPathIn, m_lexiconPathOut,
//                    m_grammarPathOut, m_simpleVocabPathOut, m_promptsPathOut))
//    {
//        return false;
//    }
    emit  status(i18n("Model adaption complete"), 100, 100);
    emit adaptionComplete();

    return true;
}

