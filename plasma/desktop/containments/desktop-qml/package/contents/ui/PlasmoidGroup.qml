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
 *   GNU Library General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.containments 0.1 as PlasmaContainments

import "plasmapackage:/code/LayoutManager.js" as LayoutManager

ItemGroup {
    id: plasmoidGroup
    //scale: plasmoid.scale
    //scale: 1.0
    canResizeHeight: true
    title: applet.name
    minimumWidth: Math.max(LayoutManager.cellSize.width,
                           appletContainer.minimumWidth +
                           plasmoidGroup.contents.anchors.leftMargin +
                           plasmoidGroup.contents.anchors.rightMargin)

    minimumHeight: Math.max(LayoutManager.cellSize.height,
                            appletContainer.minimumHeight +
                            plasmoidGroup.contents.anchors.topMargin +
                            plasmoidGroup.contents.anchors.bottomMargin)

    property alias applet: appletContainer.applet


    PlasmaContainments.AppletContainer {
        id: appletContainer
        anchors.fill: plasmoidGroup.contents
        onAppletChanged: {
            applet.appletDestroyed.connect(appletDestroyed)
            appletTimer.running = true
        }
        Behavior on opacity {
            NumberAnimation {
                duration: 250
                easing.type: Easing.InOutQuad
            }
        }
        function appletDestroyed() {
            LayoutManager.setSpaceAvailable(plasmoidGroup.x, plasmoidGroup.y, plasmoidGroup.width, plasmoidGroup.height, true)
            plasmoidGroup.destroy()
        }
    }
    Connections {
        target: plasmoid
        onImmutableChanged: {
            print("Applet backgroundhints: "  + applet.name + " : "  + hasBackground + " " + applet.backgroundHints + " " + plasmoidGroup.imagePath);
//             if (applet.backgroundHints != 0) {
//                 plasmoidGroup.imagePath = "widgets/background"
//             } else {
//                 plasmoidGroup.imagePath = "widgets/translucentbackground"
//             }
//             applet.backgroundHints = "NoBackground"
        }
    }


    //FIXME: this delay is because backgroundHints gets updated only after a while in qml applets
    Timer {
        id: appletTimer
        interval: 250
        repeat: false
        running: false
        onTriggered: {
            hasBackground = (applet.backgroundHints != 0);
            if (applet.backgroundHints == 1) {
                plasmoidGroup.imagePath = "widgets/background"
            } else if (applet.backgroundHints == 2) {
                plasmoidGroup.imagePath = "widgets/translucentbackground"
            }
            print(" @@@@@@@ Background: " + applet.name + " " + applet.backgroundHints + " " + hasBackground + " " + plasmoidGroup.imagePath);
            applet.backgroundHints = "NoBackground"
        }
    }
}