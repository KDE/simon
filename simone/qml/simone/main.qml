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

PageStackWindow {
    id: appWindow
    property int pageMargin: 16
    showToolBar: !settingsSheet.visible

    initialPage: mainPage

    MainPage {
        id: mainPage
        objectName: "mainPage"
        tools: commonTools
        state:  "main"
    }

    SettingsSheet {
        id: settingsSheet
        visualParent: pageStack
    }


    ToolBarLayout {
        id: commonTools
        visible: true
        /*ToolIcon {
            platformIconId: "toolbar-back"
            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: {
                myMenu.close();
                pageStack.pop();
            }
            visible: pageStack.currentPage != mainPage
        }*/
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Settings")
                onClicked: {
                    myMenu.close()
                    settingsSheet.open() //pageStack.push(settingsPage);
                }
            }
        }
    }
}
