//
// C++ Implementation: settings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "settings.h"
#include <QString>
#include <QVariant>
#include <QSettings>


QSettings* Settings::settings;

/**
 * \brief Static function static function to initialize the settings
 * \author Peter Grasch
 * 
 * this MUST be called before the first use of setting/set
 */
void Settings::initSettings()
{
	settings = new QSettings ( QSettings::IniFormat,QSettings::UserScope,"simonlistens","simon" );
	settings->sync();
}


/**
 * \brief Checks if the value is already set (it is not null) and sets it if it isn't
 *
 * @param option The setting to change
 * @param value The new value
 * \return true, if we changed anything
 */
bool Settings::checkAndSet(QString option, QVariant value)
{
	bool changed=false;
	if (Settings::get(option).isNull())
	{
		Settings::set(option, value);
		changed = true;
	}
	return changed;
}


/**
 * \brief returns the setting of the given name
 * \author Peter Grasch
 * If there is no setting "name" it returns a default QVariant
 * @param name the name of the setting
 * @return the retrieved setting
 */
QVariant Settings::get(QString name)
{
	return settings->value(name);
}

/**
 * \brief returns the setting-string of the given name; overloaded member function provided for convenience
 * \author Peter Grasch
 * If there is no setting "name" it returns a default QVariant
 * @param name the name of the setting
 * @return the retrieved setting
 */
QString Settings::getS(QString name)
{
	return get(name).toString();
}

/**
 * \brief returns the setting-int of the given name; overloaded member function provided for convenience
 * \author Peter Grasch
 * If there is no setting "name" it returns a default QVariant
 * @param name the name of the setting
 * @return the retrieved setting
 */
int Settings::getI(QString name)
{
	return get(name).toInt();
}

/**
 * \brief returns the setting-bool of the given name; overloaded member function provided for convenience
 * \author Peter Grasch
 * If there is no setting "name" it returns a default QVariant
 * @param name the name of the setting
 * @return the retrieved setting
 */
bool Settings::getB(QString name)
{
	return get(name).toBool();
}

/**
 * \brief returns the setting of the given name
 * \author Peter Grasch
 * @param name The name of the setting
 * @param value the value to set it to
 */
void Settings::set(QString name, QVariant value)
{
	settings->setValue(name, value);
}
