import QtQuick 2.5
import QtQuick.Controls 1.4

Rectangle {
    implicitWidth: 100
    implicitHeight: 100
    Image {
        id: image
        layer.enabled: true
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height
        source: "file:///mnt/bulkdata/SDO/2011/04/13/20110413_004253_4096_0335.jpg"
        width: (sourceSize.width / sourceSize.height) * height
        property real scaleRel: sourceSize.height / height
        Column {
            z: 1
            Repeater {
                id: spinBoxRepeater
                model: pointListModel
                delegate: Row {
                    SpinBox {
                        id: xSpinBox
                        activeFocusOnPress: true
                        activeFocusOnTab: true
                        minimumValue: -999999
                        maximumValue: 999999
                        decimals: 5
                        value: x_
                        onValueChanged: {
                            if(Math.abs(x_ - value) > 0.00001)
                            {
                                x_ = value
                            }
                        }
                    }
                    SpinBox {
                        id: ySpinBox
                        minimumValue: -999999
                        maximumValue: 999999
                        decimals: 5
                        value: y_
                        onValueChanged: {
                            if(Math.abs(y_ - value) > 0.00001)
                            {
                                y_ = value
                            }
                        }
                    }
                }
            }
        }
        Repeater {
            id: controlPointRepeater
            model: pointListModel
            delegate: Rectangle {
                id: rect
                transformOrigin: Item.Center
                x: x_ / image.scaleRel
                y: y_ / image.scaleRel
                z: 2
                width: 11
                height: 11
                color: activeFocus ? "red" : "yellow"
                Drag.active: dragArea.drag.active
                Keys.onDeletePressed: pointListModel.delAtIndex(index)
                onXChanged: x_ = x * image.scaleRel
                onYChanged: y_ = y * image.scaleRel
                MouseArea {
                    id: dragArea
                    anchors.fill: parent
                    drag.target: parent
                    onPressed: parent.focus = true
                    drag.threshold: 0
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                pointListModel.append(mouseX * image.scaleRel, mouseY * image.scaleRel)
            }
        }
    }
}
