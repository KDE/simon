/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_SSC_LANGUAGE_H_1E25B8B7C2B84348A22343E8C9A7D679
#define SIMON_SSC_LANGUAGE_H_1E25B8B7C2B84348A22343E8C9A7D679

#include <QString>
#include "sscobject.h"
#include "sscobjects_export.h"

class SSCOBJECTS_EXPORT Language : public SSCObject
{

  private:
    QString m_id;
    QString m_name;

  public:
    Language(const QString& id, const QString& name);

    Language() {}

    void deserialize(QByteArray data);
    QByteArray serialize();

    QString id() { return m_id; }
    QString name() { return m_name; }

    ~Language() {}
};
#endif
