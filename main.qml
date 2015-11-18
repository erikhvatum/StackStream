import QtQuick 2.5
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import StackStream 1.0

ApplicationWindow {
    id: stackStream
    objectName: "stackStream"
    visible: true
    width: 1024
    height: 768
    title: qsTr("StackStream")

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

    Item {
        id: poop
        objectName: "plop"
        property int faff: 11
    }

    onClosing:{
        print(poop.objectName)
    }

    /*Slayer {
        id : biff
        objectName: "slaerfoo"
        property int faff : 5
    }*/

    /*Layer {
        id : a_layer
        gamma : 0.51
        //tint : Qt.rgba(1,0.5,0.5,1)
        onTintChanged: messageDialog.show(qsTr("tint changed"), tint)
    }*/

    /*MainForm {
        anchors.fill: parent
        button1.onClicked: messageDialog.show(qsTr("tint is"), a_layer.tint)
        button2.onClicked: a_layer.tint = Qt.rgba(.1,.1,.1,.1)//messageDialog.show(qsTr("Button 2 pressed"))
        resetButton.onClicked: a_layer.tint = undefined
    }*/

    /*Component {
        id: editableDelegate
        Item {

            Text {
                width: parent.width
                anchors.margins: 4
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                elide: styleData.elideMode
                text: styleData.value !== undefined ? styleData.value : ""
                color: styleData.textColor
                visible: !styleData.selected
            }
            Loader {
                id: loaderEditor
                anchors.fill: parent
                anchors.margins: 4
                Connections {
                    target: loaderEditor.item
                    onAccepted: {
                        if (typeof styleData.value === 'number')
                        {
                            print(styleData.role)
                            test_model.setProperty(styleData.row, Number(parseFloat(loaderEditor.item.text).toFixed(0)), styleData.role);
                        }
                        else
                            test_model.setProperty(styleData.row, loaderEditor.item.text, styleData.role);
                    }
                }
                sourceComponent: styleData.selected ? editor : null
                Component {
                    id: editor
                    TextInput {
                        id: textinput
                        color: styleData.textColor
                        text: styleData.value
                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: textinput.forceActiveFocus()
                        }
                    }
                }
            }
        }
    }

    TableView {
        //anchors.fill: parent
        anchors.
        id: test_list_view
        TableViewColumn { title: "gamma"; role: "gamma"; width: 150 }
        TableViewColumn { title: "tint"; role: "tint"; width: 150 }
        model: test_model
        itemDelegate: {return editableDelegate;}
    }*/

    /*MessageDialog {
        id: messageDialog

        function show(title, caption) {
            messageDialog.text = caption;
            messageDialog.title = title;
            messageDialog.open();
        }
    }*/
}

