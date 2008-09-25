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


#ifndef SETTINGS_H
#define SETTINGS_H

class QSettings;
class QString;
class QVariant;
/**
 * \class Settings
 * \author Peter Grasch
 * \date 12.08.2007
 * \version 0.1
 * \brief Provides a convinient way to store and retrieve settings
 * \note Uses QSettings
*/
class Settings{
private:
	static QSettings *settings;
public:
	static QVariant get(QString name);
	static QString 	getS(QString name);
	static int 	getI(QString name);
	static bool 	getB(QString name);
	static bool 	checkAndSet(QString option, QVariant value);

	static void set(QString name, QVariant value);
	static void initSettings();

	

};

#endif
