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

#ifndef SIMON_XMLTRAININGTEXT_H_F18D0131FBE348769C44ECBADBFA789E
#define SIMON_XMLTRAININGTEXT_H_F18D0131FBE348769C44ECBADBFA789E

#include <QStringList>
#include <simonxml/xmldomreader.h>
#include <speechmodelbase/trainingtext.h>

/**
 *  @class XMLTrainingText
 *  @brief To handle with xml-documents
 *
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Christoph Kirschner, Peter Grasch
 */
class XMLTrainingText : virtual public XMLDomReader, virtual public TrainingText
{
  public:
    XMLTrainingText( QString name, QString path, QStringList pages );
    XMLTrainingText( QString path );

    void save(QString path);
    virtual bool save();

    void load(QString path="");
    static XMLTrainingText* createTrainingText(const QString& path);

    void addPages(QStringList pages);

    ~XMLTrainingText();
};
#endif
