import QtQuick 2.0
import OsmAndQt 1.0

MapCanvas {
    resources: contextResources

    MouseArea {
        anchors.fill: parent

        onWheel: {
            var targetAtMouse = parent.target31AtScreenPoint(Qt.point(wheel.x, wheel.y))
            parent.zoom += wheel.angleDelta.y > 0 ? +1 : -1;
            parent.setTarget31AtScreenPoint(targetAtMouse, Qt.point(wheel.x, wheel.y))
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
