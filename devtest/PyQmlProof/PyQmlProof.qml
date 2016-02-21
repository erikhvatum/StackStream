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
        source: "file:///Volumes/bulkdata/Sinha_Drew/2015.11.13_ZPL8Prelim3/non-vignette.png"
        width: (sourceSize.width / sourceSize.height) * height
        Column {
            Repeater {
                id: spinBoxRepeater
                model: outlinePath.pointCount
                Row {
                    property alias xSpinBox: xSpinBox
                    SpinBox {
                        id: xSpinBox
                        minimumValue: -999999
                        maximumValue: 999999
                        decimals: 3
                        value: outlinePath.pointAt(index).x
                        z: 11
                        onValueChanged: {
                            var p = outlinePath.pointAt(index)
                            var r = image.sourceSize.height / image.height
                            outlinePath.setPointAt(index, Qt.point(value * r, p.y * r))
                            controlPointRepeater.itemAt(index).x = value / r
                        }
                    }
                    property alias ySpinBox: ySpinBox
                    SpinBox {
                        id: ySpinBox
                        minimumValue: -999999
                        maximumValue: 999999
                        decimals: 3
                        value: outlinePath.pointAt(index).y
                        z: 11
                        onValueChanged: {
                            var p = outlinePath.pointAt(index)
                            var r = image.sourceSize.height / image.height
                            outlinePath.setPointAt(index, Qt.point(p.x * r, value * r))
                            controlPointRepeater.itemAt(index).y = value / r
                        }
                    }
                }
            }
        }
        Repeater {
            id: controlPointRepeater
            model: outlinePath.pointCount
            Rectangle {
                id: rect
                x: outlinePath.pointAt(index).x * image.height / image.sourceSize.height
                y: outlinePath.pointAt(index).y * image.height / image.sourceSize.height
                z: 10
                width: 10
                height: 10
                color: activeFocus ? "red" : "yellow"
                Drag.active: dragArea.drag.active
                Keys.onDeletePressed: outlinePath.delPointAt(index)
                MouseArea {
                    id: dragArea
                    anchors.fill: parent
                    drag.target: parent
                    onPressed: parent.focus = true
                    onPositionChanged: {
                        if(Drag.active) {
                            var p = Qt.point(rect.x * image.sourceSize.height / image.height,
                                             rect.y * image.sourceSize.height / image.height)
                            outlinePath.setPointAt(index, p)
                            console.log(spinBoxRepeater.itemAt(index).xSpinBox.value)
                            spinBoxRepeater.itemAt(index).xSpinBox.value = p.x
                            spinBoxRepeater.itemAt(index).ySpinBox.value = p.y
                        }
                    }
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                outlinePath.appendPoint(Qt.point(mouseX * image.sourceSize.height / image.height,
                                                 mouseY * image.sourceSize.height / image.height))
            }
        }
    }
}
