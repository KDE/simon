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

Sheet {
    id: settingsSheet

    acceptButtonText: "Save"
    rejectButtonText: "Cancel"

    content: PageStack {
        anchors.fill: parent
        visible: true
        toolBar: settingsToolBar

        ToolBar {
            id: settingsToolBar
            z: 5
            visible: true
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            tools: ToolBarLayout {
                ButtonRow {
                    TabButton {
                        id: tabButton1
                        tab: generalSettings
                        text: "General"
                    }
                    TabButton {
                        id: tabButton2
                        tab: actionSettings
                        text: "Actions"
                    }
                }
            }
        }

        TabGroup {
            id: settingsTabGroup

            currentTab: generalSettings
            width: parent.width
            anchors.top: settingsToolBar.bottom
            anchors.bottom: parent.bottom


            GeneralSettingsPage {
                id: generalSettings
            }
            ActionsSettingsPage {
                id: actionSettings
                usePublicServer: generalSettings.usePublicServer
            }
        }
    }
    onAccepted: simoneView.storeConfiguration();
    onRejected: simoneView.restoreConfiguration();
}
