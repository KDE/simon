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

import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: optionalItem
    property alias bodyItems: optionalItemContent.children
    property alias spacing: optionalItemContent.spacing
    property bool isShown: false

    visible: true
    width: parent.width
    height: (optionalItemContent.opacity > 0) ? optionalItemContent.height : 0

    function show() {
        optionalItemContent.opacity = 1
        optionalItemContent.anchors.topMargin = 10
        isShown = true
    }
    function hide() {
        optionalItemContent.opacity = 0
        optionalItemContent.anchors.topMargin = 150
        isShown = false
    }


    Column {
        id: optionalItemContent
        opacity: 0
        anchors.top: optionalItem.top
        anchors.topMargin: 150
        width: parent.width
        spacing: 10

        Behavior on anchors.topMargin {
            SpringAnimation{ spring: 2; damping: 0.2 }
        }
        Behavior on opacity {
            NumberAnimation {properties: "opacity"; duration: 200}
        }
    }
}
