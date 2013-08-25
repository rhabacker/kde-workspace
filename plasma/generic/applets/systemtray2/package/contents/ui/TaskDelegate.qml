/***************************************************************************
 *   Copyright 2013 Sebastian Kügler <sebas@kde.org>                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

import QtQuick 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.kde.private.systemtray 2.0 as SystemTray
import "plasmapackage:/code/Layout.js" as Layout


Item {
    id: taskItemContainer
    objectName: "taskItemContainer"

    width: root.itemSize
    height: root.itemSize + (root.smallSpacing * 2)

    property int taskStatus: status
    property Item expandedItem: taskItemExpanded
    property alias icon: itemIcon

    Rectangle {
        anchors.fill: parent;
        border.width: 1;
        border.color: "violet";
        color: "pink";
        visible: root.debug;
        opacity: 0.5;
    }

    onExpandedItemChanged: {
        print("ST2P TaskDelegate Expanded changed ...");
        if (expandedItem != undefined) {
            print("ST2P Expanded defined ..." + expandedItem);
        } else {
            print("ST2P == undefined...");
        }
        if (expandedItem != null) {
            print("ST2P not null...");
        } else {
            print("ST2P == null...");
        }
//         if (root.expandedItem != expandedItem) {
//             root.expandedItem.visible = false;
//         }
        root.expandedItem = expandedItem;
    }

    PlasmaCore.IconItem {
        id: itemIcon
        width: parent.height
        height: parent.height
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
        }
        //visible: source != ""
        source: iconName != "" ? iconName : (typeof(icon) != "undefined" ? icon : "")
    }

    PulseAnimation {
        targetItem: taskItemContainer
        running: status == SystemTray.Task.NeedsAttention
    }
    /*
    onTaskStatusChanged: {
        print("ST2 status changed to " + taskStatusString());
    }

    // just for debugging purposes
    function taskStatusMnemonic() {
        if (status == SystemTray.Task.Passive) {
            return "--";
        } else if (status == SystemTray.Task.Active) {
            return "o";
        } else if (status == SystemTray.Task.NeedsAttention) {
            return "\o/";
        }
        return "??";
    }
    function taskStatusString() {
        if (status == SystemTray.Task.Passive) {
            return "Passive";
        } else if (status == SystemTray.Task.Active) {
            return "Active";
        } else if (status == SystemTray.Task.NeedsAttention) {
            return "NeedsAttention";
        }
        return "Unknown";
    }
    */

    MouseArea {
        anchors.fill: parent
        onClicked: {
            print("ST2B click ... " + !plasmoid.expanded + root.itemSize);
            plasmoid.expanded = !plasmoid.expanded;
            //root.expandedItem = taskItemExpanded;
            if (expandedItem != undefined && expandedItem != null) {
                root.expandedItem = expandedItem;
            } else {
                root.expandedItem = null;
            }
     }
    }
    onWidthChanged: updatePlasmoidGeometry()
    onHeightChanged: updatePlasmoidGeometry()

    function updatePlasmoidGeometry() {
        if (taskItem != undefined) {

            var _size = root.itemSize;
            //var _size = 32;
            var _m = (taskItemContainer.height - _size) / 2

            taskItem.anchors.verticalCenter = taskItemContainer.verticalCenter;
            taskItem.x = _m;
            taskItem.height = _size;
            taskItem.width = _size;
        }
    }
    Component.onCompleted: {
//         print("ST2P baseSize: " + root.baseSize);
        //print(" ST2 taskitem created: " + taskItem + " " + iconName);
        if (taskItem != undefined) {
            taskItem.parent = taskItemContainer;
            updatePlasmoidGeometry();
        }
    }
}