import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import StackStream 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("StackStream")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
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

    /*MainForm {
        anchors.fill: parent
        button1.onClicked: messageDialog.show(qsTr("tint is"), a_layer.tint)
        button2.onClicked: a_layer.tint = Qt.rgba(.1,.1,.1,.1)//messageDialog.show(qsTr("Button 2 pressed"))
        resetButton.onClicked: a_layer.tint = undefined
    }*/

    Component {
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
                            test_model.setData(test_model.index(styleData.row, styleData.role), Number(parseFloat(loaderEditor.item.text).toFixed(0)))
                        else
                            test_model.setData(test_model.index(styleData.row, styleData.role), loaderEditor.item.text)
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
        anchors.fill: parent
        id: test_list_view
        TableViewColumn { title: "gamma"; role: "gamma"; width: 150 }
        TableViewColumn { title: "tint"; role: "tint"; width: 150 }
        model: test_model
        itemDelegate: {return editableDelegate;}
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

