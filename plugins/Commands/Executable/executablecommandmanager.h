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

#ifndef SIMON_EXECUTABLECOMMANDMANAGER_H_1234C148A54C491F8324D40535523A70
#define SIMON_EXECUTABLECOMMANDMANAGER_H_1234C148A54C491F8324D40535523A70

#include <simonscenarios/commandmanager.h>
#include <QVariantList>

class CreateExecutableCommandWidget;
/**
 *	@class ExecutableCommandManager
 *	@brief Manager for the executable commands
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class ExecutableCommandManager : public CommandManager {
Q_OBJECT
public:
	bool addCommand(Command *command);
	const QString name() const;
	const KIcon icon() const;
	CreateCommandWidget* getCreateCommandWidget(QWidget *parent);

	DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_H;

    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    ExecutableCommandManager(QObject* parent, const QVariantList& args);

    
    ~ExecutableCommandManager();

};

#endif
