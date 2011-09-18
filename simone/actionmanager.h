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

#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QObject>
#include "recognitionresult.h"

class Contacts;
class Destinations;

class ActionManager : public QObject
{
    Q_OBJECT
signals:
    void enterCallMode();
    void enterNavigationMode();
    void returnToMain();

private:
    enum Mode {
        Main,
        Call,
        Navigation
    };

    Contacts *contacts;
    Destinations *destinations;
    Mode currentMode;

    void parseCallActions(const QString& result);
    void parseNavigationActions(const QString& result);

public:
    explicit ActionManager(Contacts *_contacts, Destinations *_destinations, QObject *parent = 0);

signals:
    void actionExecuted(const QString& name);

public slots:
    void recognized(const RecognitionResultList& results);
    void mainMode();
    void callMode();
    void navigationMode();

};

#endif // ACTIONMANAGER_H
