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

#include "player.h"
#include "mprisconstants.h"
#include "mprisplayercommand.h"
#include "mprisplayercommandmanager.h"

#include <QDBusConnection>
#include <QDBusObjectPath>
#include <QDBusInterface>
#include <QDBusPendingCallWatcher>
#include <QDBusMetaType>
#include <QDebug>

Player::Player(const QString& serviceName, QObject *parent)
    : QObject(parent),
      m_serviceName(serviceName)
{
    qDBusRegisterMetaType< QList<QVariantMap> >();
    getAllTracks();
    connectTracklistSignals();
}

Player::~Player()
{
}

void Player::trackAdded(const QVariantMap& metadata, const QDBusObjectPath& afterTrack)
{
    Q_UNUSED(afterTrack)

    QString trackId = metadata[trackIdMetadata].value<QDBusObjectPath>().path();
    QString title = metadata[titleMetadata].toString();
    m_currentTracklist[trackId] = title;

    TrackList m;
    m << Track(trackId, title);
    static_cast<MprisPlayerCommandManager*>(parent())->
            addToCommandlist(m, m_serviceName);
}

void Player::trackRemoved(const QDBusObjectPath& trackId)
{
    m_currentTracklist.remove(trackId.path());
    static_cast<MprisPlayerCommandManager*>(parent())->
            removeFromCommandlist(QStringList() << trackId.path(), m_serviceName);
}

void Player::trackMetadataChanged(const QDBusObjectPath& trackId, const QVariantMap& metadata)
{
    QString newTrackId;
    QString newTitle;
    if (m_currentTracklist.contains(trackId.path())) {
        newTrackId = metadata[trackIdMetadata].value<QDBusObjectPath>().path();
        newTitle = metadata[titleMetadata].toString();
        m_currentTracklist[newTrackId] = newTitle;
        if (trackId.path() != newTrackId) {
            m_currentTracklist.remove(trackId.path());
        }

        static_cast<MprisPlayerCommandManager*>(parent())->
                removeFromCommandlist(QStringList() << trackId.path(), m_serviceName);

        TrackList m;
        m << Track(newTrackId, newTitle);
        static_cast<MprisPlayerCommandManager*>(parent())->
                addToCommandlist(m, m_serviceName);
    }
}

void Player::tracklistReplaced(const QList<QDBusObjectPath>& tracks, const QDBusObjectPath& currentTrack)
{
    Q_UNUSED(currentTrack)

    QDBusInterface iface(m_serviceName, DBusMprisPath, TracklistInterface);
    QDBusPendingCall pendingCall = iface.asyncCall("GetTracksMetadata",
                                                   QVariant::fromValue< QList<QDBusObjectPath> >(tracks));

    connect(new QDBusPendingCallWatcher(pendingCall, this),
            SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(tracksMetadataReceived(QDBusPendingCallWatcher*)));
}

void Player::tracksMetadataReceived(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply< QList<QVariantMap> > reply = *watcher;

    if (reply.error().isValid()) {
        qDebug() << "GetTracksMetadata for service " << m_serviceName
                 << " failed with error: " << reply.error().message();
        return;
    }

    static_cast<MprisPlayerCommandManager*>(parent())->
            removeFromCommandlist(m_currentTracklist.uniqueKeys(), m_serviceName);
    m_currentTracklist.clear();

    QString trackId;
    TrackList m;
    foreach (const QVariantMap& metadata, reply.value()) {
        trackId = metadata[trackIdMetadata].value<QDBusObjectPath>().path();
        m_currentTracklist[trackId] = metadata[titleMetadata].toString();
        m << Track(trackId, metadata[titleMetadata].toString());
    }

    static_cast<MprisPlayerCommandManager*>(parent())->
            addToCommandlist(m, m_serviceName);

    watcher->deleteLater();
}

void Player::connectTracklistSignals()
{

    QDBusConnection::sessionBus().connect(m_serviceName,
                                          DBusMprisPath,
                                          TracklistInterface,
                                          "TrackAdded",
                                          this,
                                          SLOT(trackAdded(QVariantMap,QDBusObjectPath)));
    QDBusConnection::sessionBus().connect(m_serviceName,
                                          DBusMprisPath,
                                          TracklistInterface,
                                          "TrackRemoved",
                                          this,
                                          SLOT(trackRemoved(QDBusObjectPath)));
    QDBusConnection::sessionBus().connect(m_serviceName,
                                          DBusMprisPath,
                                          TracklistInterface,
                                          "TrackMetadataChanged",
                                          this,
                                          SLOT(trackMetadataChanged(QDBusObjectPath,QVariantMap)));
    QDBusConnection::sessionBus().connect(m_serviceName,
                                          DBusMprisPath,
                                          TracklistInterface,
                                          "TrackListReplaced",
                                          this,
                                          SLOT(tracklistReplaced(QList<QDBusObjectPath>,QDBusObjectPath)));
}

void Player::getAllTracks()
{
    QDBusInterface propsIface(m_serviceName, DBusMprisPath, PropertiesInterface);
    QDBusPendingCall pendingCall = propsIface.asyncCall("Get",TracklistInterface, "Tracks");

    connect(new QDBusPendingCallWatcher(pendingCall, this),
            SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(tracksPropertyReceived(QDBusPendingCallWatcher*)));

}

void Player::tracksPropertyReceived(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<QVariant> reply = *watcher;
    if (reply.isError()) {
        qDebug() << "\"Get\" for property \"Tracks\" for service "
                 << m_serviceName << " failed with error: "
                 << reply.error().message();
        return;
    }

    QList<QDBusObjectPath> tracks;
    const QDBusArgument& arg = reply.value().value<QDBusArgument>();
    arg.beginArray();
    while (!arg.atEnd()) {
        tracks << arg.asVariant().value<QDBusObjectPath>();
    }
    arg.endArray();

    tracklistReplaced(tracks, QDBusObjectPath());
    watcher->deleteLater();
}
