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

import QtQuick 1.0
import com.nokia.meego 1.0

Rectangle {
    property alias model: listView.model
    property alias delegate: listView.delegate
    width: parent.width
    height: 230
    radius: 15

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#ebebeb" }
        GradientStop { position: 1.0; color: "#b8b8b8" }
    }
    border.color: "gray"
    border.width: 1
    ListView {
        id: listView
        anchors.fill: parent
        clip: true;
    }
}
