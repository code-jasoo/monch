import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: root
    width: 900
    height: 600
    visible: true
    title: "monch"

    color: Theme ? Qt.alpha(Theme.surface, 1.0) : "black"

    Text {
        anchors.centerIn: parent
        text: ""
        color: "#cdd6f4"
        font.pixelSize: 18
    }

    Button {
      text: "click me"
      anchors.centerIn: parent
    }

}
