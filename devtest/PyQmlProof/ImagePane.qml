// The MIT License (MIT)
//
// Copyright (c) 2016 Erik Hvatum
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Authors: Erik Hvatum <ice.rikh@gmail.com>

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

Item {
    id: imageContainer
    Layout.fillWidth: true
    clip: true

    DropArea {
        anchors.fill: parent
        onEntered: {
//            console.log('drop area entered');
            drag.accept(Qt.CopyAction);
        }
        onDropped: {
            image.source = drop.urls[0]
        }
//        onExited: {
//            console.log('drop area exited');
//        }
    }

    TiledBackground {}

    Image {
        id: image
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: imageContainer.horizontalCenter
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

        Canvas {
            anchors.fill: parent
//            renderStrategy: Canvas.Threaded
//            renderTarget: Canvas.FramebufferObject
            antialiasing: true
            smooth: true
            Component.onCompleted: {
                pointListModel.dataChanged.connect(requestPaint)
                pointListModel.rowsInserted.connect(requestPaint)
                pointListModel.rowsRemoved.connect(requestPaint)
                pointListModel.modelReset.connect(requestPaint)
                pointListModel.rowsMoved.connect(requestPaint)
            }
            onPaint: {
                var rowCount = pointListModel.rowCount()
                if(rowCount >= 2) {
                    var ctx = getContext("2d")
                    ctx.reset()
                    ctx.strokeStyle = Qt.rgba(0,1,0,1)
                    ctx.lineWidth = 2
                    ctx.beginPath()
                    var p = pointListModel.data(pointListModel.index(0, 0), 0)
                    ctx.moveTo(p.x_/image.scaleRel, p.y_/image.scaleRel)
                    var idx_=1
                    for(; idx_ < rowCount; idx_++) {
                        p = pointListModel.data(pointListModel.index(idx_, 0), 0)
                        ctx.lineTo(p.x_/image.scaleRel, p.y_/image.scaleRel)
                    }
                    ctx.stroke()
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
