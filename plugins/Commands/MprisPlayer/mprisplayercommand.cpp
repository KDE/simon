/*
 *   Copyright (C) 2014 Ashish Madeti <ashishmadeti@gmail.com>
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

#include "mprisplayercommand.h"
#include "mprisplayercommandmanager.h"

#include <QObject>
#include <QVariant>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusVariant>
#include <QDBusInterface>

#include <KIcon>
#include <KLocalizedString>

const QString MprisPlayerCommand::staticCategoryText()
{
    return i18n("Media Player Control");
}

const QString MprisPlayerCommand::getCategoryText() const
{
    return MprisPlayerCommand::staticCategoryText();
}

const KIcon MprisPlayerCommand::staticCategoryIcon()
{
    return KIcon("applications-multimedia");
}

const KIcon MprisPlayerCommand::getCategoryIcon() const
{
    return MprisPlayerCommand::staticCategoryIcon();
}

CommandRole MprisPlayerCommand::role()
{
    return m_role;
}

QString MprisPlayerCommand::trackId()
{
    return m_trackId.path();
}

QString MprisPlayerCommand::serviceName()
{
    return m_serviceName;
}

QDomElement MprisPlayerCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
    QDomElement roleElem = doc->createElement("role");
    roleElem.appendChild(doc->createTextNode(QString::number((int)m_role)));
    commandElem.appendChild(roleElem);

    return commandElem;
}


bool MprisPlayerCommand::deSerializePrivate(const QDomElement& commandElem)
{
    QDomElement roleElem = commandElem.firstChildElement("role");

    if (roleElem.isNull())
        return false;
    m_role = static_cast<CommandRole>(roleElem.text().toInt());

    return true;
}

const QMap<QString,QVariant> MprisPlayerCommand::getValueMapPrivate() const
{
    QMap<QString,QVariant> out;
    QString typeString;
    switch (m_role) {
    case PlayPause:
        typeString = i18nc("Toggle between Play and Pause", "Play / Pause");
        break;
    case Play:
        typeString = i18nc("Start playing the media", "Play");
        break;
    case Pause:
        typeString = i18nc("Pause the media", "Pause");
        break;
    case Stop:
        typeString = i18nc("Stop the media", "Stop");
        break;
    case Next:
        typeString = i18nc("Play the next media in the list", "Next");
        break;
    case Previous:
        typeString = i18nc("Play the previous media in the list", "Previous");
        break;
    case VolumeUp:
        typeString = i18nc("Increase the volume of the player", "Increase Volume");
        break;
    case VolumeDown:
        typeString = i18nc("Decrease the volume of the player", "Decrease Volume");
        break;
    case SeekAhead:
        typeString = i18nc("Fast forward to some time ahead", "Seek Ahead");
        break;
    case SeekBack:
        typeString = i18nc("Rewind to some time back", "Seek Back");
        break;
    case PlayParticular:
        typeString = i18nc("Play a particular track from a playlist", "Play Particular");
        break;
    }
    out.insert(i18nc("Type of the command", "Type"), typeString);
    return out;
}

bool MprisPlayerCommand::triggerPrivate(int *state)
{
    Q_UNUSED(state);
    QStringList playersList;
    playersList << static_cast<MprisPlayerCommandManager*>(parent())->targetServices();

    QString method;
    QList<QVariant> args;
    QDBusMessage m;
    qlonglong offset;
    double currentVolume, change;

    bool success = true;
    switch (m_role) {
    case PlayPause:
        method = "PlayPause";
        break;
    case Play:
        method = "Play";
        break;
    case Pause:
        method = "Pause";
        break;
    case Stop:
        method = "Stop";
        break;
    case Next:
        method = "Next";
        break;
    case Previous:
        method = "Previous";
        break;
    case VolumeUp:
    case VolumeDown:
        change = volumeChange;
        if (m_role == VolumeDown) {
            change *= -1.0;
        }
        foreach (const QString& service, playersList) {
            currentVolume = getPropertyValue(service, PlayerInterface, "Volume").value().toDouble();
            if (success) {
                success = setPropertyValue(service, PlayerInterface, "Volume", QVariant(currentVolume + change));
            }
        }
        return success;
    case SeekAhead:
    case SeekBack:
        offset = seekOffset;
        if (m_role == SeekBack) {
            offset *= -1;
        }
        args << offset;
        method = "Seek";
        break;
    case PlayParticular:
        m = QDBusMessage::createMethodCall(m_serviceName, DBusMprisPath,
                                                        TracklistInterface,
                                                        "GoTo");
        args << QVariant::fromValue(m_trackId);
        m.setArguments(args);
        success = QDBusConnection::sessionBus().send(m);
        return success;
    }

    foreach (const QString& service, playersList) {
        m = QDBusMessage::createMethodCall(service, DBusMprisPath, PlayerInterface, method);
        m.setArguments(args);
        if (success) {
            success = QDBusConnection::sessionBus().send(m);
        }
    }
    return success;
}

QDBusReply<QVariant> MprisPlayerCommand::getPropertyValue(const QString& service, const QString& interfaceName, const QString& propertyName)
{
    QDBusInterface *iface = new QDBusInterface(service, DBusMprisPath, PropertiesInterface,
                                               QDBusConnection::sessionBus());
    QDBusReply<QVariant> reply = iface->call("Get", interfaceName, propertyName);
    delete iface;

    return reply;
}

bool MprisPlayerCommand::setPropertyValue(const QString& service, const QString& interfaceName, const QString& propertyName, const QVariant& propValue)
{
    QDBusInterface *iface = new QDBusInterface(service, DBusMprisPath, PropertiesInterface,
                                               QDBusConnection::sessionBus());
    QDBusVariant arg(propValue);
    QDBusReply<void> reply = iface->call("Set", interfaceName, propertyName,
                                         QVariant::fromValue<QDBusVariant>(arg));
    delete iface;

    return reply.isValid();
}

STATIC_CREATE_INSTANCE_C(MprisPlayerCommand)
