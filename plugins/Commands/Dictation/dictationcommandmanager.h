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

#ifndef DICTATIONCOMMANDMANAGER_H
#define DICTATIONCOMMANDMANAGER_H

#include <simonscenarios/commandmanager.h>
#include <QVariantList>

/**
 *	@class DictationCommandManager
 *	@brief Manager for the dictation
 *
 *	@version 0.1
 *	@date 16.10.2008
 *	@author Peter Grasch
 */
class DictationCommandManager : public CommandManager {
Q_OBJECT

protected:
	bool trigger(const QString& triggerName);

public:
	const QString preferredTrigger() const { return ""; }
	bool addCommand(Command *) { return false; }
	const QString name() const;

    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    DictationCommandManager(QObject* parent, const QVariantList& args);

    
    ~DictationCommandManager();

};

#endif
