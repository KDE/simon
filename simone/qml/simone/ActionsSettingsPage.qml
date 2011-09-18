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
    id: actionSettings
    property bool usePublicServer: false

    function updateLanguage() {
        console.debug("Updating language selection...");
        cbContactAcceptCall.comboBoxData = cbContactTrigger.comboBoxData = simoneView.getStaticCommandList();
        console.debug("Done...");
    }

    function showBackButton() {
        if (contactsSettings.isShown || navigationSettings.isShown) {
            if (!backButtonSettings.isShown)
                backButtonSettings.show()
        } else {
            if (backButtonSettings.isShown)
                backButtonSettings.hide()
        }
    }

    Flickable {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.topMargin: 10
        contentWidth: actionColumns.width
        contentHeight: actionColumns.height
        Column {
            id: actionColumns
            spacing: 10
            width: settingsSheet.width


            Item {
                width: parent.width - 20 // margin
                height: cbProcessActions.height
                Label {
                    anchors.verticalCenter: cbProcessActions.verticalCenter
                    text: qsTr("Voice control of the device");
                }
                Switch {
                    id: cbProcessActions
                    objectName: "cbProcessActions"
                    anchors.right:parent.right
                }
            }

            OptionalItem {
                id: backButtonSettings
                width: parent.width - 20

                bodyItems: [
                    TriggerSelector {
                        id: cbBackButton
                        triggerName: qsTr("Back to main:")
                        parameterName: "cbBackToMain"
                        onlyStaticCommands: usePublicServer
                        commandType: 4
                    }
                ]
            }

            CheckBox {
                id: cbContacts
                objectName: "cbContacts"
                text: "Contacts"
                onCheckedChanged: {
                    if (checked) {
                        contactsSettings.show()
                    } else
                        contactsSettings.hide()
                    showBackButton();
                }
            }

            OptionalItem {
                id: contactsSettings
                width: parent.width - 20

                bodyItems: [
                    TriggerSelector {
                        id: cbContactTrigger
                        triggerName: qsTr("Dialing:")
                        parameterName: "cbContactTrigger"
                        onlyStaticCommands: usePublicServer
                        commandType: 0
                    },
                    TriggerSelector {
                        id: cbContactAcceptCall
                        triggerName: qsTr("Accept call:")
                        parameterName: "cbContactAcceptCall"
                        onlyStaticCommands: usePublicServer
                        commandType: 1
                    },
                    Button {
                        text: "Add contact"
                        width: parent.width

                        onClicked: {
                            contactsModel.addContact()
                        }
                    },
                    CommandListView {
                        model: contactsModel
                        delegate: ContactsDelegate {}

                    }

                ]

            }

            Item {
                height: 5
                width: parent.width
            }

            //----------- End contacts



            CheckBox {
                id: cbNavigation
                objectName: "cbNavigation"
                text: "Navigation"
                onCheckedChanged: {
                    if (checked) {
                        navigationSettings.show()
                    } else
                        navigationSettings.hide()
                    showBackButton();
                }
            }

            OptionalItem {
                id: navigationSettings
                width: parent.width - 20

                bodyItems: [
                    TriggerSelector {
                        id: cbNavigationTrigger
                        triggerName: qsTr("Pick destination:")
                        parameterName: "cbNavigationTrigger"
                        onlyStaticCommands: usePublicServer
                        commandType: 2
                    },
                    TriggerSelector {
                        id: cbStopNavigation
                        triggerName: qsTr("Stop navigation:")
                        parameterName: "cbStopNavigation"
                        onlyStaticCommands: usePublicServer
                        commandType: 3
                    },
                    Button {
                        text: "Add destination"
                        width: parent.width

                        onClicked: {
                            destinationsModel.addDestination()
                        }
                    },
                    CommandListView {
                        model: destinationsModel
                        delegate: DestinationDelegate {}

                    }

                ]

            }
            // end navigation
        }
    }
}
