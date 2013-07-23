/*
 * Copyright 2013  Heena Mahour <heena393@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as Components
import org.kde.locale 0.1

Item {
    id:main
    property int minimumWidth:formFactor == Horizontal ? height : 1
    property int minimumHeight:formFactor == Vertical ? width  : 1
    property int formFactor: plasmoid.formFactor
    property bool constrained:formFactor==Vertical||formFactor==Horizontal
    
    PlasmaCore.DataSource {
        id: dataSource
        engine: "time"
        connectedSources: ["Local"]
        interval: 500
    }
   
    Locale {
        id: locale
    }
    
    Rectangle {
        id:currentApp
        width:main.width
        height:width
        anchors.centerIn:parent
        color:"transparent"
        Components.Label  {
            id: time
            font.pointSize:main.width/8
            width: Math.max(paintedWidth,time.paintedWidth)
            text :Qt.formatTime( dataSource.data["Local"]["Time"],"hh:mm:ss AP" );
            horizontalAlignment:main.AlignHCenter
            anchors {
                left:parent.left
                leftMargin:5
                top:parent.top
                bottom:parent.bottom
            }
        }
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: plasmoid.togglePopup()
            PlasmaCore.ToolTip {
                id: tooltip
                target: mouseArea
                mainText:"Current Time"
                subText: Qt.formatDate( dataSource.data["Local"]["Date"],"dddd, MMM d yyyy" )+"\n"+Qt.formatTime( dataSource.data["Local"]["Time"],"hh:mm:ss AP" )
                image:"preferences-system-time"
            }
        }
    }
}