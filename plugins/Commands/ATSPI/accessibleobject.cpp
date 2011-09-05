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
#include "atspiaction.h"
#include <atspi-constants.h>

#include <QDBusVariant>
#include <QDBusArgument>
#include <QStringList>
#include <KDebug>

AccessibleObject::AccessibleObject ( QDBusConnection &conn, const QString &service, const QString &path, AccessibleObject *parent)
    : QObject(parent), m_parent ( parent ), m_conn ( conn ), m_service ( service ), m_path ( path ), m_actions(0)
{
  fetchName();
  fetchRole();
  fetchState();
  fetchIndexInParent();
  fetchChildCount();
  
  //setting up monitoring
  if (!parent) {
    m_conn.connect(m_service, "", "org.a11y.atspi.Event.Object", "StateChanged", this, 
                              SLOT(slotStateChanged(QString, int, int, QDBusVariant, QSpiObjectReference)));
    m_conn.connect(m_service, "", "org.a11y.atspi.Event.Object", "ChildrenChanged", this, 
                              SLOT(slotChildrenChanged(QString, int, int, QDBusVariant, QSpiObjectReference)));
    m_conn.connect(m_service, "", "org.a11y.atspi.Event.Object", "PropertyChanged", this, 
                              SLOT(slotPropertyChange(QString, int, int, QDBusVariant, QSpiObjectReference)));
  }
  
  kDebug() << "Created new accessible object: " << m_name << m_path;
}

AccessibleObject::~AccessibleObject()
{
  if (m_actions) qDeleteAll(*m_actions);
  delete m_actions;
  
  qDeleteAll(m_children);
}

// Monitoring functions
//////////////////////////////

AccessibleObject* AccessibleObject::findChild(const QString& path)
{
  if (m_path == path)
    return this;
  
  for (int i=0; i < childCount(); i++) {
    AccessibleObject *c = getChild(i)->findChild(path);
    if (c) return c;
  }
  
  return 0;
}

AccessibleObject* AccessibleObject::findOrCreateChild(const QString& pathOfChild)
{
  AccessibleObject *changedObject = findChild(pathOfChild);
  if (!changedObject) {
    //find parent object
    QString parentPath = pathOfChild;
    do
    {
      kDebug() << "Fetching parent of " << parentPath;
      QDBusMessage parentReply = m_conn.call (QDBusMessage::createMethodCall(
                              m_service, parentPath, "org.a11y.atspi.Accessible", "GetParent"));
      if (parentReply.arguments().isEmpty() || 
        (!parentReply.arguments().first().isValid() || parentReply.arguments().first().isNull())) {
        return 0;
      }
      
      QSpiObjectReference parentRef;
      QDBusArgument parentArg(parentReply.arguments().first().value<QDBusArgument>());
      
      parentArg >> parentRef;
      
      parentPath = parentRef.path.path();
      
      if (parentPath == QSPI_OBJECT_PATH_ACCESSIBLE_NULL)
        return 0;
      
      changedObject = findChild(parentPath);
    }
    while (!changedObject);
    
    changedObject->resetChildren();
    changedObject = findChild(pathOfChild);
  }
  return changedObject;
}

void AccessibleObject::slotStateChanged(const QString& change, int arg1, int arg2, QDBusVariant data, QSpiObjectReference reference)
{
  Q_UNUSED(data);
  Q_UNUSED(reference);
  kDebug() << "State changed of " << message().path() << ": " << change << arg1 << arg2;
  AccessibleObject *o = findOrCreateChild(message().path());
  if (o) {
    o->fetchState();
    emit changed();
  }
}

void AccessibleObject::slotChildrenChanged(const QString &change, int arg1, int arg2, QDBusVariant data, QSpiObjectReference reference)
{
  Q_UNUSED(reference);
  QDBusArgument dataArg(data.variant().value<QDBusArgument>());
  QSpiObjectReference ref;
  dataArg >> ref;

  kDebug() << "Children changed of " << ref.path.path() << ": " << change << arg1 << arg2;
  AccessibleObject *o = findOrCreateChild(ref.path.path());
  if (o) {
    if ((o == this) && (change == "remove")) {
      emit serviceRemoved(this);
    } else {
      o->getParent()->resetChildren(); //because o != this there will be a parent
      emit changed();
    }
  } else {
    //can't do anything else but re-fresh everything at this point...
    resetChildren();
    emit changed();
  }
}

void AccessibleObject::slotPropertyChange(const QString& change, int arg1, int arg2, QDBusVariant data, QSpiObjectReference reference)
{
  Q_UNUSED(data);
  Q_UNUSED(reference);
  if (change != "accessible-name")
    return; //everything is irrelevant for us
  
  kDebug() << "Property changed of " << message().path() << ": " << change << arg1 << arg2;
  AccessibleObject *o = findOrCreateChild(message().path());
  if (o) {
    o->fetchName();
    emit changed();
  }
}

// Fetching functions
//////////////////////////////

void AccessibleObject::fetchChildCount()
{
  m_childCount = getProperty ( m_service, m_path, "org.a11y.atspi.Accessible", "ChildCount" ).toInt();
}

void AccessibleObject::fetchIndexInParent()
{
  QDBusMessage indexInParentReply = m_conn.call (QDBusMessage::createMethodCall(
                           m_service, m_path, "org.a11y.atspi.Accessible", "GetIndexInParent"));
  m_indexInParent = (indexInParentReply.arguments().isEmpty()) ? -1 : indexInParentReply.arguments().at ( 0 ).toInt();
}

void AccessibleObject::fetchName()
{
  m_name = getProperty ( m_service, m_path, "org.a11y.atspi.Accessible", "Name" ).toString();
  
  m_name.remove(QRegExp("<[^>]*>")); //strip html tags
  m_name.remove('\n'); //remove linebreaks
  
  //TODO: NUMBERS!
  m_name.remove(QRegExp("[^a-z A-Z]")); //strip everything except characters and spaces
  m_name.replace(QRegExp("  +"), " "); //eliminate double spacing
  m_name = m_name.trimmed();
}

void AccessibleObject::fetchRole()
{
  QDBusMessage roleReply = m_conn.call ( QDBusMessage::createMethodCall (
                           m_service, m_path, "org.a11y.atspi.Accessible", "GetRole" ) );
  if (roleReply.arguments().isEmpty())
    m_role = -1;
  else
    m_role = roleReply.arguments().at ( 0 ).toInt();
}

void AccessibleObject::fetchState()
{
  QDBusMessage message = QDBusMessage::createMethodCall (
                           m_service, m_path, "org.a11y.atspi.Accessible", "GetState" );

  QDBusMessage reply = m_conn.call ( message );
  if (reply.arguments().isEmpty()) {
    m_state = 0;
    return;
  }
  
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
  
  m_state = state.at(1);
  m_state = m_state << 32;
  m_state += state.at(0);
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

// Getter functions
//////////////////////////////

QString AccessibleObject::name() const
{
  return m_name;
}

QString AccessibleObject::service() const
{
  return m_service;
}

int AccessibleObject::indexInParent() const
{
 return m_indexInParent;
}

int AccessibleObject::childCount() const
{
  return m_childCount;
}

int AccessibleObject::role() const
{
  return m_role;
}

bool AccessibleObject::isShown() const
{
  return   (!m_name.isEmpty() && 
            (m_state & (quint64(1) << ATSPI_STATE_VISIBLE)) &&
            (m_state & (quint64(1) << ATSPI_STATE_SHOWING)));
}

bool AccessibleObject::isSelectable() const
{
  return   (m_state & (quint64(1) << ATSPI_STATE_SELECTABLE));
}

AccessibleObject *AccessibleObject::getParent() const
{
  return m_parent;
}

QString AccessibleObject::path() const
{
  return m_path;
}

// Getter functions (lazy)
//////////////////////////////

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

  m_children[index] = new AccessibleObject ( m_conn, service, path, const_cast<AccessibleObject*> ( this ) );

  return m_children.at ( index );
}


// Helper functions
//////////////////////////////

QString AccessibleObject::roleName() const
{
  QDBusMessage message = QDBusMessage::createMethodCall (
                           m_service, m_path, "org.a11y.atspi.Accessible", "GetRoleName" );

  QDBusMessage reply = m_conn.call ( message );
  return (reply.arguments().isEmpty()) ? QString() : reply.arguments().at ( 0 ).toString();
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
  if (reply.arguments().isEmpty()) return QVariant();
  
  QDBusVariant v = reply.arguments().at ( 0 ).value<QDBusVariant>();
  return v.variant();
}

void AccessibleObject::resetChildren()
{
  //invalidate children cache to be re-build on next access
  QHash<QString, AccessibleObject*> oldChildren;
  foreach (AccessibleObject *o, m_children)
    if (o)
      oldChildren.insert(o->path(), o);
  m_children.clear();
  fetchChildCount();
  
  for (int i=0; i < m_childCount; i++) {
    AccessibleObject *newObject = getChild(i);
    QString childPath = newObject->path();
    //replace newly created child with old one
    if (oldChildren.contains(childPath)) {
      delete m_children.takeAt(i);
      m_children.insert(i, oldChildren.take(childPath));
    }
  }
  
  qDeleteAll(oldChildren.values());
}

// Logic functions
//////////////////////////////

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

      if (!reply.arguments().isEmpty() && reply.arguments().at ( 0 ).toBool())
        return true;
    }
  }
  
  for (int i=0; i < childCount(); i++)
    if (getChild(i)->trigger(name_)) return true;
  
  return false;
}

