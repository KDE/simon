/*
 *   Copyright (C) 2014 Peter Grasch <peter.grasch@bedahr.org>
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

#include "amarokcommand.h"
#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QVariant>
#include <QDBusConnection>
#include <QDBusMessage>
#include <KIcon>
#include <KLocalizedString>
#include <KDebug>

const QString AmarokCommand::staticCategoryText()
{
  return i18n("Amarok");
}


const QString AmarokCommand::getCategoryText() const
{
  return AmarokCommand::staticCategoryText();
}


const KIcon AmarokCommand::staticCategoryIcon()
{
  return KIcon("amarok");
}


const KIcon AmarokCommand::getCategoryIcon() const
{
  return AmarokCommand::staticCategoryIcon();
}


const QMap<QString,QVariant> AmarokCommand::getValueMapPrivate() const
{
  QMap<QString,QVariant> out;
  out.insert(i18nc("Path of the song to play", "Path"), m_path);
  return out;
}


bool AmarokCommand::triggerPrivate(int *state)
{
  QDBusMessage msg(QDBusMessage::createMethodCall("org.kde.amarok", "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player", "OpenUri"));
  msg.setArguments(QList<QVariant>() << m_path );
  QDBusConnection::sessionBus().call(msg);
  return true;
}


QDomElement AmarokCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
  QDomElement pathElem = doc->createElement("path");
  pathElem.appendChild(doc->createTextNode(m_path));
  commandElem.appendChild(pathElem);
  return commandElem;
}


bool AmarokCommand::deSerializePrivate(const QDomElement& commandElem)
{
  m_path = commandElem.firstChildElement("path").text();
  return true;
}


STATIC_CREATE_INSTANCE_C(AmarokCommand);
