/*
 *   Copyright (C) 2010 Grasch Peter <grasch@simon-listens.org>
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

#ifndef SIMON_AVATAR_H_4B4956DCAE204C49977297D20CB81F09
#define SIMON_AVATAR_H_4B4956DCAE204C49977297D20CB81F09

#include "simondialogengine_export.h"
#include <QImage>
#include <QString>
#include <QDomElement>

class QDomDocument;

class SIMONDIALOGENGINE_EXPORT Avatar
{
private:
  static const char* formatString;
  
  int m_id;
  QString m_name;
  QImage m_image;
  Avatar() {};

public:
    QDomElement serialize(QDomDocument* doc);
     bool deSerialize(const QDomElement& );
    Avatar(int id, const QString& name, const QImage& image);
    
    static Avatar* createInstance(const QDomElement& elem);
    
    int id() { return m_id; }
    QString name() { return m_name; }
    QImage image() { return m_image; }
    void setName(const QString& name);
    void setImage(const QImage& image);
    
};

#endif // SIMON_AVATAR_H_4B4956DCAE204C49977297D20CB81F09
