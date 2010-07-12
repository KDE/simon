/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "juliusvocabulary.h"
#include <QFile>
#include <QIODevice>
#include <QFileInfo>
#include <QTextStream>
#include <KMimeType>
#include <KFilterDev>
#include <KLocalizedString>
#include <KDebug>

JuliusVocabulary::JuliusVocabulary(QObject* parent): Dict(parent)
{
}


/**
 * \brief Loads the file from the given path
 * \author Peter Grasch
 *
 * @param path If the path is empty the path set by the constructor is used
 * @param encoding The encoding to use for the dictionary
 */
void JuliusVocabulary::load(QString path, QString encoding)
{
  emit progress(0);

  QIODevice *dict = KFilterDev::deviceForFile(path,
    KMimeType::findByFileContent(path)->name());
  if ((!dict) || (!dict->open(QIODevice::ReadOnly)))
    return;

  int maxProg=0;

  KMimeType::Ptr mimeType = KMimeType::findByFileContent(path);
  if (mimeType->is("text/plain")) {               //not compressed
    QFileInfo info;
    QFile f(path);
    info.setFile(f);
    maxProg = info.size();
  }

  int currentProg = 0;

  QTextStream *dictStream = new QTextStream(dict);
  dictStream->setCodec(encoding.toAscii());
  emit loaded();

  QString line, word, terminal, xsp;
  int splitter;
  terminal = i18n("Unknown");
  while (!dictStream->atEnd()) {
    line = dictStream->readLine(1000);
    currentProg += line.length();
    line = line.trimmed();
    if (line.startsWith(QLatin1String("% "))) {
      terminal = line.mid(2).trimmed();
      continue;
    }
    else {
      splitter = line.indexOf("\t");
      word = line.left(splitter).trimmed();
      if (word.isEmpty() || (word == "<s>") || (word == "</s>"))
        continue;
      xsp = line.mid(splitter).trimmed();
      words << word;
      terminals << terminal;
      pronunciations << adaptToSimonPhonemeSet(xsp);
    }

    if (maxProg != 0) {
      emit progress((int) (((((double)currentProg) /
        ((double)maxProg)))*1000));
    }
  }
  dict->close();
  dict->deleteLater();
  delete dictStream;
}


JuliusVocabulary::~JuliusVocabulary()
{
}
