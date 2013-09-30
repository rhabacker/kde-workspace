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

import org.kde.private.systemtray 2.0 as SystemTray

Item {
    id: root

//     width: 256
//     height: 32
//     anchors {
//         left: parent.left
//         right: parent.right
//         verticalCenter: parent.verticalCenter
//     }

    property int _h: 32

    SystemTray.Host {
        id: host
    }

    ListView {
        id: gridView

        anchors {
            left: parent.left
            right: parent.right
            //verticalCenter: parent.verticalCenter
        }

        orientation: Qt.Horizontal
        interactive: false
        spacing: 4
        //Rectangle { anchors.fill: parent; color: "blue"; opacity: 0.2; }

        model: host.tasks

        delegate: Component {
            Item {
                width: _h
                height: _h
                //Rectangle { anchors.fill: parent; color: "orange"; opacity: 0.4; }
                PlasmaComponents.Label {
                    anchors.fill: parent
                    //text: "task"
                }
                PlasmaCore.IconItem {
                    anchors.fill: parent
                    source: iconName != "" ? iconName : icon
                }
                Component.onCompleted: {
                    //print("YAY ST Created item");
                }
            }

        }

        //delegate: StatusNotifierItem {}
    }

    PlasmaComponents.Label {
        anchors { bottom: parent.bottom; right: parent.right }
        text: "Items: "
    }

    /*
    Flow {
        spacing: 4
        anchors.fill: parent

        PlasmaCore.IconItem {
            source: "configure"
            width: _h
            height: width
        }
        PlasmaCore.IconItem {
            source: "dialog-ok"
            width: _h
            height: width
        }
        PlasmaCore.IconItem {
            source: "resize-tr2bl"
            width: _h
            height: width
        }
        PlasmaCore.IconItem {
            source: "akonadi"
            width: _h
            height: width
        }
        PlasmaCore.IconItem {
            source: "clock"
            width: _h
            height: width
        }
    }
    */

}