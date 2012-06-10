/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#include "modelcompilationadapter.h"

#include <QTimer>
#include <KLocale>
#include <KDebug>
#include <KGlobal>
#include <QString>
#include <QFile>
#include <KStandardDirs>
#include <KAboutData>
#include <KComponentData>

ModelCompilationAdapter::ModelCompilationAdapter(const QString& userName, QObject *parent) : QObject(parent), m_userName(userName)
{
}

void ModelCompilationAdapter::abort()
{
  kDebug() << "Aborting current adaption";
  keepGoing = false;
}

bool ModelCompilationAdapter::removeContextAdditions()
{
    QString realInPrompts = m_promptsPathIn;
    m_promptsPathIn = KStandardDirs::locateLocal("tmp", KGlobal::mainComponent().aboutData()->appName()+'/'+m_userName+"/compile/tmpprompts");
    QFile newPrompts(m_promptsPathIn);
    QFile oldPrompts(realInPrompts);
    if (!newPrompts.open(QIODevice::WriteOnly) || !oldPrompts.open(QIODevice::ReadOnly)) {
        emit error(i18n("Couldn't strip context of prompts"));
        return false;
    }
    while (!oldPrompts.atEnd()) {
        QByteArray line = oldPrompts.readLine();
        int firstIndex = line.indexOf('"');
        line.remove(firstIndex, line.indexOf('"', firstIndex+1) - firstIndex + 1);
        newPrompts.write(line);
    }
    return true;
}
