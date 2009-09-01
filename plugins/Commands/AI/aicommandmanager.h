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

#ifndef AICOMMANDMANAGER_H
#define AICOMMANDMANAGER_H

#include <commandpluginbase/commandmanager.h>

#include <QVariantList>

/**
 *	@class AICommandManager
 *	@brief Manager of the ScreenGrid
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class AIMLParser;
class KProcess;

class AICommandManager : public CommandManager{
Q_OBJECT

private:
	AIMLParser *parser;
	KProcess *festivalProc;

protected:
	bool trigger(const QString& triggerName);

public:
	bool deSerializeConfig(const QDomElement& elem, const QString& scenarioId);
	const QString name() const;
	bool load();
	bool setupParser();
	bool save();
	bool addCommand(Command *) { return false; }

	CommandConfiguration* getConfigurationPage();

    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    AICommandManager(QObject *parent, const QVariantList& args);

    
    ~AICommandManager();

};

#endif
