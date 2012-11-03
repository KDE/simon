/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SSC_USERININSTITUTION_H_745494ACED494E8B9CE7267E37117ED2
#define SIMON_SSC_USERININSTITUTION_H_745494ACED494E8B9CE7267E37117ED2

#include <QString>
#include <QMetaType>
#include "sscobject.h"
#include "sscobjects_export.h"

class SSCOBJECTS_EXPORT UserInInstitution : public SSCObject
{

  private:
    qint32 m_userId;
    qint32 m_institutionId;
    QString m_referenceId;

  public:
    UserInInstitution(qint32 userId, qint32 institutionId, const QString& referenceId);
    UserInInstitution() {}

    void deserialize(QByteArray data);
    QByteArray serialize();

    qint32 userId() { return m_userId; }
    qint32 institutionId() { return m_institutionId; }
    QString referenceId() { return m_referenceId; }

    void setUserId(qint32 userId) { m_userId = userId; }

    ~UserInInstitution() {}
};

Q_DECLARE_METATYPE(UserInInstitution*);
#endif
