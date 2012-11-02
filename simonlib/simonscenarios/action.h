/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_ACTION_H_E2E10CBF867745BA82754A32EA168B28
#define SIMON_ACTION_H_E2E10CBF867745BA82754A32EA168B28

#include <QString>
#include <QObject>
#include <QPointer>
#include <QIcon>
#include <KLocalizedString>
#include <QDomElement>
#include <simonscenariobase/scenarioobject.h>
#include "simonmodelmanagement_export.h"
class CommandManager;
class Command;
class VersionNumber;
class CommandConfiguration;
class ActionCollection;

class MODELMANAGEMENT_EXPORT Action : public QObject, public ScenarioObject
{
  Q_OBJECT

    signals:
  void changed();

  private:
    QString m_source;
    QString m_trigger;
    QString m_version;
    VersionNumber *pluginMinVersion;
    VersionNumber *pluginMaxVersion;
    QPointer<CommandManager> m_manager;
    ActionCollection *m_actionCollection;
    bool m_enabledByDefault;

    void init(const QString& source, const QString& trigger=QString());
  public:
    void assignParent(Scenario *s);
    Action(Scenario *parent, const QString& source, const QString& trigger, ActionCollection *actionCollection);
    typedef QPointer<Action> Ptr;

    static Action* createAction(Scenario *parent, const QDomElement& elem, ActionCollection *actionCollection);

    bool enabledByDefault() { return m_enabledByDefault; }
    QString source() { return m_source; }
    QString trigger() { return m_trigger; }

    QString getPluginVersion() { return m_version; }

    QIcon icon();
    QPointer<CommandManager> manager() { return m_manager; }
    bool hasCommands();
    bool setTrigger(const QString& newTrigger);

    bool deSerialize(const QDomElement&);
    QDomElement serialize(QDomDocument *doc);

    bool removeCommand(Command *command);
    CommandConfiguration* getConfigurationPage();

    void setPluginFont(const QFont& font);

    void finalize();
    ~Action();

};
#endif
