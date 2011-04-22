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

#include "avatarmodel.h"
#include <QPixmap>
#include <KDebug>

AvatarModel::AvatarModel(const QList< Avatar* > avatars) : m_avatars(avatars)
{
}

AvatarModel* AvatarModel::createInstance(const QDomElement& elem)
{
  AvatarModel *am = new AvatarModel;
  if (!am->deSerialize(elem))
  {
    delete am;
    am = 0;
  }
  return am;
}

bool AvatarModel::deSerialize(const QDomElement& elem)
{
  //clear
  beginRemoveRows(QModelIndex(), 0, m_avatars.count());
  qDeleteAll(m_avatars);
  m_avatars.clear();
  endRemoveRows();
  
  bool succ = true;
  m_avatars.clear();
  QDomElement avatarElem = elem.firstChildElement("avatar");
  while (!avatarElem.isNull())
  {
    kDebug() << "Deserializing element..." << avatarElem.attribute("id");
    Avatar *a = Avatar::createInstance(avatarElem);
    avatarElem = avatarElem.nextSiblingElement("avatar");
    if (!a) {
      succ = false;
      continue;
    }
    m_avatars << a;
  }
  kDebug() << "Done";
  reset();
  return succ;
}

QDomElement AvatarModel::serialize(QDomDocument* doc)
{
  QDomElement avatarsElem = doc->createElement("avatars");
  foreach (Avatar *a, m_avatars)
    avatarsElem.appendChild(a->serialize(doc));
  return avatarsElem;
}

QVariant AvatarModel::data(const QModelIndex& index, int role) const
{
  Avatar *a = m_avatars[index.row()];
  
  Q_ASSERT(a);
  
  switch (role)
  {
    case Qt::DisplayRole:
      return a->name();
    case Qt::DecorationRole:
      return QPixmap::fromImage(a->image().scaled(64,64,Qt::KeepAspectRatio));
    case Qt::UserRole:
      return a->id();
  }
  
  return QVariant();
}

bool AvatarModel::addAvatar(Avatar* a)
{
  if (getAvatar(a->id()))
    return false; // id already taken
    
  beginInsertRows(QModelIndex(), m_avatars.count(), m_avatars.count()+1);
  m_avatars << a;
  endInsertRows();
  return true;
}

int AvatarModel::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return m_avatars.count();
}

QModelIndex AvatarModel::index(int row, int column, const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  if (row >= m_avatars.count())
    return QModelIndex();
  
  return createIndex(row, column, m_avatars[row]);
}

bool AvatarModel::removeAvatar(Avatar* a)
{
  int i=0;
  bool found = false;
  foreach (Avatar *stored, m_avatars)
  {
    if (stored == a)
    {
      beginRemoveRows(QModelIndex(), i, i+1);
      m_avatars.removeAt(i);
      endRemoveRows();
      found = true;
    }
    i++;
  }
  return found;
}

Avatar* AvatarModel::getAvatar(int id)
{
  foreach (Avatar *a, m_avatars)
    if (a->id() == id)
      return a;
    
  return 0;
}

int AvatarModel::getNextId(const QString& name)
{
  int i = qHash(name);
  bool alreadyTaken;	//simulate linear probing
  do
  {
    alreadyTaken = false;
    foreach (Avatar *a, m_avatars)
      if (a->id() == i)
	alreadyTaken = true;
    if (alreadyTaken)
      i++;
  } while (alreadyTaken);
  return i;
}

QModelIndex AvatarModel::getAvatarIndex ( int id )
{
  return index(m_avatars.indexOf(getAvatar(id)), 0);
}

