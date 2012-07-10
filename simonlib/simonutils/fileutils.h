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

#ifndef FILEUTILS_H_B9CC942F82B347F2B4613266AB81B9D1F
#define FILEUTILS_H_B9CC942F82B347F2B4613266AB81B9D1F

#include <QString>

#include "simonutils_export.h"
#include <QHash>

class SIMONUTILS_EXPORT FileUtils
{
public:
    static bool removeDirRecursive(const QString &dirName);
    static bool pack(const QString &targetArchive,
                     const QHash<QString, QByteArray> &fromMemory /*target fileame, data to store*/,
                     const QHash<QString, QString> &existingFiles /*filename - target filename*/);
    static bool unpack(const QString &archive, const QString& targetDir, const QStringList &files);
    static bool unpackAll(const QString &archive, const QString& targetDir);
};

#endif // FILEUTILS_H
