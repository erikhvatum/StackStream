import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import StackStream 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Layer {
        id : a_layer
        gamma : 0.51
        //tint : Qt.rgba(1,0.5,0.5,1)
        onTintChanged: messageDialog.show(qsTr("tint changed"), tint)
    }

    MainForm {
        anchors.fill: parent
        button1.onClicked: messageDialog.show(qsTr("tint is"), a_layer.tint)
        button2.onClicked: a_layer.tint = Qt.rgba(.1,.1,.1,.1)//messageDialog.show(qsTr("Button 2 pressed"))
    }

    MessageDialog {
        id: messageDialog

        function show(title, caption) {
            messageDialog.text = caption;
            messageDialog.title = title;
            messageDialog.open();
        }
    }
}

