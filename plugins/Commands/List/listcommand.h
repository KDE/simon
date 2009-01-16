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

#ifndef LISTCOMMAND_H
#define LISTCOMMAND_H

#include <commandpluginbase/command.h>
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
class ListCommand : public Command{
Q_OBJECT
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

private slots:
	bool processRequest(int index);
	void cancel();

public slots:
	bool executeSelection(QString inputText);

public:
	static const QString staticCategoryText();
	static const KIcon staticCategoryIcon();

	const KIcon getCategoryIcon() const;
	const QString getCategoryText() const;

	
	/**
	* @brief Constructor
	* 
	*	@author Peter Grasch
	*/
	ListCommand(const QString& name, const QString& iconSrc, const QStringList& commands, const QStringList& iconSrcs, const QStringList& commandTypes);
	

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
   

	void change(const QString& newName, const QString& newIconSrc, const QStringList& newCommands, const QStringList& newIconSrcs, const QStringList& newCommandTypes)
	{ 
		this->iconsrcs = newIconSrcs;
		this->commands = newCommands;
		this->commandTypes = newCommandTypes;
		Command::change(newName, newIconSrc);
	}

    ~ListCommand();

};

#endif
