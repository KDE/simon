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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class QSettings;

class Settings
{
private:
    QSettings *settings;
    static Settings* instance;

    Settings();
    ~Settings();

    static Settings* getInstance();

    template <typename T>
    T getValue(const QString& key, T defaultValue);

    template <typename T>
    void setValue(const QString& key, T value);

    void storeP();

public:
    //Getter----------
    static bool autoConnect();
    static bool publicServer();
    static QString language();
    static QString host();
    static int port();
    static QString user();
    static QString password();
    static bool processActions();
    static bool voiceActivityDetection();
    static int cutoffLevel();
    static int headMargin();
    static int tailMargin();
    static int minimumSampleLength();

    //triggers
    static QString backToMainTrigger();
    static bool callControl();
    static QString callTrigger();
    static QString answerCallTrigger();
    static QStringList contacts();

    static bool navigationControl();
    static QString startNavigationTrigger();
    static QString stopNavigationTrigger();
    static QStringList destinations();


    //Setter----------
    static void setAutoConnect(bool autoConnect);
    static void setPublicServer(bool publicServer);
    static void setLanguage(const QString& language);
    static void setHost(const QString& host);
    static void setPort(int port);
    static void setUser(const QString& user);
    static void setPassword(const QString& password);
    static void setProcessActions(bool process);
    static void setVoiceActivityDetection(bool vad);
    static void setCutoffLevel(int level);
    static void setHeadMargin(int headMargin);
    static void setTailMargin(int tailMargin);
    static void setMinimumSampleLength(int length);

    //triggers
    static void setBackToMainTrigger(const QString& backToMain);
    static void setCallControl(bool control);
    static void setCallTrigger(const QString& callTrigger);
    static void setAnswerCallTrigger(const QString& trigger);
    static void setContacts(const QStringList& contacts);

    static void setNavigationControl(bool control);
    static void setStartNavigationTrigger(const QString& trigger);
    static void setStopNavigationTrigger(const QString& trigger);
    static void setDestinations(const QStringList& destinations);

    static void store();
};

#endif // SETTINGS_H
