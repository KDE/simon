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

#include "importgrammar.h"
#include <QFile>
#include <KI18n/klocalizedstring.h>
#include <KArchive/KFilterDev>

#include <QTextCodec>
#include <QDebug>
#include <kencodingdetector.h>
#include <simonscenarios/speechmodel.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/scenariomanager.h>
#include <QMimeDatabase>
#include <KEncodingProber>

ImportGrammar::ImportGrammar(QObject* parent): QThread(parent),
includeUnknown(false)
{
}


void ImportGrammar::run()
{
  emit allProgress(0, files.count());
  QStringList sentences;
  for (int i=0; i < files.count(); i++) {         //for every file
    QStringList newSentences=importFile(files[i]);//import the file
    for (int j=0; j<newSentences.count(); j++) {  //remove the doubles
      if (!sentences.contains(newSentences[j]))
        sentences << newSentences[j];             //add them to the list
    }
    emit allProgress(i+1, files.count());
    QFile::remove(files[i]);
  }
  ScenarioManager::getInstance()->getCurrentScenario()->addStructures(sentences);
  emit grammarCreated();
}


QStringList ImportGrammar::readFile(QString path)
{
  emit status(i18n("Opening File..."));
  QStringList structures;

  QMimeDatabase db;
  QIODevice *file = KFilterDev::deviceForFile(path, db.mimeTypeForFile(path, QMimeDatabase::MatchContent).name());
  if ((!file) || (!file->open(QIODevice::ReadOnly)))
    return structures;

  QTextCodec *codec;
  if (encoding == i18nc("Selection box for the encoding: Determine the encoding automatically", "Automatic")) {
    //read first 5000 bytes and run encoding detection
    //seek back to the beginning and parse file using the guessed encoding
    QByteArray preview = file->peek(5000);
    KEncodingProber prober;
    #ifdef Q_OS_WIN32
    prober.setProberType(KEncodingProber::WesternEuropean);
    #else
    prober.setProberType(KEncodingProber::Unicode);
    #endif
    prober.feed(preview);
    QString out = QTextCodec::codecForName(prober.encoding())->toUnicode(preview);
    codec = QTextCodec::codecForName(prober.encoding());
  } else
  codec = QTextCodec::codecForName(encoding.toAscii());

  emit status(i18n("Reading File..."));

  //matches for example the following:
  // this is a test.
  // this is a test...
  // this is a test!
  // this is a test?
  // this is a test!?
  // this is a test?!...!
  // this is a test - or is it? (is recognised as two separate sentences: this is a test; or is it)
  // he said: Test
  QRegExp sentenceStoppers = QRegExp("((\\.|,|\\?|\\!|:)(\\.|\\?|\\!)*| )-*( |$|\\n|\\r\\n)");

  QString currentSentence;
  while (!file->atEnd()) {
    QStringList realSentences;

    QString sentence;                             //=leftOvers;
    // 		leftOvers="";

    //		while (!file->atEnd() && (!sentence.contains(sentenceStoppers)))
    sentence = codec->toUnicode(file->readLine(4000).trimmed())+'\n';
    qDebug() << sentence;

    QStringList sentences = sentence.split(sentenceStoppers, QString::SkipEmptyParts);
    qDebug() << sentences;
    for (int i=0; i < sentences.count();i++) {
      currentSentence = sentences[i].trimmed();
      if (currentSentence.trimmed().isEmpty()) continue;

      realSentences << currentSentence;
    }

    //if (!sentences.at(sentences.count()-1).contains(sentenceStoppers))
    //	leftOvers = sentences.at(sentences.count()-1);
    //else realSentences << leftOvers;

    structures << realSentences;
  }
  file->close();
  delete file;
  return structures;
}


QStringList ImportGrammar::importFile(QString path)
{
  emit fileProgress(0, 0);                        //waiting

  QStringList out;

  QStringList structures = readFile(path);

  emit status(i18n("Processing..."));
  emit fileProgress(0, structures.count());

  QList<Word*> lookupResult;

  QString currentSentence;
  int progress=0;
  int max=structures.count();
  while (!structures.isEmpty()) {
    currentSentence = structures.takeAt(0);
    currentSentence.remove(',');                  //cleaning up
    currentSentence.remove('.');
    currentSentence.remove('!');
    currentSentence.remove('?');
    currentSentence.remove('(');
    currentSentence.remove(')');
    currentSentence.remove(':');
    currentSentence.remove('-');
    currentSentence.remove('"');
    QStringList words = currentSentence.split(' ',QString::SkipEmptyParts);

    QString category;
    bool everyWordSure=true;
    for (int j=0; (j < words.count()) && everyWordSure; j++) {
      //first: quick lookup
      lookupResult = ScenarioManager::getInstance()->findWords(words[j],
        SpeechModel::ScenarioVocabulary, Vocabulary::ExactMatch);
      qDebug() << "Looking up " << words[j] << " found " << lookupResult.count() << " results in the active dictionary";

      QStringList wordCategories=categories(lookupResult);
      if (wordCategories.count()==0) {
        //don't delete the contents of the list
        lookupResult = ScenarioManager::getInstance()->findWords(words[j],
          SpeechModel::ShadowVocabulary, Vocabulary::ExactMatch);

        qDebug() << "Looking up " << words[j] << " found " << lookupResult.count() << " results in the active dictionary";

        wordCategories = categories(lookupResult);
      }
      qDebug() << wordCategories;

      if (wordCategories.count() != 1 /*change this to include ambigous categories */) {
        if (includeUnknown)
          words.replace(j, i18nc("Category name for words that are imported from a dictionary "
				  "which does not provide category information", "Unknown"));
        else
          everyWordSure = false;
      } else
      words.replace(j, wordCategories[0]);
    }
    if (everyWordSure) {
      //add to output
      qDebug() << "Found sentence: " << words.join(" ");
      out << words.join(" ");
    }
    emit fileProgress(++progress, max);
  }

  emit status(i18nc("The import has finished", "Finished"));
  return out;
}


QStringList ImportGrammar::categories(QList<Word*> in)
{
  QStringList categories;
  QString category;
  foreach (Word* w, in) {
    category = w->getCategory();
    if (!categories.contains(category)) categories << category;
  }
  if (!includeUnknown) categories.removeAll(i18nc("Category name for words that are imported from a dictionary "
				  "which does not provide category information", "Unknown"));
  return categories;
}


ImportGrammar::~ImportGrammar()
{
}
