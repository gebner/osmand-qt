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

    function scrollStep() {
        return Math.round(0.2 * Math.pow(2, 31-zoom));
    }

    focus: true
    Keys.onPressed: {
        switch (event.key) {
        case Qt.Key_Equal: case Qt.Key_Plus: zoom += 1; break;
        case Qt.Key_Minus: zoom -= 1; break;

        case Qt.Key_Left: case Qt.Key_H: target31.x -= scrollStep(); break;
        case Qt.Key_Down: case Qt.Key_J: target31.y += scrollStep(); break;
        case Qt.Key_Up: case Qt.Key_K: target31.y -= scrollStep(); break;
        case Qt.Key_Right: case Qt.Key_L: target31.x += scrollStep(); break;
        }
    }
}
