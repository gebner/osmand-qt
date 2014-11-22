import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Window {
    ScrollView {
        anchors.fill: parent

        ListView {
            model: contextResources.resourcesInRepository
            delegate: RowLayout {
                Text { text: model.modelData.id }

                Button {
                    text: "Install"
                    visible: !model.modelData.installed
                    onClicked: model.modelData.install()
                }

                Button {
                    text: "Uninstall"
                    visible: model.modelData.installed
                }

                Button {
                    text: "Update"
                    visible: model.modelData.installed && !model.modelData.upToDate
                    onClicked: model.modelData.update()
                }
            }
        }
    }
}
