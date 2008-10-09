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


#define WORD 1
#define INFO 2
#define ERROR 4

#define STARTMINIMIZED 8
#define CONNECTONSTART 16
#define AUTOSTART 32




class QSettings;
class QString;
class QVariant;
/**
 * \class Settings
 * \author Peter Grasch
 * \date 12.08.2007
 * \version 0.1
 * \brief Provides a convient way to store and retrieve settings
 * \note Uses QSettings
*/
class Settings{
private:
	static QSettings *settings;
public:
	static QVariant get(QString name);
	static QString getS(QString name);
	static bool getB(QString name);
	static void set(QString name, QVariant value);
	static void initSettings();

	

};

#endif
