import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: root
    width: 900
    height: 600
    visible: true
    title: "monch"

    color: Theme ? Qt.alpha(Theme.surface, 1.0) : "black"

    Rectangle {
        id: monitorsContainer
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: settingsField.top //parent.bottom //monchText.top
        anchors.margins: 8
        radius: 4
        color: Theme ? Theme.surface_container : "black"

        property double globalScale: 5

        property rect boundRect: Monitors ? Monitors.bounds : Qt.rect(0, 0, 0, 0)
        property double xOffset: monitorsContainer.width / 2 - (boundRect.x + boundRect.width / 2) / globalScale
        property double yOffset: monitorsContainer.height / 2 - (boundRect.y + boundRect.height / 2) / globalScale
        
        property var selected: null
        property var originMonitor: null        // actual model data
        property var originMonitorRect: null    // the representing visual rect

        Button {
            anchors.centerIn: parent
            text: "click me"
            onClicked: {
                Monitors.reloadMonitors();
            }
        }

        Repeater {
            model: Monitors
            id: monitorsRepeater

                        
            function checkOverlap(rect1, rect2) {
                // AABB collision checking
                return  (rect1.x < rect2.x + rect2.width) &&
                (rect1.x + rect1.width > rect2.x) &&
                (rect1.y < rect2.y + rect2.height) &&
                (rect1.y + rect1.height > rect2.y)
            }

            function getDistance(pt1, pt2) {
                var dx = pt2.x - pt1.x
                var dy = pt2.y - pt1.y

                return dx * dx + dy * dy
            }

            delegate: Rectangle {

                id: rectDelegate

                property int scale: model.scale
                property string output: model.output

                property int futureX: 0
                property int futureY: 0
                property int futureScale: model.scale
                property int actualWidth: model.width
                property int actualHeight: model.height
                
                width: (model.width / futureScale) / monitorsContainer.globalScale
                height: (model.height / futureScale) / monitorsContainer.globalScale
                x: (model.x / monitorsContainer.globalScale) + monitorsContainer.xOffset
                y: (model.y / monitorsContainer.globalScale) + monitorsContainer.yOffset

                

                color: Theme ? Theme.surface_container_lowest : "black"

                Component.onCompleted: {
                    if (model.x == 0 && model.y == 0) {
                        monitorsContainer.originMonitor = model
                        monitorsContainer.originMonitorRect = rectDelegate
                    }

                    futureX = model.x
                    futureY = model.y
                    futureScale = model.scale
                }

                Behavior on color {
                    ColorAnimation {
                        duration: 150
                        easing.type: Easing.InOutQuad
                    }
                }

                function getSnapPoints() {
                    return [
                        Qt.point(x, y),                 // Upper Left
                        Qt.point(x, y + height),            // Bottom Left
                        Qt.point(x + width, y),             // Upper Right
                        Qt.point(x + width, y + height),        // Bottom Right
                        Qt.point(x + width / 2, y),         // Top Center
                        Qt.point(x + width / 2, y + height),    // Bottom Center
                        Qt.point(x, y + height / 2),        // Left Center
                        Qt.point(x + width, y + height / 2)     // Right Center
                    ]
                }

                Text {
                    anchors.centerIn: parent
                    font.family: "Varela Round"
                    font.pixelSize: 12
                    text: model.output
                    color: Theme ? Theme.on_surface : "black"
                }

                MouseArea {
                    anchors.fill: parent
                    drag.target: parent
                    drag.minimumX: 0
                    drag.minimumY: 0
                    drag.maximumX: monitorsContainer.width - parent.width
                    drag.maximumY: monitorsContainer.height - parent.height

                    onClicked: {
                        monitorsContainer.selected = rectDelegate
                        parent.color = Theme ? Theme.surface_container_high : "black"
                        for (var i = 0; i < monitorsRepeater.count; i++) {
                            var mon = monitorsRepeater.itemAt(i)
                            if (parent == mon) { continue; }
                            mon.color = Theme ? Theme.surface_container_lowest : "black"
                        }
                    }

                    onPositionChanged: {
                        // Overlap checking and position snapping
                        // x==x==x
                        // |     |
                        // x     x
                        // |     |
                        // x==x==x  
                        
                        for (var i = 0; i < monitorsRepeater.count; i++) {
                            var child = monitorsRepeater.itemAt(i);
                            if (i == index) { continue; }

                            // Check if any of my snap points are near any other monitors snap points
                            // connect up if so
                            
                            for (var j = 0; j < getSnapPoints().length; j++) {
                                for (var k = 0; k < child.getSnapPoints().length; k++) {
                                    if (monitorsRepeater.getDistance(getSnapPoints()[j], child.getSnapPoints()[k]) < 50) {
                                        var dx = getSnapPoints()[j].x - child.getSnapPoints()[k].x
                                        var dy = getSnapPoints()[j].y - child.getSnapPoints()[k].y
                                        
                                        parent.x -= dx
                                        parent.y -= dy

                                        //console.log(parent.x + "," + parent.y + " -- " + parent.width + "," + parent.height)
                                        //console.log(child.x + "," + child.y + " -- " + child.width + "," + child.height)
                                        //console.log("")
                                    }
                                }
                            }

                            if (monitorsRepeater.checkOverlap(parent, child)) {
                                parent.border.width = 1
                                child.border.width = 1
                                parent.border.color = Theme ? Theme.error : "red"
                                child.border.color = Theme ? Theme.error : "red"
                            } else {
                                parent.border.width = 0
                                child.border.width = 0
                                parent.border.color = Theme ? Theme.surface_container_low : "black"
                                child.border.color = Theme ? Theme.surface_container_low : "black"
                            }

                            
                        }
                        
                        // debug out the actual positions in the virtual space
                        var originMon = monitorsContainer.originMonitorRect
                        var x = (rectDelegate.x * monitorsContainer.globalScale) - (originMon.x * monitorsContainer.globalScale)
                        var y = (rectDelegate.y * monitorsContainer.globalScale) - (originMon.y * monitorsContainer.globalScale) 

                        rectDelegate.futureX = x
                        rectDelegate.futureY = y

                        console.log("(" + futureX + "," + futureY + ")")
                    }

                }
            }
        }
    }

    Row {
        id: settingsField
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
        height: 30
        z: 100
        
        Text {
            id: xText
            text: "x: "
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            font.family: "Varela Round"
            font.pixelSize: 12
            color: Theme ? Theme.on_surface : "black"
        }

        TextField {
            id: xBox
            anchors.left: xText.right
            anchors.verticalCenter: scaleText.verticalCenter
            text: monitorsContainer.selected ? monitorsContainer.selected.futureX : 0

            implicitWidth: 100
            implicitHeight: 20
        }

        Text {
            id: yText
            text: "y: "
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: xBox.right
            font.family: "Varela Round"
            font.pixelSize: 12
            color: Theme ? Theme.on_surface : "black"
        }

        TextField {
            id: yBox
            anchors.left: yText.right
            anchors.verticalCenter: yText.verticalCenter
            text: monitorsContainer.selected ? monitorsContainer.selected.futureY : 0

            implicitWidth: 100
            implicitHeight: 20
        }
        
        Text {
            id: scaleText
            text: "Scale: "
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: yBox.right
            font.family: "Varela Round"
            font.pixelSize: 12
            color: Theme ? Theme.on_surface : "black"
        }

        SpinBox {
            id: scaleBox
            anchors.left: scaleText.right
            anchors.verticalCenter: scaleText.verticalCenter
            value: monitorsContainer.selected ? monitorsContainer.selected.futureScale : 0
            from: 0
            to: 4

            onValueModified: {
                if (monitorsContainer.selected) {
                    monitorsContainer.selected.futureScale = value
                }
            }

            implicitWidth: 40
            implicitHeight: 20
        }

        Text {
            id: originText
            text: "Origin: "
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: scaleBox.right
            font.family: "Varela Round"
            font.pixelSize: 12
            color: Theme ? Theme.on_surface : "black"
        }

        CheckBox {
            id: originChkBox
            anchors.left: originText.right
            anchors.verticalCenter: originText.verticalCenter
            checked: monitorsContainer.selected ? (monitorsContainer.selected.futureX == 0 && monitorsContainer.selected.futureY == 0) : false
        }

        Button {
            // TODO clean up ui and fix visual glitches
            // TODO also maybe refactor ui code because this jawn is fried
            id: applyBtn
            anchors.left: originChkBox.right
            anchors.verticalCenter: parent.verticalCenter
            text: "Apply"

            onClicked: {
                console.log("Applying display settings")
                for (var i = 0; i < monitorsRepeater.count; i++) {
                    var mon = monitorsRepeater.itemAt(i);
                    Monitors.queueMonitor(mon.output, mon.futureX, mon.futureY, mon.actualWidth, mon.actualHeight, mon.futureScale);
                } 
                Monitors.writeMonitors();
            }
        }
    }

    
    
    /*
    Text {
        id: monchText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: versionText.top
        anchors.bottomMargin: 4
        font.family: "Varela Round"
        font.pixelSize: 18
        text: "monch"
        color: Theme ? Theme.on_surface : "black"
    }

    Text {
        id: versionText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        font.family: "Varela Round"
        font.pixelSize: 8
        text: "v1.0"
        color: Theme ? Theme.on_surface : "black"
    }
    */
}
