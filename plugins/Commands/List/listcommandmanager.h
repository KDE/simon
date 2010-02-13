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

#ifndef SIMON_LISTCOMMANDMANAGER_H_84821C333D2D473289D5FBC6A6F02DE3
#define SIMON_LISTCOMMANDMANAGER_H_84821C333D2D473289D5FBC6A6F02DE3

#include <simonscenarios/commandmanager.h>
#include <QVariantList>

class CreateListCommandWidget;
/**
 *	@class ListCommandManager
 *	@brief Manager for the list commands
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class ListCommandManager : public CommandManager {
Q_OBJECT
protected:
	bool shouldAcceptCommand(Command *c);

public:
	const QString iconSrc() const;
	const QString name() const;
	CreateCommandWidget* getCreateCommandWidget(QWidget *parent);

	bool deSerializeConfig(const QDomElement& elem);

	void setFont(const QFont& font);

	DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_H;

    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    ListCommandManager(QObject* parent, const QVariantList& args);

    
    ~ListCommandManager();

};

#endif
