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

#include "qmlsimoneview.h"
#include "qmlapplicationviewer.h"
#include "settings.h"
#include "contacts.h"
#include "destinations.h"
#include <QMetaObject>
#include <QGraphicsObject>
#include <QDeclarativeContext>
#include <QStringList>
#include <QDebug>

QMLSimoneView::QMLSimoneView(Contacts *contacts, Destinations *destinations, QObject *parent) :
    SimoneView(contacts, destinations, parent),
    viewer(new QmlApplicationViewer()), state(Unconnected),
    skipNonEssentialUIUpdates(false)
{
    //viewer->setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);
    viewer->rootContext()->setContextProperty("simoneView", this);
    viewer->rootContext()->setContextProperty("contactsModel", contacts);
    viewer->rootContext()->setContextProperty("destinationsModel", destinations);

    contacts->startLoading();
    destinations->startLoading();
    viewer->setMainQmlFile(QLatin1String("qml/simone/main.qml"));
    contacts->stopLoading();
    destinations->stopLoading();

    connect(connectionButton(), SIGNAL(clicked()),
                                 this, SLOT(toggleConnection()));
    connect(viewer->rootObject()->findChild<QObject*>("btBackToMain"), SIGNAL(clicked()),
                                 this, SLOT(displayMainMenu()));

    restoreConfiguration();
    buildStaticCommandList();
    adaptToVad();

}

QMLSimoneView::~QMLSimoneView()
{
    delete viewer;
}

void QMLSimoneView::pauseUpdates()
{
    qDebug() << "Pause";
    skipNonEssentialUIUpdates = true;
}

void QMLSimoneView::resumeUpdates()
{
    qDebug() << "Resume";
    skipNonEssentialUIUpdates = false;
}

void QMLSimoneView::toggleConnection()
{
    switch (state) {
    case Unconnected:
        emit connectToServer();
        break;
    case ConnectedWaiting:
    case Connecting:
    case Connected:
    case Disconnecting:
        emit disconnectFromServer();
        break;
    }
}

void QMLSimoneView::startRecordingRequested()
{
    if (state != Connected)
        return;
    emit startRecording();
}

void QMLSimoneView::commitRecordingRequested()
{
    if (state != Connected)
        return;
    emit commitRecording();
}

void QMLSimoneView::displayStatus(const QString& status)
{
    QObject *lbStatus = viewer->rootObject()->findChild<QObject*>("lbStatus");
    lbStatus->setProperty("text", status);
}

void QMLSimoneView::displayError(const QString& error)
{
    qDebug() << "Error: " << error;
    viewer->rootObject()->findChild<QObject*>("lbError")->setProperty("text", error);
    QMetaObject::invokeMethod(viewer->rootObject()->findChild<QObject*>("errorDialog"),
                              "open");
}

void QMLSimoneView::displayExecutedAction(const QString& action)
{
    qDebug() << "Action executed: " << action;
    viewer->rootObject()->findChild<QObject*>("recognitionResultBanner")->setProperty("text", tr("Recognized: %1").arg(action));
    QMetaObject::invokeMethod(viewer->rootObject()->findChild<QObject*>("recognitionResultBanner"),
                              "show");
}

void QMLSimoneView::displayConnectionState(ConnectionState state_)
{
    state = state_;
    qDebug() << "State changed: " << state;
    adaptToVad();

    QObject *connectButton = connectionButton();
    switch (state) {
    case Unconnected:
        connectButton->setProperty("text", tr("Connect"));
        break;
    case Connecting:
        connectButton->setProperty("text", tr("Connecting..."));
        break;
    case ConnectedWaiting:
        connectButton->setProperty("text", tr("Disconnect"));
        break;
    case Connected:
        connectButton->setProperty("text", tr("Disconnect"));
        break;
    case Disconnecting:
        connectButton->setProperty("text", tr("Disconnecting..."));
        break;
    }

    //show busy indicator always except for the connected and unconnected states
    viewer->rootObject()->findChild<QObject*>("busyIndicator")->setProperty("visible",
                                                                            ((state != Unconnected) && (state != Connected)));
}

void QMLSimoneView::displayMicrophoneLevel(int level, int min, int max)
{
    if (skipNonEssentialUIUpdates)
        return;

    QObject *pbVUMeter = viewer->rootObject()->findChild<QObject*>("pbVUMeter");
    if (min != -1)
        pbVUMeter->setProperty("minimumValue", min);
    if (max != -1)
        pbVUMeter->setProperty("maximumValue", max);
    pbVUMeter->setProperty("value", level);
}

void QMLSimoneView::displayListening()
{
    speakLabel()->setProperty("text", tr("Listening..."));
}

void QMLSimoneView::displayRecognizing()
{
    speakLabel()->setProperty("text", tr("Please speak"));
}

void QMLSimoneView::displayMainMenu()
{
    qDebug() << "Displaying main menu";
    viewer->rootObject()->findChild<QObject*>("mainPage")->setProperty("state", "main");
    emit enteredMainMenu();
}

void QMLSimoneView::displayCallMenu()
{
    qDebug() << "Displaying call menu";
    viewer->rootObject()->findChild<QObject*>("mainPage")->setProperty("state", "subMenu");
    viewer->rootObject()->findChild<QObject*>("lvCommandsView")->setProperty("model",
                                                 viewer->rootContext()->contextProperty("contactsModel"));
    emit enteredCallMenu();
}

void QMLSimoneView::displayNavigationMenu()
{
    qDebug() << "Displaying navigation menu";
    viewer->rootObject()->findChild<QObject*>("mainPage")->setProperty("state", "subMenu");
    viewer->rootObject()->findChild<QObject*>("lvCommandsView")->setProperty("model",
                                                 viewer->rootContext()->contextProperty("destinationsModel"));

    emit enteredNavigationMenu();
}

void QMLSimoneView::adaptToVad()
{
    speakButton()->setProperty("visible", ((state == Connected) && (!Settings::voiceActivityDetection())));
    speakLabel()->setProperty("visible", ((state == Connected) && (Settings::voiceActivityDetection())));
}

void QMLSimoneView::restoreConfiguration()
{
    qDebug() << "Restoring configuration";
    QObject *rootObject = viewer->rootObject();
    rootObject->findChild<QObject*>("cbAutoConnect")->setProperty("checked", Settings::autoConnect());
    rootObject->findChild<QObject*>("cbLanguage")->setProperty("currentText", Settings::language());
    rootObject->findChild<QObject*>("cbUsePublicServer")->setProperty("checked", Settings::publicServer());
    rootObject->findChild<QObject*>("tfHost")->setProperty("text", Settings::host());
    rootObject->findChild<QObject*>("tfPort")->setProperty("text", QString::number(Settings::port()));
    rootObject->findChild<QObject*>("tfUser")->setProperty("text", Settings::user());
    rootObject->findChild<QObject*>("tfPassword")->setProperty("text", Settings::password());
    rootObject->findChild<QObject*>("cbProcessActions")->setProperty("checked", Settings::processActions());
    rootObject->findChild<QObject*>("cbVoiceActivityDetection")->setProperty("checked", Settings::voiceActivityDetection());
    rootObject->findChild<QObject*>("slCutoffLevel")->setProperty("value", Settings::cutoffLevel());
    rootObject->findChild<QObject*>("slHeadMargin")->setProperty("value", Settings::headMargin());
    rootObject->findChild<QObject*>("slTailMargin")->setProperty("value", Settings::tailMargin());
    rootObject->findChild<QObject*>("slMinimumSampleLength")->setProperty("value", Settings::minimumSampleLength());

    rootObject->findChild<QObject*>("cbBackToMain")->setProperty("currentText", Settings::backToMainTrigger());
    rootObject->findChild<QObject*>("cbContacts")->setProperty("checked", Settings::callControl());
    rootObject->findChild<QObject*>("cbContactTrigger")->setProperty("currentText", Settings::callTrigger());
    rootObject->findChild<QObject*>("cbContactAcceptCall")->setProperty("currentText", Settings::answerCallTrigger());
    rootObject->findChild<QObject*>("cbNavigation")->setProperty("checked", Settings::navigationControl());
    rootObject->findChild<QObject*>("cbNavigationTrigger")->setProperty("currentText", Settings::startNavigationTrigger());
    rootObject->findChild<QObject*>("cbStopNavigation")->setProperty("currentText", Settings::stopNavigationTrigger());

    //setting up contacts according to config
    contacts->startLoading();
    contacts->restore(Settings::contacts());
    contacts->stopLoading();
    destinations->startLoading();
    destinations->restore(Settings::destinations());
    destinations->stopLoading();

    qDebug() << "Restoring configuration: Done";
}

void QMLSimoneView::storeConfiguration()
{
    qDebug() << "Storing configuration";
    QObject *rootObject = viewer->rootObject();
    Settings::setAutoConnect(rootObject->findChild<QObject*>("cbAutoConnect")->property("checked").toBool());
    Settings::setPublicServer(rootObject->findChild<QObject*>("cbUsePublicServer")->property("checked").toBool());
    Settings::setLanguage(rootObject->findChild<QObject*>("cbLanguage")->property("currentText").toString());
    Settings::setHost(rootObject->findChild<QObject*>("tfHost")->property("text").toString());
    Settings::setPort(rootObject->findChild<QObject*>("tfPort")->property("text").toInt());
    Settings::setUser(rootObject->findChild<QObject*>("tfUser")->property("text").toString());
    Settings::setPassword(rootObject->findChild<QObject*>("tfPassword")->property("text").toString());
    Settings::setProcessActions(rootObject->findChild<QObject*>("cbProcessActions")->property("checked").toBool());
    Settings::setVoiceActivityDetection(rootObject->findChild<QObject*>("cbVoiceActivityDetection")->property("checked").toBool());
    Settings::setCutoffLevel(rootObject->findChild<QObject*>("slCutoffLevel")->property("value").toInt());
    Settings::setHeadMargin(rootObject->findChild<QObject*>("slHeadMargin")->property("value").toInt());
    Settings::setTailMargin(rootObject->findChild<QObject*>("slTailMargin")->property("value").toInt());
    Settings::setMinimumSampleLength(rootObject->findChild<QObject*>("slMinimumSampleLength")->property("value").toInt());

    Settings::setBackToMainTrigger(rootObject->findChild<QObject*>("cbBackToMain")->property("currentText").toString());
    Settings::setCallControl(rootObject->findChild<QObject*>("cbContacts")->property("checked").toBool());
    Settings::setCallTrigger(rootObject->findChild<QObject*>("cbContactTrigger")->property("currentText").toString());
    Settings::setAnswerCallTrigger(rootObject->findChild<QObject*>("cbContactAcceptCall")->property("currentText").toString());
    Settings::setNavigationControl(rootObject->findChild<QObject*>("cbNavigation")->property("checked").toBool());
    Settings::setStartNavigationTrigger(rootObject->findChild<QObject*>("cbNavigationTrigger")->property("currentText").toString());
    Settings::setStopNavigationTrigger(rootObject->findChild<QObject*>("cbStopNavigation")->property("currentText").toString());
    Settings::store();

    adaptToVad();

    Settings::setContacts(contacts->store());
    Settings::setDestinations(destinations->store());

    emit configurationChanged();
}

void QMLSimoneView::show()
{
    viewer->showExpanded();
}

QObject* QMLSimoneView::connectionButton()
{
    return viewer->rootObject()->findChild<QObject*>("btConnect");
}

QObject* QMLSimoneView::speakButton()
{
    return viewer->rootObject()->findChild<QObject*>("btSpeak");
}

QObject* QMLSimoneView::speakLabel()
{
    return viewer->rootObject()->findChild<QObject*>("lbSpeak");
}


QVariant QMLSimoneView::getStaticCommandList()
{
    return currentLanguagePublicCommands;
}

void QMLSimoneView::buildStaticCommandList()
{
    currentLanguagePublicCommands.clear();

    QObject *languageSelection = viewer->rootObject()->findChild<QObject*>("cbLanguage");
    if (!languageSelection) return;

    QString language = languageSelection->property("currentText").toString();
    qDebug() << "Selected language: " << language;
    if (language == "English") {
        // phone
        currentLanguagePublicCommands << "Start dialing";
        currentLanguagePublicCommands << "Answer call";
        // navigation
        currentLanguagePublicCommands << "Start navigation";
        currentLanguagePublicCommands << "Stop navigation";
        // general
        currentLanguagePublicCommands << "Back";
        currentLanguagePublicCommands << "One";
        currentLanguagePublicCommands << "Two";
        currentLanguagePublicCommands << "Three";
        currentLanguagePublicCommands << "Four";
        currentLanguagePublicCommands << "Five";
        currentLanguagePublicCommands << "Six";
        currentLanguagePublicCommands << "Seven";
        currentLanguagePublicCommands << "Eight";
        currentLanguagePublicCommands << "Nine";
    }
    if (language == "Deutsch") {
        currentLanguagePublicCommands << "Nummer w�hlen";
        currentLanguagePublicCommands << "Anruf annehmen";
        currentLanguagePublicCommands << "Navigation starten";
        currentLanguagePublicCommands << "Navigation stoppen";
        currentLanguagePublicCommands << "Zur�ck";
        currentLanguagePublicCommands << "Eins";
        currentLanguagePublicCommands << "Zwei";
        currentLanguagePublicCommands << "Drei";
        currentLanguagePublicCommands << "Vier";
        currentLanguagePublicCommands << "F�nf";
        currentLanguagePublicCommands << "Sechs";
        currentLanguagePublicCommands << "Sieben";
        currentLanguagePublicCommands << "Acht";
        currentLanguagePublicCommands << "Neun";
    }

    emit publicCommandsChanged();
}

bool QMLSimoneView::commandSupported(const QString& command)
{
    qDebug() << "Trying to find out if " << command << " is supported: " << currentLanguagePublicCommands;
    return (currentLanguagePublicCommands.contains(command));
}

QString QMLSimoneView::getCommand(int command)
{
    return currentLanguagePublicCommands[command];
}
