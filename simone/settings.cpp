/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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

#include "settings.h"
#include <QSettings>
#include <QStringList>

Settings* Settings::instance = 0;


Settings::Settings() :
    settings(new QSettings("simon-listens.org", "simon"))
{
}

Settings::~Settings()
{
    delete settings;
}

Settings* Settings::getInstance()
{
    if (!instance)
        instance = new Settings();
    return instance;
}

void Settings::store()
{
    getInstance()->storeP();
}

void Settings::storeP()
{
    settings->sync();
}


template <typename T>
T Settings::getValue(const QString& key, T defaultArgument)
{
    QVariant v = settings->value(key, defaultArgument);
    return v.value<T>();
}

template <typename T>
void Settings::setValue(const QString& key, T value)
{
    settings->setValue(key, value);
}


bool Settings::autoConnect()
{
    return getInstance()->getValue<bool>("autoConnect", false);
}

bool Settings::publicServer()
{
    return getInstance()->getValue<bool>("publicServer", true);
}

QString Settings::language()
{
    return getInstance()->getValue<QString>("language", "English");
}

QString Settings::host()
{
    return getInstance()->getValue<QString>("host", "192.168.178.75"); //FIXME: something else
}

int Settings::port()
{
    return getInstance()->getValue<int>("port", 4444);
}

QString Settings::user()
{
    return getInstance()->getValue<QString>("user", "default");
}

QString Settings::password()
{
    return getInstance()->getValue<QString>("password", "nopassword");
}

bool Settings::processActions()
{
    return getInstance()->getValue<bool>("processActions", false);
}

bool Settings::voiceActivityDetection()
{
    return getInstance()->getValue<bool>("voiceActivityDetection", false);
}

int Settings::cutoffLevel()
{
    return getInstance()->getValue<int>("cutoffLevel", 4000);
}

int Settings::headMargin()
{
    return getInstance()->getValue<int>("headMargin", 300);
}

int Settings::tailMargin()
{
    return getInstance()->getValue<int>("tailMargin", 350);
}

int Settings::minimumSampleLength()
{
    return getInstance()->getValue<int>("minimumSampleLength", 100);
}

QString Settings::backToMainTrigger()
{
    return getInstance()->getValue<QString>("backToMain", "Back");
}

bool Settings::callControl()
{
    return getInstance()->getValue<bool>("callControl", true);
}

QString Settings::callTrigger()
{
    return getInstance()->getValue<QString>("callTrigger", "Start dialing");
}

QString Settings::answerCallTrigger()
{
    return getInstance()->getValue<QString>("callAnswerTrigger", "Answer call");
}

QStringList Settings::contacts()
{
    return getInstance()->getValue<QStringList>("contacts", QStringList());
}

bool Settings::navigationControl()
{
    return getInstance()->getValue<bool>("navigationControl", true);
}

QString Settings::startNavigationTrigger()
{
    return getInstance()->getValue<QString>("startNavigationTrigger", "Start navigation");
}

QString Settings::stopNavigationTrigger()
{
    return getInstance()->getValue<QString>("stopNavigationTrigger", "Stop navigation");
}

QStringList Settings::destinations()
{
    return getInstance()->getValue<QStringList>("destinations", QStringList());
}


void Settings::setAutoConnect(bool autoConnect)
{
    getInstance()->setValue<bool>("autoConnect", autoConnect);
}

void Settings::setPublicServer(bool publicServer)
{
    getInstance()->setValue<bool>("publicServer", publicServer);
}

void Settings::setLanguage(const QString& language)
{
    getInstance()->setValue<QString>("language", language);
}

void Settings::setHost(const QString& host)
{
    getInstance()->setValue<QString>("host", host);
}

void Settings::setPort(int port)
{
    getInstance()->setValue<int>("port", port);
}

void Settings::setUser(const QString& user)
{
    getInstance()->setValue<QString>("user", user);
}

void Settings::setPassword(const QString& password)
{
    getInstance()->setValue<QString>("password", password);
}

void Settings::setProcessActions(bool process)
{
    getInstance()->setValue<bool>("processActions", process);
}

void Settings::setVoiceActivityDetection(bool voiceActivityDetection)
{
    getInstance()->setValue<bool>("voiceActivityDetection", voiceActivityDetection);
}

void Settings::setCutoffLevel(int level)
{
    getInstance()->setValue<int>("cutoffLevel", level);
}

void Settings::setHeadMargin(int headMargin)
{
    getInstance()->setValue<int>("headMargin", headMargin);
}

void Settings::setTailMargin(int tailMargin)
{
    getInstance()->setValue<int>("tailMargin", tailMargin);
}

void Settings::setMinimumSampleLength(int minimumSampleLength)
{
    getInstance()->setValue<int>("minimumSampleLength", minimumSampleLength);
}

void Settings::setBackToMainTrigger(const QString &backToMain)
{
    getInstance()->setValue<QString>("backToMain", backToMain);
}

void Settings::setCallControl(bool callControl)
{
    getInstance()->setValue<bool>("callControl", callControl);
}

void Settings::setCallTrigger(const QString& callTrigger)
{
    getInstance()->setValue<QString>("callTrigger", callTrigger);
}

void Settings::setAnswerCallTrigger(const QString& trigger)
{
    getInstance()->setValue<QString>("callAnswerTrigger", trigger);
}

void Settings::setContacts(const QStringList &contacts)
{
    getInstance()->setValue<QStringList>("contacts", contacts);
}

void Settings::setNavigationControl(bool navigationControl)
{
    getInstance()->setValue<bool>("navigationControl", navigationControl);
}

void Settings::setStartNavigationTrigger(const QString& startNavigationTrigger)
{
    getInstance()->setValue<QString>("startNavigationTrigger", startNavigationTrigger);
}

void Settings::setStopNavigationTrigger(const QString& stopNavigationTrigger)
{
    getInstance()->setValue<QString>("stopNavigationTrigger", stopNavigationTrigger);
}

void Settings::setDestinations(const QStringList &destinations)
{
    getInstance()->setValue<QStringList>("destinations", destinations);
}
