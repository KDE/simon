/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SSC_SOUNDCARD_H_1E25B8B7C2B84348A22343E8C9A7D679
#define SIMON_SSC_SOUNDCARD_H_1E25B8B7C2B84348A22343E8C9A7D679

#include <QString>
#include "sscobject.h"
#include "sscobjects_export.h"

class SSCOBJECTS_EXPORT SoundCard : public SSCObject
{

  private:
    qint32 m_id;
    QString m_model;
    QString m_type;

  public:
    SoundCard(qint32 id, const QString& model, const QString& type);

    SoundCard() {}

    void deserialize(QByteArray data);
    QByteArray serialize();

    qint32 id() { return m_id; }
    QString model() { return m_model; }
    QString type() { return m_type; }

    void setId(qint32 id) { m_id = id; }

    ~SoundCard() {}
};
#endif
