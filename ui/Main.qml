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
        anchors.bottom: parent.bottom //monchText.top
        anchors.margins: 8
        radius: 4
        color: Theme ? Theme.surface_container : "black"

        property rect boundRect: Monitors ? Monitors.bounds : Qt.rect(0, 0, 0, 0)
        property double xOffset: monitorsContainer.width / 2 - (boundRect.x + boundRect.width / 2) / 10
        property double yOffset: monitorsContainer.height / 2 - (boundRect.y + boundRect.height / 2) / 10
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
                
                width: (model.width / model.scale) / 10
                height: (model.height / model.scale) / 10
                x: (model.x / 10) + monitorsContainer.xOffset
                y: (model.y / 10) + monitorsContainer.yOffset

                color: Theme ? Theme.surface_container_lowest : "black"

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
                    }

                }
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
