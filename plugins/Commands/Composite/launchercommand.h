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

#ifndef SIMON_LAUNCHERCOMMAND_H_F5A3CC93AA8147729EE9C50D4454925E
#define SIMON_LAUNCHERCOMMAND_H_F5A3CC93AA8147729EE9C50D4454925E

#include <simonscenarios/command.h>

class CommandLauncher;

/**
 *	@class LauncherCommand
 *	@date 8.01.2010
 *	@author Peter Grasch
 */
class LauncherCommand : public Command {
Q_OBJECT
private:
	CommandLauncher *launcher;

protected:
	const QMap<QString,QVariant> getValueMapPrivate() const;
	bool triggerPrivate();

public:
	QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem);
	static const QString staticCategoryText();
	static const KIcon staticCategoryIcon();

	const KIcon getCategoryIcon() const;
	const QString getCategoryText() const;

	LauncherCommand(CommandLauncher *launcher);
	LauncherCommand(const QString& iconSrc, const QString& trigger);


	void change(const QString& newName, const QString& newIconSrc, const QString& description, CommandLauncher *launcher)
	{ 
		this->launcher = launcher;
		Command::change(newName, newIconSrc, description);
	}

    ~LauncherCommand() {}

};

#endif
