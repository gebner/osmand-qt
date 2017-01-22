import QtQuick 2.2
import QtQuick.Controls 1.2

Item {
    id: root

    visible: true

    width: 800
    height: 600

    property variant resourcesWindow;
/*
    menuBar: MenuBar {
        Menu {
            title: qsTr("Map")

            MenuItem {
                text: qsTr("Downloaded resources...")
                shortcut: "Ctrl+D"
                onTriggered: {
                    var component = Qt.createComponent("Resources.qml");
                    resourcesWindow = component.createObject(root);
                    resourcesWindow.show()
                }
            }
        }
    }
    */
    Map {
        anchors.fill: parent
    }
}
