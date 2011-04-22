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

#ifndef SIMON_AVATARMODEL_H_4B4956DCAE204C49977297D20CB81F09
#define SIMON_AVATARMODEL_H_4B4956DCAE204C49977297D20CB81F09

#include <QList>
#include <QModelIndex>
#include <QAbstractListModel>
#include "avatar.h"

class AvatarModel : public QAbstractListModel
{
private:
    QList<Avatar*> m_avatars;
    AvatarModel() {}

public:
    AvatarModel(const QList<Avatar*> avatars);
    static AvatarModel* createInstance(const QDomElement& elem);
    
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    
    QDomElement serialize(QDomDocument* doc);
    bool deSerialize(const QDomElement& );
    
    bool addAvatar(Avatar* a);
    bool removeAvatar(Avatar *a);
    Avatar* getAvatar(int id);
    int getNextId(const QString& name);
    
    QModelIndex getAvatarIndex(int id);
};

#endif // AVATARMODEL_H
