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

  /*!
   *  \brief Delete a directory along with all of its contents.
   *  \author Vladislav Sitalo
   *
   *  \param dirName Path of directory to remove.
   *  \return true on success; false on error.
   */
  static bool removeDirRecursive(const QString &dirName);

  static bool copyDirRecursive(const QString &sourceDirName, const QString &destinationDirName);

  /*!
     * \brief   Pack data from "fromMemory" and files from disk to tar.gzip archive with name targetArchive
     *
     *  \author Vladislav Sitalo
     *  \param targetArchive Archive name
     *  \param fromMemory specifies compliance file name - data to store
     *  \param existingFiles specifies compliance target file name - real file name
     *  \return true on success; false on error.
     */
  static bool pack(const QString &targetArchive,
                   const QHash<QString, QByteArray> &fromMemory /*target filename, data to store*/,
                   const QHash<QString, QString> &existingFiles /*filename - target filename*/);


  /*!
     *  \brief Unpack selected files from gzip archive to target directory. If file list is empty: unpacks all
     *  \author Vladislav Sitalo
     *  \param archive Archive name
     *  \param targetDir Target dir
     *  \param files files to be unpacked
     *  \return true on success; false on error.
     */
  static bool unpack(const QString &archive, const QString& targetDir, const QStringList &files);
  static bool unpackAll(const QString &archive, const QString& targetDir);
};

#endif // FILEUTILS_H
