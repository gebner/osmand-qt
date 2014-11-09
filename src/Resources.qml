import QtQuick 2.0
import QtQuick.Window 2.2

Window {
    ListView {
        model: contextResources.resourcesInRepository
        anchors.fill: parent
        delegate: Text {
            text: "10" + model.modelData.timestamp
        }
    }
}
