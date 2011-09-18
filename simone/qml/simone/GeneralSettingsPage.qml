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
    id: generalSettings

    property alias usePublicServer: cbUsePublicServer.checked

    Flickable {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.topMargin: 10
        contentWidth: generalColumn.width
        contentHeight: generalColumn.height
        Column {
            id: generalColumn
            spacing: 10
            width: generalSettings.width - 10 // margins

            move: Transition {
                NumberAnimation {
                    properties: "y"
                    easing.type: Easing.OutBounce
                    duration: 300
                }
            }
            Item {
                width: parent.width - 20 // margin
                height: cbAutoConnect.height
                Label {
                    anchors.verticalCenter: cbAutoConnect.verticalCenter
                    text: qsTr("Connect on launch");
                }
                Switch {
                    id: cbAutoConnect
                    objectName: "cbAutoConnect"
                    anchors.right:parent.right
                }
            }

            Item {
                width: parent.width - 20
                height: cbUsePublicServer.height
                Label {
                    anchors.verticalCenter: cbUsePublicServer.verticalCenter
                    text: qsTr("Use public recognition server");
                }
                Switch {
                    id: cbUsePublicServer
                    objectName: "cbUsePublicServer"
                    anchors.right:parent.right
                }
            }
            ////////////////
            Item {
                height: settingsTabGroup.height
                width: parent.width
                TabGroup {
                    id: settingsTabGroup

                    currentTab: cbUsePublicServer.checked ? publicServer : privateServer
                    width: parent.width
                    height: currentTab.height

                    PublicServerConfiguration {
                        id: publicServer
                    }
                    PrivateServerConfiguration {
                        id: privateServer
                    }
                }
            }



            ////////////////

            Item {
                width: parent.width - 20 // margin
                height: lbVoiceActivityDetection.height
                Label {
                    id: lbVoiceActivityDetection
                    text: qsTr("Automatically detect speech\n(Instead of pressing button)");
                }
                Switch {
                    id: cbVoiceActivityDetection
                    anchors.verticalCenter: lbVoiceActivityDetection.verticalCenter
                    objectName: "cbVoiceActivityDetection"
                    anchors.right:parent.right

                    onCheckedChanged: {
                        if (checked)
                            vadSettings.show()
                        else
                            vadSettings.hide()
                    }
                }
            }

            OptionalItem {
                id: vadSettings
                spacing: 5
                bodyItems: [
                    Label {
                        id: lbCutoffLevel
                        text: qsTr("Cutoff level:");
                    },
                    Slider {
                        id: slCutoffLevel
                        objectName: "slCutoffLevel"
                        stepSize: 10
                        valueIndicatorVisible: true
                        minimumValue: 1
                        maximumValue: 15000
                    },
                    Label {
                        id: lbHeadMargin
                        text: qsTr("Head margin:");
                    },
                    Slider {
                        id: slHeadMargin
                        objectName: "slHeadMargin"
                        stepSize: 10
                        valueIndicatorVisible: true
                        minimumValue: 0
                        maximumValue: 2000
                    },
                    Label {
                        id: lbTailMargin
                        text: qsTr("Tail margin:");
                    },
                    Slider {
                        id: slTailmargin
                        objectName: "slTailMargin"
                        stepSize: 10
                        valueIndicatorVisible: true
                        minimumValue: 0
                        maximumValue: 2000
                    },
                    Label {
                        id: lbMinimumSampleLength
                        text: qsTr("Skip samples shorter than (ms):");
                    },
                    Slider {
                        id: slMinimumSampleLength
                        objectName: "slMinimumSampleLength"
                        stepSize: 10
                        valueIndicatorVisible: true
                        minimumValue: 0
                        maximumValue: 1000
                    }
                ]
            }
        }
    }
}
