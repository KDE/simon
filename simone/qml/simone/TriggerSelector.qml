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
    id: triggerSelector

    property alias parameterName: cbContent.parameterName
    property bool onlyStaticCommands: false
    property alias triggerName: lbTrigger.text
    property int commandType: 0 //default index

    //use this to set the trigger from the outside;
    //the changes are not guaranteed to take affect
    //(e.g. if the command is not supported by the selected public server)
    property string selectedTrigger

    signal changed()

    width: parent.width
    height: cbContent.height

    Label {
        id: lbTrigger
        anchors.verticalCenter: cbContent.verticalCenter
    }


    EditableComboBox {
        id: cbContent
        editable: !onlyStaticCommands
        anchors.right: parent.right
        property string storedTrigger: ""

        onEditableChanged: {
            checkTrigger()
        }
        onChanged: {
            var newTrigger = cbContent.currentText()
            if (newTrigger != storedTrigger) {
                console.debug("Trigger changed; Old Trigger: " + storedTrigger + " - New Trigger: " + newTrigger);
                storedTrigger = newTrigger
                triggerSelector.changed()
            }
        }
    }

    onSelectedTriggerChanged: {
        cbContent.storedTrigger = selectedTrigger;
        if (selectedTrigger == cbContent.currentText())
            return;

        console.debug("Setting trigger: " + selectedTrigger)
        cbContent.setCurrentText(selectedTrigger)
        checkTrigger()
    }

    function getTrigger(trigger) {
        return cbContent.currentText();
    }

    function checkTrigger() {
        cbContent.comboBoxData = simoneView.getStaticCommandList()
        if (usePublicServer && !simoneView.commandSupported(cbContent.currentText())) {
            console.debug("Resetting because of unsupported command: " + cbContent.currentText());
            cbContent.setCurrentText(simoneView.getCommand(commandType))
        } else
            console.debug("Not resetting because of supported command: " + cbContent.currentText());
        console.debug("Done checking trigger")
    }



    Connections {
        target: simoneView
        onPublicCommandsChanged: {
            checkTrigger()
        }
    }
}
