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

#include "accessibleobject.h"
#include "qt-atspi.h"
#include "atspiaction.h"
#include <atspi-constants.h>

#include <QDBusVariant>
#include <QDBusArgument>
#include <QStringList>
#include <KDebug>

AccessibleObject::AccessibleObject ( const QDBusConnection &conn, const QString &service, const QString &path, AccessibleObject *parent )
    : m_conn ( conn ), m_service ( service ), m_path ( path ), m_parent ( parent ), m_actions(0)
{
  qRegisterMetaType<QSpiUIntList>();
}

QString AccessibleObject::name() const
{
  QString name = getProperty ( m_service, m_path, "org.a11y.atspi.Accessible", "Name" ).toString();
//    kDebug() << "Get name for: " << m_path << name;
  return name;
}

int AccessibleObject::indexInParent() const
{
  QDBusMessage message = QDBusMessage::createMethodCall (
                           m_service, m_path, "org.a11y.atspi.Accessible", "GetIndexInParent" );

  QDBusMessage reply = m_conn.call ( message );
  return reply.arguments().at ( 0 ).toInt();
}

int AccessibleObject::childCount() const
{
  int children = getProperty ( m_service, m_path, "org.a11y.atspi.Accessible", "ChildCount" ).toInt();
//    kDebug() << "Child count: " << children;
  return children;
}

int AccessibleObject::role() const
{
  QDBusMessage message = QDBusMessage::createMethodCall (
                           m_service, m_path, "org.a11y.atspi.Accessible", "GetRole" );

  QDBusMessage reply = m_conn.call ( message );
  return reply.arguments().at ( 0 ).toInt();
}


QString AccessibleObject::roleName() const
{
  QDBusMessage message = QDBusMessage::createMethodCall (
                           m_service, m_path, "org.a11y.atspi.Accessible", "GetRoleName" );

  QDBusMessage reply = m_conn.call ( message );
  return reply.arguments().at ( 0 ).toString();
}

qint64 AccessibleObject::getState() const
{
  QDBusMessage message = QDBusMessage::createMethodCall (
                           m_service, m_path, "org.a11y.atspi.Accessible", "GetState" );

  QDBusMessage reply = m_conn.call ( message );
  const QDBusArgument returnVal = reply.arguments().at ( 0 ).value<QDBusArgument>();
  
  QSpiUIntList state;
  
  returnVal.beginArray();
  while (!returnVal.atEnd()) {
    int arg;
    returnVal >> arg;
    state << arg;
  }
  returnVal.endArray();
  
  Q_ASSERT(state.count() == 2);
  
  quint64 stateInt = state.at(1);
  stateInt = stateInt << 32;
  stateInt += state.at(0);
  return stateInt;
}


bool AccessibleObject::isShown() const
{
  qint64 stateInt = getState();
  return   ((stateInt & (quint64(1) << ATSPI_STATE_VISIBLE)) &&
            (stateInt & (quint64(1) << ATSPI_STATE_SHOWING)));
}

bool AccessibleObject::isSelectable() const
{
  qint64 stateInt = getState();
  return   (stateInt & (quint64(1) << ATSPI_STATE_SELECTABLE));
}

bool AccessibleObject::hasActions() const
{
  if (!m_actions) 
    fetchActions();
  
  return !m_actions->isEmpty();
}

QList< ATSPIAction* > AccessibleObject::actions() const
{
  if (!m_actions)
    fetchActions();
  
  return *m_actions;
}

void AccessibleObject::fetchActions() const
{
  //fetch actions
  m_actions = new QList<ATSPIAction*>();
  
  QDBusMessage message = QDBusMessage::createMethodCall (
                          m_service, m_path, "org.a11y.atspi.Action", "GetActions" );

  QDBusMessage reply = m_conn.call ( message );
  
  if (reply.arguments().isEmpty())
    return; //no actions
    
  const QDBusArgument returnVal = reply.arguments().at (0).value<QDBusArgument>();
  
  returnVal.beginArray();
  while (!returnVal.atEnd()) {
    returnVal.beginStructure();
    QString name, description, comment;
    returnVal >> name;
    returnVal >> description;
    returnVal >> comment;
    m_actions->append(new ATSPIAction(name, description, comment));
    returnVal.endStructure();
  }
  returnVal.endArray();
}

AccessibleObject *AccessibleObject::getParent() const
{
  return m_parent;
}

AccessibleObject *AccessibleObject::getChild ( int index ) const
{
  while ( m_children.size() < index + 1 ) {
    m_children.append ( 0 );
  }

  if ( m_children.at ( index ) != 0 )
    return m_children.at ( index );

  QVariantList inargs;

  inargs << index;

  QDBusMessage message = QDBusMessage::createMethodCall (
                           m_service, m_path, "org.a11y.atspi.Accessible", "GetChildAtIndex" );

  message.setArguments ( inargs );

  QDBusMessage reply = m_conn.call ( message );

  QString service;

  QString path;

  const QDBusArgument arg = reply.arguments().at ( 0 ).value<QDBusArgument>();

  arg.beginStructure();

  arg >> service;

  arg >> path;

  arg.endStructure();

  kDebug() << "new accessible object " << path;

  m_children[index] = new AccessibleObject ( m_conn, service, path, const_cast<AccessibleObject*> ( this ) );

  return m_children.at ( index );
}

QVariant AccessibleObject::getProperty ( const QString &service, const QString &path, const QString &interface, const QString &name ) const
{
  QVariantList args;
  args.append ( interface );
  args.append ( name );

  QDBusMessage message = QDBusMessage::createMethodCall (
                           service, path, "org.freedesktop.DBus.Properties", "Get" );

  message.setArguments ( args );
  QDBusMessage reply = m_conn.call ( message );
  QDBusVariant v = reply.arguments().at ( 0 ).value<QDBusVariant>();
  return v.variant();
}


QString AccessibleObject::path() const
{
  return m_path;
}

bool AccessibleObject::trigger(const QString& name_) const
{
  if (name() == name_) {
    QList<ATSPIAction*> a = actions();
    if (!a.isEmpty()) { 
      QVariantList inargs;
      inargs << 0; //TODO: display list for ambiguous actions

      QDBusMessage message = QDBusMessage::createMethodCall (
                              m_service, m_path, "org.a11y.atspi.Action", "DoAction" );
      message.setArguments ( inargs );

      QDBusMessage reply = m_conn.call ( message );

      if (reply.arguments().at ( 0 ).toBool())
        return true;
    }
  }
  
  foreach (AccessibleObject *o, m_children)
    if (o->trigger(name_)) return true;
  
  return false;
}

AccessibleObject::~AccessibleObject()
{
  delete m_actions;
}

