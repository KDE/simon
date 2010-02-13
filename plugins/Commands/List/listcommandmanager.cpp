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
#include "listcommandmanager.h"
#include <simonactions/listcommand.h>
#include "createlistcommandwidget.h"
#include <simonlogging/logger.h>
#include <simonscenarios/scenario.h>
#include <KLocalizedString>
#include <KStandardDirs>

K_PLUGIN_FACTORY( ListCommandPluginFactory, 
			registerPlugin< ListCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( ListCommandPluginFactory("simonlistcommand") )


ListCommandManager::ListCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args)
{
}

const QString ListCommandManager::iconSrc() const
{
	return "view-choose";
}

bool ListCommandManager::shouldAcceptCommand(Command *command)
{
	return (dynamic_cast<ListCommand*>(command) != NULL);
}

const QString ListCommandManager::name() const
{
	return ListCommand::staticCategoryText();
}

CreateCommandWidget* ListCommandManager::getCreateCommandWidget(QWidget *parent)
{
	return new CreateListCommandWidget(this, parent);
}

void ListCommandManager::setFont(const QFont& font)
{
	foreach (Command *c, *commands) {
		ListCommand *lc = dynamic_cast<ListCommand*>(c);
		if (!lc) continue;
		lc->setFont(font);
	}
}

bool ListCommandManager::deSerializeConfig(const QDomElement& elem)
{
	Q_UNUSED(elem);
	bool succ = true;
	succ &= installInterfaceCommand(NULL, "", i18n("Zero"), iconSrc(),
			i18n("Selects option \"back\" of the list"), true, false,
			SimonCommand::DefaultState+1, SimonCommand::DefaultState+1, "0", "selectBack");
	succ &= installInterfaceCommand(NULL, "", i18n("One"), iconSrc(),
			i18n("Selects option one of the list"), true, false,
			SimonCommand::DefaultState+1, SimonCommand::DefaultState+1, "1", "select1");
	succ &= installInterfaceCommand(NULL, "", i18n("Two"), iconSrc(),
			i18n("Selects option two of the list"), true, false,
			SimonCommand::DefaultState+1, SimonCommand::DefaultState+1, "2", "select2");
	succ &= installInterfaceCommand(NULL, "", i18n("Three"), iconSrc(),
			i18n("Selects option three of the list"), true, false,
			SimonCommand::DefaultState+1, SimonCommand::DefaultState+1, "3", "select3");
	succ &= installInterfaceCommand(NULL, "", i18n("Four"), iconSrc(),
			i18n("Selects option four of the list"), true, false,
			SimonCommand::DefaultState+1, SimonCommand::DefaultState+1, "4", "select4");
	succ &= installInterfaceCommand(NULL, "", i18n("Five"), iconSrc(),
			i18n("Selects option five of the list"), true, false,
			SimonCommand::DefaultState+1, SimonCommand::DefaultState+1, "5", "select5");
	succ &= installInterfaceCommand(NULL, "", i18n("Six"), iconSrc(),
			i18n("Selects option six of the list"), true, false,
			SimonCommand::DefaultState+1, SimonCommand::DefaultState+1, "6", "select6");
	succ &= installInterfaceCommand(NULL, "", i18n("Seven"), iconSrc(),
			i18n("Selects option seven of the list"), true, false,
			SimonCommand::DefaultState+1, SimonCommand::DefaultState+1, "7", "select7");
	succ &= installInterfaceCommand(NULL, "", i18n("Eight"), iconSrc(),
			i18n("Selects option eight of the list"), true, false,
			SimonCommand::DefaultState+1, SimonCommand::DefaultState+1, "8", "select8");
	succ &= installInterfaceCommand(NULL, "", i18n("Nine"), iconSrc(),
			i18n("Selects option \"back\" of the list"), true, false,
			SimonCommand::DefaultState+1, SimonCommand::DefaultState+1, "9", "selectNext");

	succ &= installInterfaceCommand(NULL, "", i18n("Cancel"), iconSrc(),
			i18n("Cancels the selection process"), true, true,
			SimonCommand::DefaultState+1, SimonCommand::DefaultState+1, "1", "selectCancel");
	
	return succ;
}

DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(ListCommandManager, ListCommand);

ListCommandManager::~ListCommandManager()
{
}
