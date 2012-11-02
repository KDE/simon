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

#include "plsdict.h"

#include <simonxml/xmlsaxreader.h>
#include <QStringList>
#include <QString>
#include <QFile>
#include <QRegExp>
#include <KDebug>
#include <QTextCodec>
#include <KLocalizedString>
#include <KMimeType>

/**
 * \brief Constructor
 * \author Peter Grasch
 * \param QString path
 * The path to the dict
 */
PLSDict::PLSDict(QObject* parent) : QXmlDefaultHandler(),
Dict(parent),
reader(0),
pos(0)
{
  buildTranslationTables();
}


/**
 * \brief Overwritten method of the QXmlDefaultHandler - used to react on the start of a tag
 * \author Peter Grasch
 *
 * This function is called internally every time the parser finds the start of a new tag
 * We then decide if the tag is relevant for us (the name for instance can be determined by looking
 * at the qName attribute, and react on it
 *
 * \param QString namespaceURI
 *  namespaceURI is the namespace URI, or an empty string if the element has no namespace URI or if no namespace processing is done
 * \param QString &localName
 * localName is the local name (without prefix), or an empty string if no namespace processing is done
 * \param QString &qName
 * qName is the qualified name (with prefix)
 * \param QXmlAttributes &attributes
 * atts are the attributes attached to the element. If there are no attributes, atts is an empty attributes object.
 * \return bool
 * Success. (in the current implementation this returns always true)
 */
bool PLSDict::startElement(const QString&,
const QString&,
const QString &qName,
const QXmlAttributes &attributes)
{
  if (qName == "lexicon")
    currentTagType = PLSDict::Lexicon;
  else
  if (qName == "meta")
    currentTagType = PLSDict::Meta;
  else
  if (qName == "metadata")
    currentTagType = PLSDict::Metadata;
  else
  if (qName == "lexeme") {
    currentTagType = PLSDict::Lexeme;
    currentWord.clear();
    currentWords.clear();
    phonemeDefinitions.clear();
    currentPhonemeDefinition.clear();

    int typeIndex = attributes.index("role");
    if (typeIndex != -1) {
      currentTerminal = attributes.value(typeIndex);
      if (currentTerminal.isEmpty())
        typeIndex = -1;
    }
    
    if (typeIndex == -1)
      currentTerminal = i18nc("Terminal name for words that are imported from a dictionary "
				  "which does not provide terminal information", "Unknown");
  }
  else
  if (qName == "grapheme")
    currentTagType = PLSDict::Grapheme;
  else
  if (qName == "phoneme")
    currentTagType = PLSDict::Phoneme;
  else
  if (qName == "alias")
    currentTagType = PLSDict::Alias;
  else
  if (qName == "example")
    currentTagType = PLSDict::Example;

  //adding the length of the xml data to the position
  //this is actually quite useless and slows down the process
  //but it is still there to get a better estimation of the current progress
  //even with this workaround we still loose a bit of accuracy (which is why
  //the progressbar is quite "jumpy" at the end
  pos += qName.count()+4;
  for(int i=0; i<attributes.count(); i++)
    pos += attributes.qName(i).count() + attributes.value(i).count()+4;

  return true;
}


/**
 * \brief React on the closing of a tag
 * \author Peter Grasch
 *
 * This function is called internally every time the parser sees the closing of a tag.
 * We then react on that by parsing the data we gathered while "in" the tag.
 * We extract the terminal, the pronunciation(s) and insert this data into the 3 stringlists
 * (words, terminals, pronunciations)
 *
 * \param QString namespaceURI
 *  namespaceURI is the namespace URI, or an empty string if the element has no namespace URI or if no namespace processing is done
 * \param QString &localName
 * localName is the local name (without prefix), or an empty string if no namespace processing is done
 * \param QString &qName
 * qName is the qualified name (with prefix)
 * \return bool
 * Success. (in the current implementation this returns always true)
 */
bool PLSDict::endElement(const QString&, const QString&,
const QString &qName)
{
  if (qName == "phoneme") {
    phonemeDefinitions << adaptToSimonPhonemeSet(ipaToXSampa(currentPhonemeDefinition.trimmed()));
    currentPhonemeDefinition.clear();
  } else
  if (qName == "lexeme") {
    foreach (const QString& w, currentWords) {
      // add the found words to the word
      foreach (const QString& phonemeDefinition, phonemeDefinitions) {
        words << w.trimmed();
        pronunciations << phonemeDefinition;
        terminals << currentTerminal;
      }
    }
    //cleanup
    //currentWord.clear();
    //currentWords.clear();
    //phonemeDefinitions.clear();
  } else
  if (qName == "grapheme") {
    currentWords.append(currentWord);
    currentWord.clear();
  }

  if (maxpos == 0)
    emit progress(-1);
  else
    emit progress(qRound(((double)pos/(double)maxpos)*1000));
  return true;
}


/**
 * \brief This function gatheres the "value" of the xml tags
 * \author Peter Grasch
 *
 * This is an overloaded function of the defaulthandler;
 * We use it to gather the data between the starting and the closing of a tag;
 * First we determine if we are in a WORD, TEXT, or none of both tags.
 * We then add the data to the word (member), the text (member) or nowhere (resp.)
 * We also increase the current position (needed for the progress)
 * \param QString str
 * The gathered data
 * \return bool
 * Success (in the current implementation this always returns true)
 */
bool PLSDict::characters(const QString &str)
{
  switch (currentTagType) {
    case PLSDict::Grapheme:
      currentWord += str;
      break;

    case PLSDict::Phoneme:
      currentPhonemeDefinition += str;
      break;
    default:
      //yeah there is nothing to see here, mr. compiler
      break;
  }

  pos += str.count();
  return true;
}


/**
 * \brief Loads the file
 * \author Peter Grasch
 * \param QString path
 * The path to the dict
 */
void PLSDict::load(QString path, QString encoding)
{
  Q_UNUSED(encoding);

  if (reader) reader->deleteLater();

  reader = new XMLSAXReader(path);
  connect(reader, SIGNAL(loaded()), this, SIGNAL(loaded()));

  if (KMimeType::findByFileContent(path)->name() == "text/xml")
    this->maxpos = QFile(path).size();
  else
    //compressed
    this->maxpos = 0;

  reader->load(this, path);
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
PLSDict::~PLSDict()
{
  if (reader) reader->deleteLater();
}
