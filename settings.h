//
// C++ Interface: settings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SETTINGS_H
#define SETTINGS_H

class QSettings;
class QString;
class QVariant;
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class Settings{
private:
	static QSettings *settings;
public:
	static QVariant get(QString name);
	static void set(QString name, QVariant value);
	static void initSettings();

	

};

#endif
