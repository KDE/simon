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
#include "dictationcommandmanager.h"
#include <eventsimulation/eventhandler.h>
#include <KLocalizedString>

K_PLUGIN_FACTORY( DictationCommandPluginFactory, 
			registerPlugin< DictationCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( DictationCommandPluginFactory("simondictationcommand") )


DictationCommandManager::DictationCommandManager(QObject *parent, const QVariantList& args) :CommandManager(parent, args)  
{
}

bool DictationCommandManager::trigger(const QString& triggerName)
{
	EventHandler::getInstance()->sendWord(triggerName);
	return true;
}

const QString DictationCommandManager::name() const
{
	return i18n("Dictation");
}

bool DictationCommandManager::load()
{
	return true;
}

bool DictationCommandManager::save()
{
	return true;
}

DictationCommandManager::~DictationCommandManager()
{
}
