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

#include "bompdict.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QFileInfo>
#include <QDebug>

#include <KArchive/KFilterDev>
#include <QTextCodec>
#include <QMimeDatabase>
#include <QMimeType>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param path Sets the path of the dictionary (member)
 * @param parent Parent of the object
 */
BOMPDict::BOMPDict(QObject* parent): Dict(parent)
{
}


/**
 * \brief Loads the file from the given path
 * \author Peter Grasch
 *
 * @fixme This function assumes the system-charset to be ISO-8859-1 and WILL destroy special characters if it isn't
 *
 * @param path If the path is empty the path set by the constructor is used
 */
void BOMPDict::load(QString path, QString encodingName)
{
  emit progress(0);

  QMimeDatabase db;
  QIODevice *dict = KFilterDev::deviceForFile(path,
    db.mimeTypeForFile(path, QMimeDatabase::MatchContent).name());
  if (!dict)
    return;
  if (!dict->open(QIODevice::ReadOnly)) {
    dict->deleteLater();
    return;
  }

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
  dictStream->setCodec(QTextCodec::codecForName(encodingName.toAscii()));
  emit loaded();

  QString line, xsp, category;
  int wordend, termend;
  line = dictStream->readLine(1000);

  QString filteredXsp;
  QString xspFertig;
  QString currentPhoneme;
  QString currentCategory;
  QString currentFinalXsp;
  QString currentWord;
  while (!line.isNull()) {
    wordend = line.indexOf("\t");
    termend = line.indexOf("\t", wordend+1);

    xsp = line.mid(termend).trimmed();

    //		xsp.remove(QRegExp("^'*?*"));
    xsp.remove('\'');
    xsp.remove('|');
    xsp.remove(',');
    currentFinalXsp = segmentSampa(adaptToSimonPhonemeSet(xsp));

    currentWord = line.left(wordend);
    currentCategory = line.mid(wordend,
      termend-wordend).trimmed();

    QStringList currentCategories = currentCategory.split(':', QString::SkipEmptyParts);
    QStringList currentCategoriesUnique;

    if (currentCategories.isEmpty()) {
      line = dictStream->readLine(1000);
      continue;
    }

    QString currentCategoryStr = currentCategories[0];
    currentCategoriesUnique << currentCategoryStr;
    words << currentWord;
    categories << currentCategoryStr;
    pronunciations << currentFinalXsp;

    for (int k=1; k < currentCategories.count(); k++) {
      currentCategoryStr = currentCategories[k];
      if (!currentCategoriesUnique.contains(currentCategoryStr)) {
        currentCategoriesUnique << currentCategoryStr;

        words << currentWord;
        categories << currentCategoryStr;
        pronunciations << currentFinalXsp;
      }
    }

    currentProg += line.length();

    if (maxProg != 0)
      emit progress((int) (((((double)currentProg) /
        ((double)maxProg)))*1000));
    else emit progress(-1);
    line = dictStream->readLine(1000);
  }
  delete dictStream;
  dict->close();
  delete dict;
}


BOMPDict::~BOMPDict()
{
}
