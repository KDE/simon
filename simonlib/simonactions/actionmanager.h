/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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


#include <QObject>
#include <QList>
#include <simonrecognitionresult/recognitionresult.h>
#include <simonscenarios/command.h>
#include <simonscenarios/commandlistelements.h>
#include <simonactions/greedyreceiver.h>
#include "actionlib_export.h"
#include <KIcon>
#include <simonscenarios/action.h>

class CommandManager;
class CommandLauncher;
class CommandSettings;
class Action;

class SIMONACTIONS_EXPORT ActionManager : public QObject {

Q_OBJECT

private:
	static ActionManager* instance;

	QList<Action::Ptr> actions;

	RecognitionResultList *currentlyPromptedListOfResults;
	QList<GreedyReceiver*> *greedyReceivers;

	float minimumConfidenceThreshold;
	bool useDYM;


protected:
	ActionManager(QObject *parent=0);

private slots:
	void resultSelectionDone();


public:
	void retrieveRecognitionResultFilteringParameters();
	static ActionManager* getInstance() {
		if (!instance) instance = new ActionManager();
		return instance;
	}

	void processRawResults(RecognitionResultList* recognitionResults);
	void presentUserWithResults(RecognitionResultList* recognitionResults);
	bool processResult(RecognitionResult recognitionResult);
	bool runLauncher(const QString& trigger);
	bool triggerCommand(const QString& type, const QString& trigger);

	void deRegisterGreedyReceiver(GreedyReceiver *);
	void registerGreedyReceiver(GreedyReceiver *);

	CommandList* getCommandList();
	QList<CommandLauncher*> getLauncherList();
	QFont pluginBaseFont();
	QHash<CommandListElements::Element, VoiceInterfaceCommand*> getGlobalListInterfaceCommands();
	QHash<CommandListElements::Element, VoiceInterfaceCommand*> getListInterfaceCommands();

	~ActionManager();

};

#endif

