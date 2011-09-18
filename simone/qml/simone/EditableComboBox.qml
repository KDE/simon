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
    id: editableComboBox

    property bool editable: true
    property string parameterName: ""
    property alias comboBoxData: cbCombo.model

    signal changed()

    width: editable ? tfEdit.width : cbCombo.width
    height: editable ? tfEdit.height : cbCombo.height

    function currentText() {
        if (editable)
            return tfEdit.currentText
        return cbCombo.currentText
    }

    function setCurrentText(text) {
        if (editable)
            tfEdit.currentText = text
        else cbCombo.currentText = text
    }

    TextField {
        id: tfEdit
        property alias currentText: tfEdit.text
        objectName: editable  ? parameterName : ""
        visible: editable
        onCurrentTextChanged: {
            editableComboBox.changed()
        }
    }
    ComboBox {
        id: cbCombo
        objectName: !editable ? parameterName : ""
        visible: !editable

        onChanged: {
            editableComboBox.changed()
        }
    }

    onEditableChanged:  {
        if (editable) {
            tfEdit.text = cbCombo.currentText
        } else {
            cbCombo.currentText = tfEdit.text
        }
    }

}
