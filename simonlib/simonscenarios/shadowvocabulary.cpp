/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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

#include "shadowvocabulary.h"
#include "trainingmanager.h"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <KColorScheme>

#include <KDateTime>
#include <KArchive/KFilterDev>
#include <QMimeDatabase>
#include <QStandardPaths>


/**
 * Empty, private constructor
 */
ShadowVocabulary::ShadowVocabulary(QObject *parent)
  : Vocabulary(parent), loadFailed(false)
{
  QString vocabFilename = QStandardPaths::locate(QStandardPaths::DataLocation, "shadowvocabulary.xml");

  QMimeDatabase db;
  //it's only an error if the vocabulary file exists but can not be read for some reason
  //if it doesn't exist, we'll silently create a new (empty one)
  if (QFile::exists(vocabFilename)) {
    QIODevice *shadowVocabFile = KFilterDev::deviceForFile(vocabFilename,
      db.mimeTypeForFile(vocabFilename, QMimeDatabase::MatchContent).name());

    reset(shadowVocabFile);
    shadowVocabFile->deleteLater();
  }
}


bool ShadowVocabulary::reset(QIODevice* f)
{
  if (!f->open(QIODevice::ReadOnly)) {
    loadFailed = true;
    return false;
  }

  deleteAll();

  f->readLine();                                  //skip document type
  QString root = QString::fromUtf8(f->readLine());//skip root element

  root = root.mid(26, 19);

  lastModifiedDate = QDateTime::fromString(root, Qt::ISODate);

  while (!f->atEnd()) {
    //<word>
    //	<name>
    //		Aal
    //	</name>
    //	<pronunciation>
    //		a: l
    //	</pronunciation>
    //	<category>
    //		NOM
    //	</category>
    //</word>
    f->readLine();                                //skip word
    f->readLine();                                //skip name
    QString name = QString::fromUtf8(f->readLine()).trimmed();
    f->readLine();                                //skip nameend

    f->readLine();                                //skip pronunciation
    QString pronunciation = QString::fromUtf8(f->readLine()).trimmed();
    f->readLine();                                //skip pronunciationend

    f->readLine();                                //skip category
    QString category = QString::fromUtf8(f->readLine()).trimmed();
    f->readLine();                                //skip categoryend
    f->readLine();                                //skip wordend

    if (category.isEmpty()) continue;

    if (!categories.contains(category)) categories << category;

    m_words.append(new Word(name, pronunciation, category));
  }
  QAbstractItemModel::reset();
  loadFailed = false;
  return true;
}


bool ShadowVocabulary::empty()
{
  deleteAll();
  return save();
}


int ShadowVocabulary::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 3;
}


void ShadowVocabulary::touch()
{
  lastModifiedDate = KDateTime::currentUtcDateTime().dateTime();
}


bool ShadowVocabulary::save()
{
  touch();

  QFile f(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "shadowvocabulary.xml");
  QIODevice *shadowVocabFile = KFilterDev::device(&f, "application/x-gzip", false);

  if (!shadowVocabFile->open(QIODevice::WriteOnly))
    return false;

  shadowVocabFile->write("<!DOCTYPE Vocabulary>\n");
  shadowVocabFile->write("<vocabulary lastModified=\""+lastModifiedDate.toString(Qt::ISODate).toUtf8()+"\">\n");

  foreach (Word *w, m_words) {
    //<word>
    //	<name>
    //		Aal
    //	</name>
    //	<pronunciation>
    //		a: l
    //	</pronunciation>
    //	<category>
    //		NOM
    //	</category>
    //</word>
    shadowVocabFile->write("<word>\n");
    shadowVocabFile->write("\t<name>\n");
    shadowVocabFile->write("\t\t"+w->getWord().toUtf8()+'\n');
    shadowVocabFile->write("\t</name>\n");

    shadowVocabFile->write("\t<pronunciation>\n");
    shadowVocabFile->write("\t\t"+w->getPronunciation().toUtf8()+'\n');
    shadowVocabFile->write("\t</pronunciation>\n");

    shadowVocabFile->write("\t<category>\n");
    shadowVocabFile->write("\t\t"+w->getCategory().toUtf8()+'\n');
    shadowVocabFile->write("\t</category>\n");
    shadowVocabFile->write("</word>\n");
  }

  shadowVocabFile->write("</vocabulary>");
  shadowVocabFile->close();
  f.close();

  emit changed();

  return true;

  /*	QDomDocument doc("vocabulary");

    QFile f(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "shadowvocabulary.xml");

    if (!f.open(QIODevice::WriteOnly))
      return false;

    QDomElement elem = serialize(&doc);
    doc.appendChild(elem);

    bool succ = f.write(doc.toString().toUtf8());

  if (succ)
  emit changed();

  return succ;*/
}


bool ShadowVocabulary::addWords(QList<Word*> w)
{
  if (!Vocabulary::addWords(w)) return false;
  return save();
}


bool ShadowVocabulary::addWord(Word *w)
{
  if (!Vocabulary::addWord(w)) return false;
  return save();
}


bool ShadowVocabulary::reOrder(Word* w)
{
  if (!Vocabulary::removeWord(w, false /* do not delete this */))
    return false;
  if (!Vocabulary::addWord(w)) return false;

  return save();
}


bool ShadowVocabulary::removeWord(Word* w, bool deleteWord)
{
  if (Vocabulary::removeWord(w, deleteWord)) {
    return save();
  }
  return false;
}


bool ShadowVocabulary::renameCategory(const QString& from, const QString& to)
{
  if (Vocabulary::renameCategory(from, to))
    return save();
  return false;
}
