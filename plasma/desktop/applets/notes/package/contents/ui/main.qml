/*
    Copyright (C) 2014 David Edmundson <davidedmundson@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

import QtQuick 2.1
import QtQuick.Layouts 1.1

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.plasmoid 2.0

import org.kde.plasma.private.notes 0.1 as Notes

PlasmaCore.SvgItem
{
    svg: PlasmaCore.Svg {imagePath: "widgets/notes"}
    Layout.minimumWidth: 300
    Layout.minimumHeight: 300

    Plasmoid.backgroundHints: PlasmaCore.Types.NoBackground

    //deliberately not PlasmaComponents.textEdit
    //as we have custom font selection
    TextEdit {
        id: mainTextArea
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: fontButtons.top
            margins: 30
        }
        text: documentHandler.text
        focus: true
        textFormat: Qt.RichText
        wrapMode: TextEdit.Wrap
    }

    Notes.DocumentHandler {
        id: documentHandler
        target: mainTextArea
        cursorPosition: mainTextArea.cursorPosition
        selectionStart: mainTextArea.selectionStart
        selectionEnd: mainTextArea.selectionEnd
    }

    Row {
        id: fontButtons
        spacing: 4
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 30
        }
        PlasmaComponents.ToolButton {
            id: toggleFormatBarButton
            iconSource: "draw-text"
            checkable: true
        }

        PlasmaComponents.ToolButton {
            id: formatBoldButton
            iconSource: "format-text-bold"
            visible: toggleFormatBarButton.checked
            checkable: true
            checked: documentHandler.bold
            onClicked: documentHandler.bold = !documentHandler.bold

        }
        PlasmaComponents.ToolButton {
            iconSource: "format-text-italic"
            visible: toggleFormatBarButton.checked
            checkable: true
            checked: documentHandler.italic
            onClicked: documentHandler.italic = !documentHandler.italic
        }
        PlasmaComponents.ToolButton {
            iconSource: "format-text-underline"
            visible: toggleFormatBarButton.checked
            checkable: true
            checked: documentHandler.underline
            onClicked: documentHandler.underline = !documentHandler.underline
        }
        PlasmaComponents.ToolButton {
            iconSource: "format-text-strikethrough"
            visible: toggleFormatBarButton.checked
            checkable: true
            checked: documentHandler.strikeOut
            onClicked: documentHandler.strikeOut = !documentHandler.strikeOut
        }
        PlasmaComponents.ToolButton {
            iconSource: "format-justify-center"
            visible: toggleFormatBarButton.checked
            checkable: true

        }
        PlasmaComponents.ToolButton {
            iconSource: "format-justify-fill"
            visible: toggleFormatBarButton.checked
            checkable: true
        }
    }

}
