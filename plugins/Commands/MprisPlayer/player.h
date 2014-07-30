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

#ifndef SIMON_PLAYER_H_ABC0C7EC08DA40FEBD5B5BA233D7AC7C
#define SIMON_PLAYER_H_ABC0C7EC08DA40FEBD5B5BA233D7AC7C

#include <QDBusMetaType>

Q_DECLARE_METATYPE(QList<QVariantMap>)

class QDBusObjectPath;
class QDBusPendingCallWatcher;

/**
 *	@class Player
 *	@brief Manages Tracklist of an MPRIS media player
 *
 *	@version 0.1
 *	@date 29.06.2014
 *	@author Ashish Madeti
 */

class Player : public QObject
{
    Q_OBJECT

public:
    Player(const QString& serviceName, QObject *parent = 0);
    ~Player();

public slots:
    void trackAdded(const QVariantMap& metadata, const QDBusObjectPath& afterTrack);
    void trackRemoved(const QDBusObjectPath& trackId);
    void trackMetadataChanged(const QDBusObjectPath& trackId, const QVariantMap& metadata);
    void tracklistReplaced(const QList<QDBusObjectPath>& tracks, const QDBusObjectPath& currentTrack);

private slots:
    void tracksPropertyReceived(QDBusPendingCallWatcher* watcher);
    void tracksMetadataReceived(QDBusPendingCallWatcher* watcher);

private:
    void connectTracklistSignals();
    void getAllTracks();

    QHash<QString, QString> m_currentTracklist;
    QString m_serviceName;
};

#endif // SIMON_PLAYER_H_ABC0C7EC08DA40FEBD5B5BA233D7AC7C
