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

Item {
    id:combobox
    width: btSelect.width
    height: btSelect.height
    property alias currentText: btSelect.text
    property alias model: lvSelectValue.model

    signal changed()

    onCurrentTextChanged: {
        combobox.changed()
    }

    Button {
        id: btSelect
        onClicked: dlgSelectValue.open()
        MoreIndicator {
            anchors.right: btSelect.right
            anchors.rightMargin: 5
            anchors.verticalCenter: btSelect.verticalCenter
        }
    }


    Dialog {
        id: dlgSelectValue

        title: Label   {
            text: qsTr("Select value:")
            visible: true
            color: "white"
        }
        content: Item {
            width:parent.width
            height: 400

            anchors.bottomMargin: 15
            ListView {
                id: lvSelectValue
                width: parent.width
                height: parent.height
                visible: true
                clip:true
                //highlightFollowsCurrentItem: true
                spacing: 10

                delegate: Item {
                    property alias currentText: itemLabel.text
                    height: itemLabel.height + 10
                    width: parent.width
                    Label {
                        color: "white";
                        id: itemLabel
                        text: modelData
                        wrapMode: "WordWrap"
                        platformStyle: LabelStyle {
                            fontPixelSize: 45
                        }

                        width: parent.width - 10
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            lvSelectValue.currentIndex = index
                            btSelect.text = lvSelectValue.currentItem.currentText
                            dlgSelectValue.accept()
                        }
                    }
                }
                //highlight: Rectangle { color: "black"; radius: 5 }
                focus: true
            }
        }
        buttons: ButtonRow {
          style: ButtonStyle { }
          anchors.horizontalCenter: parent.horizontalCenter
          Button {text: qsTr("Cancel"); onClicked: dlgSelectValue.reject()}
        }
    }

}
