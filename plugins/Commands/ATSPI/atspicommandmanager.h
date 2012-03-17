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

#ifndef SIMON_ATSPICOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_ATSPICOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37

#include "qt-atspi.h"
#include <simonscenarios/commandmanager.h>
#include <QVariantList>
#include <QDBusVariant>
#include <QDBusArgument>
#include <QDBusMessage>

class ATSPIConfiguration;
class AccessibleObject;
class DBusConnection;
class QDBusMessage;
class QTimer;

class ATSPICommandManager : public CommandManager
{
  Q_OBJECT

private slots:
  ATSPIConfiguration* getATSPIConfiguration();
  void registry(const QDBusMessage &message);
  void objectChanged();
  
  void newClient(const QString& change, int, int, QDBusVariant data, QSpiObjectReference reference);
  
  void serviceRemoved(AccessibleObject *service);
  
  void setupObjects();
  
private:
  unsigned int sentenceNr;
  DBusConnection *c;
  QTimer* setupObjectsTimeout;
  
  QStringList lastCommands;

  QList<AccessibleObject*> rootAccessibles;
  QVariant getProperty(const QString &service, const QString &path, const QString &interface, const QString &name);
  
  void setupLanguageModel(const QStringList& commands);
  void clearDynamicLanguageModel();
  
  void setupService(const QString& service, const QString& path);

public:
  const QString preferredTrigger() const { return QString(); }
  const QString iconSrc() const;
  const QString name() const;
  bool deSerializeConfig(const QDomElement& elem);

  ATSPICommandManager(QObject* parent, const QVariantList& args);
  
  virtual bool trigger(const QString& triggerName, bool silent);
  virtual void cleanup();

  ~ATSPICommandManager();

};
#endif
