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

#include "actionmanager.h"
#include "settings.h"
#include "contacts.h"
#include "contactcommand.h"
#include "destinations.h"
#include "destinationcommand.h"
#include <QDebug>
#include <QProcess>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>

ActionManager::ActionManager(Contacts *_contacts, Destinations *_destinations, QObject *parent) :
    QObject(parent),
    contacts(_contacts),
    destinations(_destinations),
    currentMode(Main)
{
}

void ActionManager::parseCallActions(const QString &result)
{
    if (result== Settings::answerCallTrigger()) {
        qDebug() << "Answering call...";
        QDBusMessage m = QDBusMessage::createMethodCall("Com.Nokia.Telephony.CallUi",
                                                        "/Call",
                                                        "Com.Nokia.Telephony.CallUi",
                                                        "AnswerCall");
        QDBusConnection::sessionBus().send(m);
    }
    if (result== Settings::callTrigger()) {
        qDebug() << "Starting dialing...";
        emit enterCallMode();

    }

    if (currentMode != Call) return;
    Command *c = contacts->getCommand(result);
    if (c) {
        //make call
        QString number = static_cast<ContactCommand*>(c)->number();
        qDebug() << "Calling: " << number;
        QDBusMessage m = QDBusMessage::createMethodCall("com.nokia.csd.Call",
                                                        "/com/nokia/csd/call",
                                                        "com.nokia.csd.Call",
                                                        "CreateWith");
        QList<QVariant> args;
        args << number;
        args << 0;
        m.setArguments(args);
        QDBusConnection::systemBus().send(m);
    }
}

void ActionManager::parseNavigationActions(const QString &result)
{
    if (result== Settings::startNavigationTrigger()) {
        qDebug() << "Starting navigation";
        emit enterNavigationMode();
    }
    if (result== Settings::stopNavigationTrigger()) {
        qDebug() << "Stopping current navigation...";
        QDBusMessage m = QDBusMessage::createMethodCall("com.nokia.Drive",
                                                        "/org/maemo/m",
                                                        "com.nokia.MApplicationIf",
                                                        "exit");
        QDBusConnection::sessionBus().send(m);

    }

    if (currentMode != Navigation) return;
    Command *c = destinations->getCommand(result);
    if (c) {
        //navigation
        QString destination = static_cast<DestinationCommand*>(c)->destination();
        qDebug() << "Navigating to: " << destination;
        QProcess::startDetached("drive-qml", QStringList() <<  "geo:0,0?searchTerm="+destination+"&action=driveTo");
    }
}

void ActionManager::recognized(const RecognitionResultList& results)
{
    if (results.isEmpty())
        return;

    QString result = results.first().sentence();
    emit actionExecuted(result);

    if (!Settings::processActions())
        return;
    qDebug() << "Processing action on device...";

    if (Settings::callControl()) {
        parseCallActions(result);
    }

    if (Settings::navigationControl()) {
        parseNavigationActions(result);
    }

    if (result == Settings::backToMainTrigger())
        emit returnToMain();
}

void ActionManager::mainMode()
{
    currentMode = Main;
}

void ActionManager::callMode()
{
    currentMode = Call;
}

void ActionManager::navigationMode()
{
    currentMode = Navigation;
}
