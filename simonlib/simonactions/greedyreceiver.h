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

#ifndef SIMON_GREEDYRECEIVER_H_561D6EF525114CB7B4D84C003137CBE9
#define SIMON_GREEDYRECEIVER_H_561D6EF525114CB7B4D84C003137CBE9

#include "actionlib_export.h"
#include <simonrecognitionresult/recognitionresult.h>
#include <QString>

class CommandManager;

class SIMONACTIONS_EXPORT GreedyReceiver {

private:
	CommandManager *m_manager;

public:
	virtual bool greedyTriggerRawList(RecognitionResultList*);
	virtual bool greedyTriggerRaw(const RecognitionResult&);
	virtual bool greedyTrigger(const QString&);

	virtual void startGreedy();
	virtual void stopGreedy();
	
	GreedyReceiver(CommandManager *manager) : m_manager(manager)
	{ }

	virtual ~GreedyReceiver() {}

};

#endif
