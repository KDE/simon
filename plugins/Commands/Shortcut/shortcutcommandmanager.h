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


#ifndef SIMON_SHORTCUTCOMMANDMANAGER_H_8145931DA2E84AF5B282B047E2BD9FE9
#define SIMON_SHORTCUTCOMMANDMANAGER_H_8145931DA2E84AF5B282B047E2BD9FE9

#include <simonscenarios/commandmanager.h>


class CreateCommandWidget;

/**
 *	@class ShortcutCommandManager 
 *	@brief Manager for the executable commands
 *
 *	@version 0.1
 *	@date 22.05.2008
 *	@author Peter Grasch
 */
class ShortcutCommandManager : public CommandManager {
Q_OBJECT
public:
	const QString name() const;
	const KIcon icon() const;
	bool addCommandPrivate(Command *command);
	CreateCommandWidget* getCreateCommandWidget(QWidget *parent);

	DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_H;

    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    ShortcutCommandManager (QObject *parent, const QVariantList& args);

    
    ~ShortcutCommandManager ();

};

#endif
