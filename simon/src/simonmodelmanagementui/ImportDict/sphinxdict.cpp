/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "sphinxdict.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QFileInfo>

#include <KI18n/klocalizedstring.h>
#include <KArchive/KFilterDev>
#include <QDebug>
#include <QMimeDatabase>
#include <QMimeType>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param path Sets the path of the dictionary (member)
 * @param parent Parent of the object
 */
SPHINXDict::SPHINXDict(QObject* parent): Dict(parent)
{
}


/**
 * \brief Loads the file from the given path
 * \author Peter Grasch
 *
 * \todo Document
 * WARNING: This function assumes the dictionary-charset to be ISO-8859-1 and WILL destroy special characters if it isn't
 *
 * @param path If the path is empty the path set by the constructor is used
 */
void SPHINXDict::load(QString path, QString encodingName)
{
  if (path.isEmpty()) path = this->path;
  emit progress(0);

  QMimeDatabase db;
  QIODevice *dict = KFilterDev::deviceForFile(path,
    db.mimeTypeForFile(path, QMimeDatabase::MatchContent).name());
  if ((!dict) || (!dict->open(QIODevice::ReadOnly)))
    return;

  int maxProg=0;

  QMimeType mimeType = db.mimeTypeForFile(path, QMimeDatabase::MatchContent);
  if (mimeType.inherits("text/plain")) {               //not compressed
    QFileInfo info;
    QFile f(path);
    info.setFile(f);
    maxProg = info.size();
  }

  int currentProg = 0;

  QTextStream *dictStream = new QTextStream(dict);
  dictStream->setCodec(encodingName.toAscii());
  emit loaded();

  QString line, word;
  int wordend;
  line = dictStream->readLine(1000);

  QRegExp duplicateIndex("\\(([0-9])*\\)$");
  QRegExp splitter("[ \\t]");
  while (!line.isNull()) {
    wordend = line.indexOf(splitter);
    QString word = line.left(wordend).remove(duplicateIndex);

    if (!word.isEmpty()) {
      words << word;
      categories << i18nc("Category name for words that are imported from a dictionary "
				  "which does not provide category information", "Unknown");
      pronunciations << line.mid(wordend+1).trimmed();
    }

    if (maxProg != 0) {
      currentProg += line.length();
      emit progress((int) (((((double)currentProg) /
        ((double)maxProg)))*1000));
    }

    line = dictStream->readLine(1000);
  }
  delete dictStream;
  dict->close();
  dict->deleteLater();
}


SPHINXDict::~SPHINXDict()
{
}
