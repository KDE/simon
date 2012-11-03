/*
 *   Copyright (C) 2011 Frederik Gladhorn <gladhorn@kde.org>
 *   Copyright (C) 2011-2012 Peter Grasch <peter.grasch@bedahr.org>
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

#include <simonscenarios/commandmanager.h>

#include <QHash>
#include <QVariantList>
#include <QSharedPointer>
#include <QList>
#include <QVector>

class QAction;
class QTimer;
class ATSPIScanner;
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
  void scheduleLanguageModel(const QStringList& commands, bool reset);
  void schedulingTimeout();

  void setupLanguageModel(const QStringList& commands, bool reset);
  void adaptLanguageModel(const QStringList& toRemove, const QStringList& toAdd);
  void triggerAction(const QSharedPointer<QAction> action);

  void resultSelectionDone();
  
private:
  uint m_sentenceNr;
  ATSPIScanner *m_scanner;
  QVector<QSharedPointer<QAction> > m_pendingActions;
  QStringList m_lastCommands;

  QStringList m_proposedCommands;
  bool m_shouldReset;

  QTimer *m_updateGrouping;
  
  void clearDynamicLanguageModel();
  void clearATModel();
  
  ATSPIConfiguration* getATSPIConfiguration();
};

#endif
