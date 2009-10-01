/*
 *   Copyright (C) 2009 Dominik Neumeister <neudob06@edvhtl.at>
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

#ifndef SIMON_KEYBOARDSETXMLREADER_H_84466364391D4314A086F383F2EF6DC9
#define SIMON_KEYBOARDSETXMLREADER_H_84466364391D4314A086F383F2EF6DC9

#include <simonxml/xmldomreader.h>
#include <QList>
#include "keyboardset.h"


class KeyboardsetXMLReader : public XMLDomReader
{
    public:
        KeyboardsetXMLReader(const QString& path="");

        bool save(QList<KeyboardSet *> *list, const QString& path=QString());

        QList<KeyboardSet *> * load(QString path=QString());

        ~KeyboardsetXMLReader();
};

#endif
