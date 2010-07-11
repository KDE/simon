/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_XMLTRAININGTEXTLIST_H_017BA847C4564227879DCB0DF884BA64
#define SIMON_XMLTRAININGTEXTLIST_H_017BA847C4564227879DCB0DF884BA64

#include <QString>
#include <QHash>
#include <simonxml/xmldomreader.h>

/**
 * \class XMLTrainingTextList
 * \brief This class handles the list of avalible Traininngtexts when importing from the net
 * \author Peter Grasch
 * \version 0.1
 * \date 3.06.07
 */
class XMLTrainingTextList : public XMLDomReader
{
  Q_OBJECT

    private:
    QHash<QString,QString> trainingtexts;
  public:
    XMLTrainingTextList(QString path);
    bool load(QString path="");

    QHash<QString,QString> getTrainingTextList() { return trainingtexts; }

    ~XMLTrainingTextList();

};
#endif
