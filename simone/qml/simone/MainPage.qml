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
import com.nokia.extras 1.0

Page {
    id: page
    Connections {
        target: platformWindow
        onActiveChanged: viewModeChanged()
    }

    anchors.margins: appWindow.pageMargin

    states: [
        State {
            name: "main"
            AnchorChanges {
                target: recognitionControlElements;
                anchors.verticalCenter: page.verticalCenter
            }
            PropertyChanges {
                target: recognitionControlElements;
                anchors.topMargin: 0
            }
            PropertyChanges {
                target: subMenuBody;
                opacity: 0
            }

        },
        State {
            name: "subMenu"

            AnchorChanges {
                target: recognitionControlElements;
                anchors.top: pbVUMeter.bottom
            }
            PropertyChanges {
                target: recognitionControlElements;
                anchors.topMargin: 10
            }
            PropertyChanges {
                target: subMenuBody;
                opacity: 1
            }
        }
    ]
    function viewModeChanged() {
        console.debug("Changed view mode...")
        if (platformWindow.active)
            simoneView.resumeUpdates()
        else
            simoneView.pauseUpdates()
    }

    transitions: Transition {
        AnchorAnimation { duration: 300 }
        NumberAnimation { properties: "topMargin"; easing.type: Easing.InOutQuad }
        NumberAnimation { properties: "leftMargin"; easing.type: Easing.InOutQuad }
    }

    InfoBanner {
        id: recognitionResultBanner
        objectName: "recognitionResultBanner"
        width: pbVUMeter.width
        z: 2
        anchors {
            top: pbVUMeter.bottom
            topMargin: 10
        }
    }

    Item {
        id: subMenuBody
        width: parent.width
        anchors.top: recognitionControlElements.bottom
        anchors.topMargin: 5
        //height: page.height - top - btConnect.top - 20

        anchors.left: page.left
        anchors.bottom: btConnect.top
        anchors.bottomMargin: 10
        opacity: 0

        Button {
            id: btBackToMain
            objectName: "btBackToMain"
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Back")
        }

        Rectangle {
            width: parent.width
            radius: 15
            anchors {
                top: btBackToMain.bottom
                bottom: parent.bottom
                topMargin: 10
            }

            gradient: Gradient {
                GradientStop { position: 0.0; color: "#ebebeb" }
                GradientStop { position: 1.0; color: "#b8b8b8" }
            }
            border.color: "gray"
            border.width: 1
            ListView {
                id: lvCommandsView
                objectName: "lvCommandsView"
                anchors.fill: parent
                clip: true;
                model: contactsModel
                delegate: CommandDelegate {}
            }
        }
    }

    Item {
        id: recognitionControlElements
        height: childrenRect.height
        width: parent.width
        anchors.horizontalCenter: page.horizontalCenter

        Button{
            id: btSpeak
            anchors.horizontalCenter: parent.horizontalCenter
            objectName: "btSpeak"
            text: pressed ? qsTr("Release to recognize") : qsTr("Hold to speak")
            visible: false
            onPressedChanged: {
                if (pressed)
                    simoneView.startRecordingRequested();
                else
                    simoneView.commitRecordingRequested();
            }
        }

        Label{
            id: lbSpeak
            anchors.horizontalCenter: parent.horizontalCenter
            objectName: "lbSpeak"
            text: qsTr("Please speak")
            visible: false
        }
        BusyIndicator {
            id: busyIndicator
            anchors.horizontalCenter: parent.horizontalCenter
            objectName: "busyIndicator"
            platformStyle: BusyIndicatorStyle { size: "medium" }
            running: true
            visible: false
        }
    }


    // constant ui
    ProgressBar {
        id: pbVUMeter
        objectName: "pbVUMeter"
        width: parent.width - 20
        anchors.top: parent.top
        anchors.topMargin: 5
    }

    Button{
        id: btConnect
        objectName: "btConnect"
        anchors {
            bottom: lbStatus.top
            bottomMargin: 30
            horizontalCenter: parent.horizontalCenter
        }
        text: qsTr("Connect to recognition")
    }

    Label {
        id: lbStatus
        objectName: "lbStatus"
        anchors.bottom: parent.bottom
        text: qsTr("Idle.")
    }

    Dialog {
        id: errorDialog
        objectName: "errorDialog"
        title: Label   {
            text: qsTr("Error")
        }
        content: Item {
            width:parent.width
            height: lbError.height
            anchors.bottomMargin: 15
            Label {
                id: lbError
                width: parent.width
                objectName: "lbError"
                color:"white";
                wrapMode: "WordWrap"
            }
        }
        buttons: ButtonRow {
          style: ButtonStyle { }
          anchors.horizontalCenter: parent.horizontalCenter
          Button {text: qsTr("OK"); onClicked: errorDialog.accept()}
        }
    }
}
