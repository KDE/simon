/*
 *   Copyright (C) 2011 Frederik Gladhorn <gladhorn@kde.org>
 *   Copyright (C) 2011-2012 Peter Grasch <grasch@simon-listens.org>
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

#include <accessibleobject.h>

#include <registry.h>
#include <simonscenarios/commandmanager.h>

#include <QHash>
#include <QVariantList>
#include <QList>

class QAction;
class ATSPIConfiguration;

class ATSPICommandManager : public CommandManager
{
  Q_OBJECT

public:
  const QString preferredTrigger() const { return QString(); }
  const QString iconSrc() const;
  const QString name() const;
  bool deSerializeConfig(const QDomElement& elem);

  ATSPICommandManager(QObject* parent, const QVariantList& args);
  
  virtual bool trigger(const QString& triggerName, bool silent);
  virtual void cleanup();

  ~ATSPICommandManager();
  
private slots:
  void windowActivated(const QAccessibleClient::AccessibleObject& object);
  void triggerAction(QAction* action);
  void nameChanged (const QAccessibleClient::AccessibleObject &object);
  void descriptionChanged (const QAccessibleClient::AccessibleObject &object);
  void visibleDataChanged (const QAccessibleClient::AccessibleObject &object);
  void modelChanged (const QAccessibleClient::AccessibleObject &object);
  
private:
  QAccessibleClient::Registry *m_registry;
  QList<QAction*> m_pendingActions;
  unsigned int sentenceNr; 
  QStringList lastCommands;
  
  void setupLanguageModel(const QStringList& commands);
  void adaptLanguageModel(const QStringList& toRemove, const QStringList& toAdd);
  void clearDynamicLanguageModel();
  void clearATModel();
  
  ATSPIConfiguration* getATSPIConfiguration();
  
  QHash<QString /* name (trigger) */, QAccessibleClient::AccessibleObject /* object id */> m_actions;
  QHash<QAccessibleClient::AccessibleObject /* object */, QString /* name (trigger) */> m_reverseActions;
};

#endif