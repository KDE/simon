/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_ACTIONCOLLECTION_H_5874BB16920A437B82CADE63873D25F9
#define SIMON_ACTIONCOLLECTION_H_5874BB16920A437B82CADE63873D25F9

#include <QString>
#include <QList>
#include "actionmodel.h"
#include "command.h"
#include "commandlistelements.h"
#include <simonscenariobase/scenarioobject.h>
#include <simonrecognitionresult/recognitionresult.h>
#include <simonscenarios/commandlistelements.h>
#include "simonmodelmanagement_export.h"

class Action;
class CreateCommandWidget;
class Command;
class QAction;
class ActionCommandModel;
class CommandConfiguration;
class VoiceInterfaceCommand;

class MODELMANAGEMENT_EXPORT ActionCollection : public ScenarioObject, public ActionModel
{

  private:
    ActionCommandModel *proxy;
    QHash<CommandListElements::Element, VoiceInterfaceCommand*> listInterfaceCommands;
    bool m_autorunActive;
    QString m_autorunCommand, m_autorunType;

  public:
    ActionCollection(Scenario *parent);

    static ActionCollection* createActionCollection(Scenario *parent, const QDomElement&);
    bool deSerialize(const QDomElement&);
    QDomElement serialize(QDomDocument *doc);
    static QDomElement createEmpty(QDomDocument *doc);

    QList<CreateCommandWidget*>* getCreateCommandWidgets(QWidget *parent);
    QList<CommandConfiguration*>* getConfigurationPages();

    ActionCommandModel* getProxy() { return proxy; }

    QList<Action*> actions() { return m_actions; }

    bool removeCommand(Command *command);
    bool setAutorun(bool active, const QString& type, const QString& trigger);
    bool autorunActive();
    QString autorunType();
    QString autorunTrigger();

    bool addAction(Action *action, bool silent, bool save);
    bool deleteAction(Action *action);

    bool moveActionUp(Action *action);
    bool moveActionDown(Action *action);

    bool processResult(RecognitionResult recognitionResult);
    bool triggerCommand(const QString& type, const QString& trigger);

    bool setTrigger(const QString& trigger);
    void setPluginFont(const QFont& font);

    QHash<CommandListElements::Element, VoiceInterfaceCommand*> getListInterfaceCommands();
    void setListInterfaceCommands(QHash<CommandListElements::Element, VoiceInterfaceCommand*> commands);

    CommandList* getCommandList();

    QList<QAction*> getGuiActions();
    ~ActionCollection();

};
#endif
