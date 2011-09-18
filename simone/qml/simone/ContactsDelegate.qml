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
import com.nokia.extras 1.0
import QtMobility.contacts 1.1

Item {
    property alias onlyStaticCommands: tsSelector.onlyStaticCommands
    property string displayName: display
    width: parent.width
    height: columnContents.height

    function storeChanges() {
        console.debug("Updating contact: "+index+ " - " + tsSelector.getTrigger() + tfContact.text);
        contactsModel.updateContact(index, tsSelector.getTrigger(), displayName, tfContact.text)
        console.debug("Updating done");
    }

    Column {
        id: columnContents
        width: parent.width
        spacing: 10

        TriggerSelector {
            id: tsSelector
            triggerName: qsTr("Trigger:")
            parameterName: "contact"+index
            onlyStaticCommands: usePublicServer
            commandType: (index % 9) + 5
            selectedTrigger: trigger
            onChanged: {
                console.debug("TriggerSelector: storing changes...")
                storeChanges()
            }
        }
        Item {
            height: tfContact.height
            width: parent.width
            Label {
                id: lbContact
                anchors.verticalCenter: tfContact.verticalCenter
                text: qsTr("Contact:")
            }
            Button {
                id: tfContact
                property string modelText : number

                onModelTextChanged: {
                    text = modelText
                }

                onClicked: {
                    console.debug("Selecting from contacts...")
                    dlgSelectContact.open()
                }

                anchors.right: parent.right

                Connections {
                    target: dlgSelectContact
                    onAccepted: {
                        if (lvSelectValue.currentItem) {
                            displayName = lvSelectValue.currentItem.currentTextName;
                            tfContact.text = lvSelectValue.currentItem.currentText;
                            console.debug("Number: storing changes...")
                            storeChanges()
                        }
                    }
                }
            }
        }

        Button {
            anchors.right: parent.right
            //width: parent.width/2 - 5
            text: "Remove"
            onClicked: {
                contactsModel.removeContact(index);
            }
        }

        Rectangle { //separator
            height: 1
            width: parent.width
            color: "grey"
        }

    }
    Dialog {
        id: dlgSelectContact

        title: Label   {
            text: qsTr("Select contact:")
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
                spacing: 10

                model: ContactModel {
                    sortOrders: [
                        SortOrder {
                            detail:ContactDetail.Name
                            field:Name.FirstName
                            direction:Qt.AscendingOrder
                        },
                        SortOrder {
                           detail:ContactDetail.Name
                           field:Name.LastName
                           direction:Qt.AscendingOrder
                        }
                     ]
                }

                delegate: Item {
                    property alias currentText: itemLabel.text
                    property alias currentTextName: itemLabelName.text
                    height: detail.height + 10
                    width: parent.width
                    Column {
                        id: detail
                        Label {
                            color: "white";
                            text: display
                            id: itemLabelName
                            wrapMode: "WordWrap"
                            width: parent.width - 10
                            platformStyle: LabelStyle {
                                fontPixelSize: 45
                            }
                        }
                        Label {
                            color: "white";
                            id: itemLabel
                            text: contact.phoneNumber.number
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            lvSelectValue.currentIndex = index
                            console.debug("Setting contact to the one selected")
                            dlgSelectContact.accept()
                        }
                    }
                }
                focus: true
            }
        }
        buttons: ButtonRow {
          style: ButtonStyle { }
          anchors.horizontalCenter: parent.horizontalCenter
          Button {text: qsTr("Cancel"); onClicked: dlgSelectContact.reject()}
        }
    }
}
