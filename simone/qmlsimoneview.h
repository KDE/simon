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

#ifndef QMLSIMONEVIEW_H
#define QMLSIMONEVIEW_H

#include "simoneview.h"
#include <QVariant>
#include <QStringList>

class QmlApplicationViewer;

class QMLSimoneView : public SimoneView
{
    Q_OBJECT

signals:
    void publicCommandsChanged();

private:
    QmlApplicationViewer *viewer;
    ConnectionState state;
    bool skipNonEssentialUIUpdates;

    QStringList currentLanguagePublicCommands;

    QObject* connectionButton();
    QObject* speakButton();
    QObject* speakLabel();

    void adaptToVad();

public:
    QMLSimoneView(Contacts *contacts, Destinations *destinations, QObject *parent=0);
    ~QMLSimoneView();

private slots:
    void toggleConnection();

public slots:
    void show();

    void displayStatus(const QString& status);
    void displayError(const QString& error);
    void displayConnectionState(ConnectionState state);

    void storeConfiguration();
    void restoreConfiguration();

    void startRecordingRequested();
    void commitRecordingRequested();

    void displayExecutedAction(const QString& action);

    void displayMicrophoneLevel(int level, int min, int max);
    void displayListening();
    void displayRecognizing();

    void displayMainMenu();
    void displayCallMenu();
    void displayNavigationMenu();

    void pauseUpdates();
    void resumeUpdates();

    QVariant getStaticCommandList();
    void buildStaticCommandList();
    bool commandSupported(const QString& command);
    QString getCommand(int command);
};

#endif // QMLSIMONEVIEW_H
