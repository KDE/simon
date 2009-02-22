/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#ifndef ACTION_H
#define ACTION_H

#include <QString>
#include <QObject>
#include <QPointer>
#include <QIcon>
#include <KLocalizedString>
class CommandManager;

class Action : public QObject {
	Q_OBJECT
	private:
		QString m_source;
		QString m_trigger;
		CommandManager* m_manager;
		bool m_enabledByDefault;
	public:
		typedef QPointer<Action> Ptr;
		Action(const QString& source, const QString& trigger=QString());
		bool enabledByDefault() { return m_enabledByDefault; }
		QString source() { return m_source; }
		QString trigger() { return m_trigger; }

		QIcon icon();
		CommandManager* manager() { return m_manager; }
		void setTrigger(const QString& newTrigger) { m_trigger=newTrigger; }

		~Action();

};

#endif
