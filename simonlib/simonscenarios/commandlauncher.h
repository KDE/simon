/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_COMMANDLAUNCHER_H
#define SIMON_COMMANDLAUNCHER_H

#include "simonmodelmanagement_export.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <KIcon>

/**
 *	@class CommandLauncher
 *
 *	@date 5.01.2010
 *	@author Peter Grasch
 */
class MODELMANAGEMENT_EXPORT  CommandLauncher : public QObject{

Q_OBJECT

private:
	QString m_plugin;
	QString m_icon;
	QString m_trigger;
	QString m_pluginTrigger;
	QString m_description;

public:
    CommandLauncher(const QString& iconSrc_, const QString& trigger_, const QString& description_)
	    : m_icon(iconSrc_),
	      m_trigger(trigger_),
	      m_description(description_)
    {
    }

    void setPlugin(const QString& plugin)
    {
	    m_plugin = plugin;
    }

    void setPluginTrigger(const QString& pluginTrigger)
    {
	    m_pluginTrigger = pluginTrigger;
    }

    KIcon icon() const { return KIcon(m_icon); }
    QString iconSrc() const { return m_icon; }
    QString plugin() const { kDebug() << this; return m_plugin; }
    QString pluginTrigger() const { return m_pluginTrigger; }
    QString trigger() const { return m_trigger; }
    QString description() const { return m_description; }

};

#endif
