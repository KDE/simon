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

Page {
    height: content.height
    width: content.width
    Column {
        id: content

        Label {
            id: lbHost
            text: qsTr("Host:");

        }
        TextField {
            id: tfHost
            objectName: "tfHost"
        }

        Label {
            id: lbPort
            text: qsTr("Port:");

        }
        TextField {
            id: tfPort
            objectName: "tfPort"
            validator: IntValidator {bottom: 1; top: 65536}
        }
        Label {
            id: lbUser
            text: qsTr("User:");

        }
        TextField {
            id: tfUser
            objectName: "tfUser"
        }
        Label {
            id: lbPassword
            text: qsTr("Password:");
        }
        TextField {
            id: tfPassword
            objectName: "tfPassword"
            echoMode: TextInput.Password
        }
    }
}
