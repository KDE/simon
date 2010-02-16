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

#ifndef SIMON_LISTCOMMAND_H_B003A824E4CE4D008341F2E92534890A
#define SIMON_LISTCOMMAND_H_B003A824E4CE4D008341F2E92534890A

#include <simonscenarios/command.h>
#include <simonscenarios/voiceinterfacecommand.h>
#include "greedyreceiver.h"
#include "commandlistwidget.h"
#include "actionlib_export.h"
#include <QList>
#include <QObject>
#include <QHash>
#include <KUrl>
class CommandListWidget;
class VoiceInterfaceCommand;

/**
 *	@class ListCommand
 *	@brief Describes a list command; Consists of several other commands
 *	
 *	@version 0.1
 *	@date 19.05.2008
 *	@author Peter Grasch
 */
class SIMONACTIONS_EXPORT ListCommand : public QObject, public Command, public GreedyReceiver {
Q_OBJECT

signals:
	void entrySelected();
	void canceled();

private:
	QList<VoiceInterfaceCommand*> m_subCommands;
	CommandManager *m_parentManager;
	CommandListWidget *clw;

	int startIndex;

	QStringList iconsrcs;
	QStringList commands;
	QStringList commandTypes;

	void listCurrentCommandSection();
	QHash<CommandListElements::Element, VoiceInterfaceCommand*> getAdaption();

protected:
	const QMap<QString,QVariant> getValueMapPrivate() const;
	bool triggerPrivate(int *state);
	virtual bool deSerializePrivate(const QDomElement& commandElem);
	ListCommand(CommandManager *parentManager);
	void init();

private slots:
	bool processRequest(int index);

public slots:
	bool greedyTrigger(const QString& inputText);
	void cancel();

public:
	QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem);
	static const QString staticCategoryText();
	static const KIcon staticCategoryIcon();

	const KIcon getCategoryIcon() const;
	const QString getCategoryText() const;
	
	/**
	* @brief Constructor
	* 
	*	@author Peter Grasch
	*/
	ListCommand(CommandManager *parentManager, const QString& name, const QString& iconSrc, const QString& description, const QStringList& commands, const QStringList& iconSrcs, const QStringList& commandTypes);

	/**
	* @brief Returns the command list
	* 
	*	@author Peter Grasch
	*/
	QStringList getCommands() const { return this->commands; }
	QStringList getIconSrcs() const { return this->iconsrcs; }
 
	/**
	* @brief Returns the command list
	* 
	*	@author Peter Grasch
	*/
	QStringList getCommandTypes() const { return this->commandTypes; }
   
	void setFont(const QFont& font);

	static ListCommand* createInstance(CommandManager *manager, const QDomElement& element);

    ~ListCommand();

};

#endif
