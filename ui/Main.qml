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
        anchors.bottom: monchText.top
        anchors.margins: 8
        radius: 4
        color: Theme ? Theme.surface_container : "black"
        
        Button {
            anchors.centerIn: parent
            text: "click me"
            onClicked: {
                Monitors.reloadMonitors();
            }
        }

        Repeater {
            model: Monitors

            delegate: Rectangle {
                width: (model.width / model.scale) / 10
                height: (model.height / model.scale) / 10
                x: model.x / 10
                y: model.y / 10

                color: Theme ? Theme.surface_container_low : "black"

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
                }
            }
        }
    }
    
    

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
}
