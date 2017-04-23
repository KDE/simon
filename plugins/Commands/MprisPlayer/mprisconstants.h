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

#ifndef SIMON_MPRIS_CONSTANTS_H_9A22E03B797C494599B9629C6B47E478
#define SIMON_MPRIS_CONSTANTS_H_9A22E03B797C494599B9629C6B47E478

#include <QPair>
#include <QString>

typedef QPair<QString, QString> Track;
typedef QList<Track> TrackList;

enum CommandRole
{
    PlayPause,
    Play,
    Pause,
    Stop,
    Next,
    Previous,
    VolumeUp,
    VolumeDown,
    SeekAhead,
    SeekBack,
    PlayParticular
};

static const QString MprisPlayerPrefix("org.mpris.MediaPlayer2.");
static const QString DBusMprisPath("/org/mpris/MediaPlayer2");
static const QString PlayerInterface("org.mpris.MediaPlayer2.Player");
static const QString TracklistInterface("org.mpris.MediaPlayer2.TrackList");
static const QString PropertiesInterface("org.freedesktop.DBus.Properties");

static const QString titleMetadata("xesam:title");
static const QString trackIdMetadata("mpris:trackid");

static const double volumeChange = 0.05;
static const qlonglong seekOffset = 1000000; // 1 second

#endif // SIMON_MPRIS_CONSTANTS_H_9A22E03B797C494599B9629C6B47E478
