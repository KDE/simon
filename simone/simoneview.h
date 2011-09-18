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

#ifndef SIMONEVIEW_H
#define SIMONEVIEW_H

#include "simone.h"
#include <QObject>

class Contacts;
class Destinations;

class SimoneView : public QObject
{
    Q_OBJECT

protected:
    Contacts *contacts;
    Destinations *destinations;

public:
    explicit SimoneView(Contacts *c, Destinations *d, QObject *parent = 0);
    virtual ~SimoneView();

signals:
    void connectToServer();
    void disconnectFromServer();

    void startRecording();
    void commitRecording();

    void configurationChanged();

    void enteredMainMenu();
    void enteredCallMenu();
    void enteredNavigationMenu();

public slots:
    virtual void show()=0;

    virtual void displayStatus(const QString& status)=0;
    virtual void displayError(const QString& error)=0;
    virtual void displayConnectionState(ConnectionState state)=0;

    virtual void displayExecutedAction(const QString& action)=0;

    virtual void displayMicrophoneLevel(int level, int min, int max)=0;
    virtual void displayListening()=0;
    virtual void displayRecognizing()=0;

    virtual void displayMainMenu()=0;
    virtual void displayCallMenu()=0;
    virtual void displayNavigationMenu()=0;

};

#endif // SIMONEVIEW_H
