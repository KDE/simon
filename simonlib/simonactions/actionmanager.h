/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_ACTIONMANAGER_H_232B26BFFBAC4D88A29901748E1EB441
#define SIMON_ACTIONMANAGER_H_232B26BFFBAC4D88A29901748E1EB441

#include "actionlib_export.h"
#include <QObject>
#include <QFont>
#include <QList>
#include <simonrecognitionresult/recognitionresult.h>
#include <simonscenarios/command.h>
#include <simonscenarios/commandlistelements.h>
#include <simonactions/greedyreceiver.h>
#include <KIcon>
#include <simonscenarios/action.h>

class Action;
class CommandManager;
class CommandSettings;
class VoiceInterfaceCommand;

class SIMONACTIONS_EXPORT ActionManager : public QObject
{
  Q_OBJECT
  Q_CLASSINFO("Simons action interface", "org.simon-listens.ActionManager")

  private slots:
    void resultSelectionDone();

  protected:
    ActionManager(QObject *parent=0);

  public slots:
    void processRawResults(const RecognitionResultList &recognitionResults);
    void presentUserWithResults(const RecognitionResultList &recognitionResults);
    bool processResult(RecognitionResult recognitionResult);
    bool triggerCommand(const QString& type, const QString& trigger, bool silent=false);

    void deRegisterGreedyReceiver(GreedyReceiver *);
    void registerGreedyReceiver(GreedyReceiver *);

    CommandList getCommandList();

    QFont pluginBaseFont();
  
    QHash<CommandListElements::Element, VoiceInterfaceCommand*> getGlobalListInterfaceCommands();
    QHash<CommandListElements::Element, VoiceInterfaceCommand*> getListInterfaceCommands();

  public:
    void retrieveRecognitionResultFilteringParameters();
    static ActionManager* getInstance();

    ~ActionManager();

  private:
    RecognitionResultList currentlyPromptedListOfResults;
    QList<GreedyReceiver*> greedyReceivers;
    static ActionManager *instance;

    float minimumConfidenceThreshold;
    bool useDYM;
    
  signals:
    void processedRecognitionResult(const RecognitionResult& result, bool accepted);
};
#endif
