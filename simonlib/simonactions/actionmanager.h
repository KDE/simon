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

#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H


#include <QObject>
#include <QList>
#include <simonrecognitionresult/recognitionresult.h>
#include <commandpluginbase/command.h>
#include <simonactions/greedyreceiver.h>
#include "actionlib_export.h"
#include <KIcon>
#include "action.h"

class CommandManager;
class CreateCommandWidget;
class CommandSettings;
class KCModule;
class Action;
class KXMLGUIClient;


/*
 * class GreedyReceiver {
	private:
		QObject *m_receiver;
		const char* m_slot;
	public:
		QObject* receiver() const { return m_receiver; }
		const char* slot() const { return m_slot; }

		GreedyReceiver(QObject *receiver, const char* slot) 
			: m_receiver(receiver), m_slot(slot) {}
}; */

class SIMONACTIONS_EXPORT ActionManager : public QObject {

Q_OBJECT

signals:
	void commandExecuted(QString trigger);
	void guiAction(QString trigger);
	void categoriesChanged(const QList<KIcon>& icons, const QStringList& names);
	void commandAdded(Command*);
	void commandRemoved(const QString& trigger, const QString& category);

private:
	static ActionManager* instance;
	KXMLGUIClient *mainWindow;

	CommandSettings* commandSettings;
	QList<GreedyReceiver*> greedyReceivers;

	QList<Action::Ptr> actions;

	bool askDeleteCommandByTrigger(QString trigger);

	void deleteManager(CommandManager *manager);

	void triggerCommand();


protected:
	ActionManager(QObject *parent=0);

private slots:
	void setupBackends(QList<Action::Ptr> pluginsToLoad);


public:
	void publishCategories();

	static ActionManager* getInstance() {
		if (!instance) instance = new ActionManager();
		return instance;
	}

	void setMainWindow(KXMLGUIClient *window);
	void setConfigurationDialog(KCModule*);
	KCModule* getConfigurationDialog() { return (KCModule*) commandSettings; }
	void init();

	bool triggerCommand(const QString& type, const QString& trigger);
	Command* getCommand(const QString& category, const QString& trigger);
	CommandList* getCommandsOfCategory(const QString& category);


	void processRawResults(const RecognitionResultList& recognitionResults);
	void processResult(RecognitionResult recognitionResult);
	void presentUserWithResults(const RecognitionResultList& recognitionResults);

	bool addCommand(Command *command);
	bool deleteCommand(Command *command);

	QList<CreateCommandWidget*>* getCreateCommandWidgets(QWidget *parent);

	CommandList* getCommandList();

	void deRegisterGreedyReceiver(GreedyReceiver *);
	void registerGreedyReceiver(GreedyReceiver *);

	~ActionManager();

};

#endif

