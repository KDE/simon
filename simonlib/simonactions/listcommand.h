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
#include "greedyreceiver.h"
#include "actionlib_export.h"
#include <QList>
#include <KUrl>
class CommandListWidget;

/**
 *	@class ListCommand
 *	@brief Describes a list command; Consists of several other commands
 *	
 *	@version 0.1
 *	@date 19.05.2008
 *	@author Peter Grasch
 */
class SIMONACTIONS_EXPORT ListCommand : public Command, public GreedyReceiver {
Q_OBJECT

signals:
	void entrySelected();
	void canceled();

private:
	CommandListWidget *clw;

	int startIndex;

	QStringList iconsrcs;
	QStringList commands;
	QStringList commandTypes;

	static QStringList numberIdentifiers;
	void listCurrentCommandSection();

protected:
	const QMap<QString,QVariant> getValueMapPrivate() const;
	bool triggerPrivate();
	virtual bool deSerializePrivate(const QDomElement& commandElem);
	ListCommand();
	void init();

private slots:
	bool processRequest(int index);
	void cancel();

public slots:
	bool greedyTrigger(const QString& inputText);

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
	ListCommand(const QString& name, const QString& iconSrc, const QString& description, const QStringList& commands, const QStringList& iconSrcs, const QStringList& commandTypes);
	

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
   

	void change(const QString& newName, const QString& newIconSrc, const QString& description, const QStringList& newCommands, const QStringList& newIconSrcs, const QStringList& newCommandTypes)
	{ 
		this->iconsrcs = newIconSrcs;
		this->commands = newCommands;
		this->commandTypes = newCommandTypes;
		Command::change(newName, newIconSrc, description);
	}
	void setFont(const QFont& font);

	STATIC_CREATE_INSTANCE_H(ListCommand);

    ~ListCommand();

};

#endif
