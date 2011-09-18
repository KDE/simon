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
        console.debug("Updating destination: "+index+ " - " + tsSelector.getTrigger() + tfDestination.text);
        destinationsModel.updateDestination(index, tsSelector.getTrigger(), tfDestination.text)
        console.debug("Updating done");
    }

    Column {
        id: columnContents
        width: parent.width
        spacing: 10

        TriggerSelector {
            id: tsSelector
            triggerName: qsTr("Trigger:")
            parameterName: "destination"+index
            onlyStaticCommands: usePublicServer
            commandType: (index % 9) + 5
            selectedTrigger: trigger
            onChanged: {
                console.debug("TriggerSelector: storing changes...")
                storeChanges()
            }
        }
        Item {
            height: tfDestination.height
            width: parent.width
            Label {
                anchors.verticalCenter: tfDestination.verticalCenter
                text: qsTr("Destination:")
            }
            TextField {
                id: tfDestination
                property string modelText : destination

                onModelTextChanged: {
                    text = destination
                }
                anchors.right: parent.right
                onTextChanged: {
                    if (text != modelText) {
                        console.debug("TextField: storing changes...")
                        storeChanges()
                    }
                }
            }
        }

        Button {
            anchors.right: parent.right
            text: "Remove"
            onClicked: {
                destinationsModel.removeContact(index);
            }
        }

        Rectangle { //separator
            height: 1
            width: parent.width
            color: "grey"
        }

    }
}
