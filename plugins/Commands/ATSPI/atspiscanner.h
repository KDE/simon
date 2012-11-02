/*
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_ATSPISCANNER_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_ATSPISCANNER_H_7A7B9100FF5245329569C1B540119C37

#include <qaccessibilityclient/accessibleobject.h>
#include <qaccessibilityclient/registry.h>

#include <QList>
#include <QMutex>
#include <QRegExp>
#include <QObject>

class QThread;

class ATSPIScanner : public QObject
{
Q_OBJECT

signals:
  void commandsShown(const QStringList& commands, bool reset);

public:
  ATSPIScanner();
  ~ATSPIScanner();
  
  void clearATModel();
  QVector<QSharedPointer<QAction> > getActions(const QString& triggerName);

private slots:
  void windowActivated(const QAccessibleClient::AccessibleObject& object);
  void nameChanged (const QAccessibleClient::AccessibleObject &object);
  void descriptionChanged (const QAccessibleClient::AccessibleObject &object);
  void stateChanged (const QAccessibleClient::AccessibleObject &object, const QString& state, bool active);

  void initialize();

private:
  bool m_abort;
  QMutex m_modelMutex;
  QThread *m_thread;
  QAccessibleClient::Registry *m_registry;
  QRegExp m_cleanStringRegExp;
  QHash<QString /* name (trigger) */, QAccessibleClient::AccessibleObject /* object id */> m_actions;
  QHash<QAccessibleClient::AccessibleObject /* object */, QString /* name (trigger) */> m_reverseActions;

  void processTree(const QAccessibleClient::AccessibleObject &object, bool added, bool reset);
  void removeAction(const QString& name, const QAccessibleClient::AccessibleObject& o);

  inline QString cleanString(const QString& input);
};

#endif
