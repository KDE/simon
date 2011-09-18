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

#include <QtGui/QApplication>
#include <QObject>
#include "simondconnector.h"
#include "qmlsimoneview.h"
#include "actionmanager.h"
#include "contacts.h"
#include "destinations.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SimondConnector connector;
    Contacts contacts;
    Destinations destinations;
    QMLSimoneView view(&contacts, &destinations);
    ActionManager action(&contacts, &destinations);

    QObject::connect(&view, SIGNAL(connectToServer()), &connector, SLOT(connectToServer()));
    QObject::connect(&view, SIGNAL(disconnectFromServer()), &connector, SLOT(disconnectFromServer()));
    QObject::connect(&view, SIGNAL(startRecording()), &connector, SLOT(startRecording()));
    QObject::connect(&view, SIGNAL(commitRecording()), &connector, SLOT(commitRecording()));
    QObject::connect(&view, SIGNAL(configurationChanged()), &connector, SLOT(configurationChanged()));

    QObject::connect(&connector, SIGNAL(connectionState(ConnectionState)), &view, SLOT(displayConnectionState(ConnectionState)));
    QObject::connect(&connector, SIGNAL(status(QString)), &view, SLOT(displayStatus(QString)));
    QObject::connect(&connector, SIGNAL(error(QString)), &view, SLOT(displayError(QString)));
    QObject::connect(&connector, SIGNAL(listening()), &view, SLOT(displayListening()));
    QObject::connect(&connector, SIGNAL(recognizing()), &view, SLOT(displayRecognizing()));
    QObject::connect(&connector, SIGNAL(microphoneLevel(int,int,int)), &view, SLOT(displayMicrophoneLevel(int,int,int)));

    QObject::connect(&connector, SIGNAL(recognized(RecognitionResultList)), &action, SLOT(recognized(RecognitionResultList)));
    QObject::connect(&action, SIGNAL(actionExecuted(QString)), &view, SLOT(displayExecutedAction(QString)));

    QObject::connect(&action, SIGNAL(returnToMain()), &view, SLOT(displayMainMenu()));
    QObject::connect(&action, SIGNAL(enterCallMode()), &view, SLOT(displayCallMenu()));
    QObject::connect(&action, SIGNAL(enterNavigationMode()), &view, SLOT(displayNavigationMenu()));
    QObject::connect(&view, SIGNAL(enteredCallMenu()), &action, SLOT(callMode()));
    QObject::connect(&view, SIGNAL(enteredMainMenu()), &action, SLOT(mainMode()));
    QObject::connect(&view, SIGNAL(enteredNavigationMenu()), &action, SLOT(navigationMode()));

    view.show();
    connector.init();

    return app.exec();
}
