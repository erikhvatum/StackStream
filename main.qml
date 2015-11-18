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

    DraggableImage {
        id: imageA
        source: "file:///Users/ehvatum/heic1015a.jpg"
        otherImage: imageB
        //mouseArea.onClicked: print(flaf)
        flaf: 15
        width: imageB.x
        height: imageB.y
    }

    DraggableImage {
        id: imageB
        source: "file:///Users/ehvatum/m1_hubble.jpg"
        otherImage: imageA
    }
}
