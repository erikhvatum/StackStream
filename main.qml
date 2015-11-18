import QtQuick 2.5
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: stackStream
    objectName: "stackStream"
    visible: true
    width: 1024
    height: 768
    title: qsTr("StackStream")

    property list<Layer> layerStack : [Layer{}]

    menuBar: MenuBar {
        id: foobar
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Image {
        source: "file:///Users/ehvatum/heic1015a.jpg"
        Drag.active: dragArea.drag.active
        Drag.hotSpot.x: 10
        Drag.hotSpot.y: 10

        MouseArea {
            id: dragArea
            anchors.fill: parent
            drag.target: parent
        }
    }
}
