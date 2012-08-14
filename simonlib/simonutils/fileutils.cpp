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

#include "fileutils.h"

#include <KTar>
#include <KDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>

bool FileUtils::removeDirRecursive(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDirRecursive(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}

bool FileUtils::pack(const QString &targetArchive, const QHash<QString, QByteArray> &fromMemory,
                     const QHash<QString, QString> &existingFiles)
{
  KTar archive(targetArchive, "application/x-gzip");
  if (!archive.open(QIODevice::WriteOnly)) return false;

  QHash<QString, QByteArray>::const_iterator data = fromMemory.begin(), end = fromMemory.end();

  while(data != end)
  {
    archive.writeFile(data.key(), "nobody", "nobody", data.value().constData(), data.value().length());
    data++;
  }

  QHash<QString, QString>::const_iterator edata = existingFiles.begin(), bend = existingFiles.end();
  while(edata != bend)
  {
    //if(!
       archive.addLocalFile(edata.key(), edata.value());// && edata.key().left(1)!="." && edata.key().left(2)!=".."
                                                                                                            //)
      //return false;
    edata++;
  }

  return archive.close();
}

bool FileUtils::unpack(const QString &archive, const QString &targetDir, const QStringList &files)
{
  kDebug() << "Archive: " << archive << "Target dir: " << targetDir;

  if (!QFile::exists(archive)) return false;

  KTar tar(archive, "application/x-gzip");
  if (!tar.open(QIODevice::ReadOnly)) return false;

  const KArchiveDirectory *d = tar.directory();
  if (!d) return false;

  const QStringList &iFiles = files.isEmpty()?d->entries():files;

  foreach (const QString& file, iFiles)
  {
    const KArchiveFile *entry = dynamic_cast<const KArchiveFile*>(d->entry(file));
    if (!entry) return false;

    QFile f(targetDir+file);
    if (!f.open(QIODevice::WriteOnly)) return false;
    f.write(entry->data());
    f.close();
  }
  return true;
}

bool FileUtils::unpackAll(const QString &archive, const QString &targetDir)
{
  return unpack(archive, targetDir, QStringList());
}

