/*
 *   Copyright (C) 2011 Frederik Gladhorn <gladhorn@kde.org>
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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

#ifndef ACCESSIBLEOBJECT_H
#define ACCESSIBLEOBJECT_H

#include <QString>
#include <QVariant>
#include <QVector>
#include <QDBusMessage>
#include <QDBusConnection>

class ATSPIAction;
class QStringList;

class AccessibleObject
{
public:
  AccessibleObject(const QDBusConnection &conn, const QString &service, const QString &path, AccessibleObject *parent = 0);

  QString name() const;
  int indexInParent() const;
  int childCount() const;
  int role() const;
  bool isShown() const;
  bool isSelectable() const;
  bool hasActions() const;
  QString roleName() const;
  AccessibleObject *getChild(int index) const;
  AccessibleObject *getParent() const;
  QString path() const;
  
  bool trigger(const QString& name) const;
  
  ~AccessibleObject();

private:
  QVariant getProperty(const QString &service, const QString &path, const QString &interface, const QString &name) const;

  QDBusConnection m_conn;
  QString m_service;
  QString m_path;

  AccessibleObject *m_parent;
  mutable QList<AccessibleObject*> m_children;
  mutable QList<ATSPIAction*> *m_actions;
  
  qint64 getState() const;
  void fetchActions() const;
  
  QList<ATSPIAction*> actions() const;
};

#endif // ACCESSIBLEOBJECT_H
