/*  
    Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
    Copyright (C) 2008-2009 Pali Rohár <pali.rohar@gmail.com>

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License version 2 as published by the Free Software Foundation.

    This application is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.

*/

#include "simonskype.h"
#include "unistd.h"
#include <eventsimulation/eventhandler.h>
#include <KDebug>
#include <QTimer>
#include <QKeySequence>
#include <QApplication>
#include <KCmdLineArgs>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusInterface>


SimonSkype::SimonSkype() : dropVoiceMail(false)
{
  connect(&s, SIGNAL(newCall(const QString&, const QString&)), this, SLOT(newCall(const QString&, const QString&)));
  connect(&s, SIGNAL(callStatus(const QString&, const QString&)), this, SLOT(callStatus(const QString&, const QString&)));

  kDebug() << "Setting up skype connection";
  s.setOnline();
  kDebug() << "Done";

  if (KCmdLineArgs::parsedArgs()->isSet("c")) {
    s.makeCall(KCmdLineArgs::parsedArgs()->getOption("c"));
    exit(0);
  }

  if (KCmdLineArgs::parsedArgs()->isSet("y")) {
    foreach (const QString& call, s.searchActiveCalls())
      s.acceptCall(call);
    exit(0);
  }
  if (KCmdLineArgs::parsedArgs()->isSet("n")) {
    QTimer::singleShot(5000, qApp, SLOT(quit()));
    connect(&s, SIGNAL(voiceMailActive(int)), this, SLOT(voiceMailActive(int)));
    connect(&s, SIGNAL(voiceMessageSent()), this, SLOT(voiceMessageSent()));
    dropVoiceMail = true;
    foreach (const QString& call, s.searchActiveCalls())
      s.hangUp(call);
    //exit(0);
  } else if (!KCmdLineArgs::parsedArgs()->isSet("d")) {
    kWarning() << "Nothing to do. Provide either c, y, n or d switches to use simonskype. See: simonskype --help";
    exit(0);
  }

  //s.setOnline();
}

void SimonSkype::voiceMessageSent()
{
  kDebug() << "Sent voice message!";
  EventHandler::getInstance()->sendShortcut(QKeySequence("Meta+Shift+C"));
}

void SimonSkype::voiceMailActive(int id)
{
  if (!dropVoiceMail) return;
  s.stopVoiceMail(id);
}

void SimonSkype::callStatus(const QString &callId, const QString &status)
{
  if ((KCmdLineArgs::parsedArgs()->isSet("d")) && (status == "INPROGRESS")) {
    usleep(1000000);
    s.startSendingVideo(callId);
  }
}

void SimonSkype::newCall(const QString& callId, const QString& userId)
{
  if (dropVoiceMail) {
      s.hangUp(callId);
  }
  if (!KCmdLineArgs::parsedArgs()->isSet("d"))
    return;

  kDebug() << "Call: " << callId << userId;

  QDBusMessage m = QDBusMessage::createMethodCall("org.kde.simon",
                                                    "/ActionManager",
                                                    "local.ActionManager",
                                                    "triggerCommand");
  QList<QVariant> args;
  args << "Dialog";
  args << KCmdLineArgs::parsedArgs()->getOption("d").arg(userId);
  m.setArguments(args);
  QDBusConnection::sessionBus().send(m);
}
