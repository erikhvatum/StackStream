import QtQuick 2.5
import QtQuick.Controls 1.4

Rectangle {
    id: containerRect
    implicitWidth: 1024
    implicitHeight: 768
    GroupBox {
        id: pointsGroupBox
        title: "Points"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 5
        implicitWidth: 350
        Column {
            z: 1
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
                        onValueChanged: {
                            if(Math.abs(x_ - value) > 0.00001) x_ = value
                        }
                    }
                    SpinBox {
                        id: ySpinBox
                        minimumValue: -999999
                        maximumValue: 999999
                        decimals: 5
                        value: y_
                        onValueChanged: {
                            if(Math.abs(y_ - value) > 0.00001) y_ = value
                        }
                    }
                    Button {
                        id: delButton
                        text: 'X'
                        onClicked: {
                            pointListModel.delAtIndex(index)
                        }
                    }
                }
            }
        }
    }
    Item {
        id: imageContainer
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: pointsGroupBox.right
        anchors.right: parent.right
        clip: true
        ShaderEffect {
            id: tileBackground
            anchors.fill: parent
            layer.enabled: true

            property real tileSize: 16
            property color color1: Qt.rgba(0.9, 0.9, 0.9, 1);
            property color color2: Qt.rgba(0.85, 0.85, 0.85, 1);

            property size pixelSize: Qt.size(width / tileSize, height / tileSize);

            fragmentShader:
                "
                uniform lowp vec4 color1;
                uniform lowp vec4 color2;
                uniform highp vec2 pixelSize;
                varying highp vec2 qt_TexCoord0;
                void main() {
                    highp vec2 tc = sign(sin(3.14152 * qt_TexCoord0 * pixelSize));
                    if (tc.x != tc.y)
                        gl_FragColor = color1;
                    else
                        gl_FragColor = color2;
                }
                "
        }
        Image {
            id: image
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: imageContainer.horizontalCenter
            source: "file:///mnt/bulkdata/SDO/2011/04/13/20110413_004253_4096_0335.jpg"
            property real scaleRel: sourceSize.height / height
            width: (sourceSize.width / sourceSize.height) * height
            Repeater {
                id: controlPointRepeater
                model: pointListModel
                delegate: Rectangle {
                    id: rect
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
                    transform: Translate {
                        x: -5
                        y: -5
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
}
