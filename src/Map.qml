import QtQuick 2.0
import OsmAndQt 1.0

MapCanvas {
    resources: contextResources

    MouseArea {
        anchors.fill: parent

        onWheel: {
            if (wheel.angleDelta.y > 0) {
                parent.zoom += 1
            } else {
                parent.zoom -= 1
            }
        }

        property bool currentlyDragging: false
        property point dragPoint

        onPressed: {
            currentlyDragging = true
            dragPoint = parent.target31AtScreenPoint(Qt.point(mouseX, mouseY))
        }
        onPositionChanged: {
            if (currentlyDragging) {
                parent.setTarget31AtScreenPoint(dragPoint, Qt.point(mouseX, mouseY))
            }
        }
        onReleased: currentlyDragging = false
    }

    focus: true
    Keys.onDownPressed: target31.y += Math.round(Math.pow(2, 31-zoom))
}
