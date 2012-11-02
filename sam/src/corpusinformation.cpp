/*
 *  Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "corpusinformation.h"
#include <QDomElement>
#include "samxmlhelper.h"

CorpusInformation* CorpusInformation::deSerialize(const QDomElement& elem)
{
  if (elem.isNull()) return 0;
  
  QString tag = SamXMLHelper::getText(elem, "tag");
  QString notes = SamXMLHelper::getText(elem, "notes");
  int speakers = SamXMLHelper::getInt(elem, "speakers");
  int samples = SamXMLHelper::getInt(elem, "samples");
  int samplesTotal = SamXMLHelper::getInt(elem, "samplesTotal");
  
  return new CorpusInformation(tag, notes, speakers, samples, samplesTotal);
}

QDomElement CorpusInformation::serialize(QDomDocument *doc)
{
  QDomElement corpusElem = doc->createElement("corpus");
  
  SamXMLHelper::serializeText(doc, corpusElem, tag(), "tag");
  SamXMLHelper::serializeText(doc, corpusElem, notes(), "notes");
  SamXMLHelper::serializeInt(doc, corpusElem, speakers(), "speakers");
  SamXMLHelper::serializeInt(doc, corpusElem, samples(), "samples");
  SamXMLHelper::serializeInt(doc, corpusElem, samplesTotal(), "samplesTotal");
  
  return corpusElem;
}
