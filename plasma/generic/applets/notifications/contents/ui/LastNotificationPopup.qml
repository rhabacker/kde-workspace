/***************************************************************************
 *   Copyright 2011 Davide Bettio <davide.bettio@kdemail.net>              *
 *   Copyright 2011 Marco Martin <mart@kde.org>                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.qtextracomponents 0.1
import org.kde.plasma.extras 0.1 as PlasmaExtras


PlasmaCore.Dialog {
    id: lastNotificationPopup

    function popup(icon, text)
    {
        lastNotificationText.text = text
        appIconItem.icon = icon

        var pos = lastNotificationPopup.popupPosition(notificationIcon, Qt.AlignCenter)
        lastNotificationPopup.x = pos.x
        lastNotificationPopup.y = pos.y
        lastNotificationPopup.visible = true
        lastNotificationTimer.running = true
    }

    location: plasmoid.location
    windowFlags: Qt.WindowStaysOnTopHint
    Component.onCompleted: {
        setAttribute(Qt.WA_X11NetWmWindowTypeDock, true)
    }

    mainItem: Item {
        width: theme.defaultFont.mSize.width * 30
        height: theme.defaultFont.mSize.width * 10

        Timer {
            id: lastNotificationTimer
            interval: 4000
            repeat: false
            running: false
            onTriggered: lastNotificationPopup.visible = false
        }

        QIconItem {
            id: appIconItem
            width: theme.largeIconSize
            height: theme.largeIconSize
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
        }
        PlasmaComponents.Label {
            id: lastNotificationText
            anchors {
                left: appIconItem.right
                right: parent.right
                top: parent.top
                bottom: parent.bottom
                leftMargin: 6
                rightMargin: 6
            }
            //textFormat: Text.PlainText
            verticalAlignment: Text.AlignVCenter
            color: theme.textColor
            wrapMode: Text.Wrap
            text: Text.ElideRight
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                lastNotificationPopup.visible = false
                lastNotificationTimer.running = false
            }
        }
    }
}
