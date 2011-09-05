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


#include "qt-atspi.h"

#include <QString>
#include <QVariant>
#include <QVariantList>
#include <QVector>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusArgument>
#include <QDBusContext>

class ATSPIAction;
class QStringList;

class AccessibleObject : public QObject, protected QDBusContext
{
Q_OBJECT
signals:
  void changed();
  void serviceRemoved(AccessibleObject*);
  
public:
  AccessibleObject(QDBusConnection &conn, const QString &service, const QString &path, AccessibleObject *parent);
  ~AccessibleObject();

  //getter functions
  QString name() const;
  QString service() const;
  AccessibleObject *getChild(int index) const;
  AccessibleObject *getParent() const;
  QString path() const;
  int indexInParent() const;
  int childCount() const;
  int role() const;
  bool isShown() const;
  bool isSelectable() const;
  bool hasActions() const;
  QList<ATSPIAction*> actions() const;
  
  //info functions (expensive)
  QString roleName() const;
  
  //actions
  bool trigger(const QString& name) const;
  
  //monitoring
  void resetChildren();

private:
  //set in constructor
  AccessibleObject *m_parent;
  QDBusConnection& m_conn;
  QString m_service;
  QString m_path;
  
  //fetched in constructor
  QString m_name;
  int m_role;
  qint64 m_state;
  int m_indexInParent;
  int m_childCount;

  //fetch on demand
  mutable QList<AccessibleObject*> m_children;
  mutable QList<ATSPIAction*> *m_actions;
  
  //helper functions
  QVariant getProperty(const QString &service, const QString &path, const QString &interface, const QString &name) const;
  
  //fetching data
  void fetchActions() const;
  
  void fetchState();
  void fetchName();
  void fetchRole();
  void fetchIndexInParent();
  void fetchChildCount();
  
  AccessibleObject* findChild(const QString& path);
  AccessibleObject* findOrCreateChild(const QString& path);
  
private slots:
  void slotPropertyChange(const QString& change, int, int, QDBusVariant, QSpiObjectReference);
  void slotStateChanged(const QString& change, int, int, QDBusVariant, QSpiObjectReference);
  void slotChildrenChanged(const QString& change, int, int, QDBusVariant, QSpiObjectReference);
};

#endif // ACCESSIBLEOBJECT_H
