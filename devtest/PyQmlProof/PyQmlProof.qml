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
            Repeater {
                id: spinBoxRepeater
                model: pointListModel
                delegate: Row {
                    SpinBox {
                        id: xSpinBox
                        minimumValue: -999999
                        maximumValue: 999999
                        decimals: 5
                        value: x_
                        z: 11
                        onValueChanged: {
                            if(Math.abs(x_ - value) > 0.00001)
                            {
                                x_ = value
                            }
//                            var p = outlinePath.pointAt(index)
//                            var r = image.sourceSize.height / image.height
//                            outlinePath.setPointAt(index, Qt.point(value * r, p.y * r))
//                            controlPointRepeater.itemAt(index).x = value / r
                        }
                    }
                    SpinBox {
                        id: ySpinBox
                        minimumValue: -999999
                        maximumValue: 999999
                        decimals: 5
                        value: y_
                        z: 11
                        onValueChanged: {
                            if(Math.abs(y_ - value) > 0.00001)
                            {
                                y_ = value
                            }
//                            var p = outlinePath.pointAt(index)
//                            var r = image.sourceSize.height / image.height
//                            outlinePath.setPointAt(index, Qt.point(p.x * r, value * r))
//                            controlPointRepeater.itemAt(index).y = value / r
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
                x: x_ * image.scaleRel
                y: y_ * image.scaleRel
                z: 10
                width: 10
                height: 10
                color: activeFocus ? "red" : "yellow"
                Drag.active: dragArea.drag.active
                Keys.onDeletePressed: pointListModel.delAtIndex(index)
                onXChanged: x_ = x / image.scaleRel
                onYChanged: y_ = y / image.scaleRel
                MouseArea {
                    id: dragArea
                    anchors.fill: parent
                    drag.target: parent
                    onPressed: parent.focus = true
//                    onPositionChanged: {
//                        if(Drag.active) {
//                            var p = Qt.point(rect.x * image.sourceSize.height / image.height,
//                                             rect.y * image.sourceSize.height / image.height)
//                            outlinePath.setPointAt(index, p)
//                            console.log(spinBoxRepeater.itemAt(index).xSpinBox.value)
//                            spinBoxRepeater.itemAt(index).xSpinBox.value = p.x
//                            spinBoxRepeater.itemAt(index).ySpinBox.value = p.y
//                        }
//                    }
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                pointListModel.append(mouseX / image.scaleRel, mouseY / image.scaleRel)
            }
        }
    }
}
