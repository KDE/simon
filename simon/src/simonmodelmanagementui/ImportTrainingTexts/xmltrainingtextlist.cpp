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

#include "xmltrainingtextlist.h"
#include <simonlogging/logger.h>
#include <KLocalizedString>
#include <QDomElement>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param path
 * Path to the textlist - default 0
 */
XMLTrainingTextList::XMLTrainingTextList(QString path) : XMLDomReader(path)
{
}


/**
 * \brief Loads the document
 * Calls the XMLDomReader::load with the given path
 * \author Peter Grasch
 * @param path
 * Path to the list - default 0
 */
bool XMLTrainingTextList::load(QString path)
{
  if (path.isEmpty()) path = this->path;
  Logger::log(i18nc("%1 is path", "Loading list of training texts from %1", path));
  if (!XMLDomReader::load(path)) return false;

  trainingtexts.clear();

  QDomElement root = doc->documentElement();

  QDomElement text = root.firstChildElement();
  while(!text.isNull()) {
    trainingtexts.insert(text.attribute("name"), text.attribute("url"));
    text = text.nextSiblingElement();
  }
  return true;
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLTrainingTextList::~XMLTrainingTextList()
{
}
