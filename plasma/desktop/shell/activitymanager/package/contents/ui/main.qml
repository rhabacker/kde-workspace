/*
 *   Copyright 2011 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 1.1
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore

Item {
    id: main
    property int minimumWidth: 200
    property int minimumHeight: theme.defaultFont.mSize.height * 10

    signal addAppletRequested(string pluginName)
    signal closeRequested()
    property variant extraActions
    property variant getWidgetsActions

    PlasmaCore.Theme {
        id: theme
    }


    PlasmaCore.DataSource {
        id: activitySource
        engine: "org.kde.activities"
        onSourceAdded: {
            if (source != "Status") {
                connectSource(source)
            }
        }
        Component.onCompleted: {
            connectedSources = sources.filter(function(val) {
                return val != "Status";
            })
        }
    }

    PlasmaComponents.ContextMenu {
        id: newActivityMenu
        visualParent: newActivityButton
        PlasmaComponents.MenuItem {
            id: templatesItem
            text: i18n("Templates")
            onClicked: activityTemplatesMenu.open()
        }
        PlasmaComponents.MenuItem {
            icon: QIcon("user-desktop")
            text: i18n("Empty Desktop")
            onClicked: activityManager.createActivity("desktop")
        }
        PlasmaComponents.MenuItem {
            icon: QIcon("edit-copy")
            text: i18n("Clone current activity")
            onClicked: activityManager.cloneCurrentActivity()
        }
    }


    PlasmaComponents.ContextMenu {
        id: activityTemplatesMenu
        visualParent: templatesItem
    }
    Repeater {
        model: activityManager.activityTypeActions
        delegate: PlasmaComponents.MenuItem {
            icon: QIcon(modelData.icon)
            text: modelData.text
            separator: modelData.separator
            onClicked: {
                //is a plugin?
                if (modelData.pluginName) {
                    activityManager.createActivity(modelData.pluginName)
                //is a script?
                } else if (modelData.scriptFile) {
                    activityManager.createActivityFromScript(modelData.scriptFile,  modelData.text, modelData.icon, modelData.startupApps)
                //invoke ghns
                } else {
                    activityManager.downloadActivityScripts()
                }
            }
            Component.onCompleted: {
                parent = activityTemplatesMenu
            }
        }
    }


    Item {
        id: topBar
        anchors {
            top: parent.top
            left:parent.left
            right: parent.right

            margins: 4
        }
        height: filterField.height

        PlasmaComponents.TextField {
            id: filterField
            width: list.width / Math.floor(list.width / 180)
            clearButtonShown: true
            placeholderText: i18n("Enter search term...")
            Component.onCompleted: forceActiveFocus()
        }

        Row {
            anchors.right: parent.right
            spacing: 4
            PlasmaComponents.Button {
                id: newActivityButton
                iconSource: "list-add"
                text: i18n("Create activity...")
                onClicked: newActivityMenu.open()
            }
            PlasmaComponents.Button {
                iconSource: "plasma"
                text: i18n("Add widgets")
                onClicked: activityManager.addWidgetsRequested()
            }
            PlasmaComponents.ToolButton {
                iconSource: "window-close"
                onClicked: activityManager.closeClicked()
            }
        }
    }
    ListView {
        id: list
        anchors.top: topBar.bottom
        anchors.left:parent.left
        anchors.right: parent.right
        anchors.bottom: scrollBar.top
        clip: true
        orientation: ListView.Horizontal
        snapMode: ListView.SnapToItem
        model: PlasmaCore.SortFilterModel {
            sourceModel: PlasmaCore.DataModel {
                dataSource: activitySource
            }
            filterRole: "Name"
            filterRegExp: ".*"+filterField.text+".*"
        }

        delegate: ActivityDelegate {}
        header: Item {
            width: 4
            height: 4
        }
    }
    PlasmaComponents.ScrollBar {
        id: scrollBar
        orientation: Qt.Horizontal
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        flickableItem: list
    }
}
