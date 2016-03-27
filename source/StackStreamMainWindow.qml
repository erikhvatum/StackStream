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
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2

import StackStream 1.0

ApplicationWindow {
    visible: false
    width: 1024
    height: 768
    title: qsTr("StackStream")
    objectName: "stackStreamMainWindow"
    property SSImage image: SSImage{}

    SSView {
        id: mainView
//        anchors.left: parent.left
//        anchors.top: parent.top
//        anchors.bottom: parent.bottom
//        width: parent.width - layerPropertiesGroupBox.width - 20
        anchors.fill: parent
        z: -1

        TileBackground {
            anchors.fill: parent
        }

        SSImageItem {
            id: imageItem
            y: 0
            x: 0
            layer.enabled: true
            Drag.active: mouseArea.drag.active
            Drag.hotSpot.x: 10
            Drag.hotSpot.y: 10

            transform: Scale {
                id: scale
                property real factor: 1.0
                xScale: factor
                yScale: factor
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                drag.target: parent
                property var factors: [ 4.0       ,  3.8       ,  3.6       ,  3.4       ,  3.2       ,
                                        3.0       ,  2.8       ,  2.6       ,  2.4       ,  2.2       ,
                                        2.0       ,  1.8       ,  1.6       ,  1.4       ,  1.2       ,
                                        1.0       ,  0.91700404,  0.84089642,  0.77110541,  0.70710678,
                                        0.64841978,  0.59460356,  0.54525387,  0.5       ,  0.45850202,
                                        0.42044821,  0.38555271,  0.35355339,  0.32420989,  0.29730178,
                                        0.27262693,  0.25      ]
                property int factorIdx: 15
                onWheel: {
                    factorIdx = (factorIdx + (wheel.angleDelta.y / 120)) | 0
                    if(factorIdx < 0) factorIdx = 0
                    else if (factorIdx >= factors.length) factorIdx = factors.length - 1
                    scale.factor = factors[factorIdx]
                }
            }
        }

//        Rectangle {
//            x: 0
//            y: 0
//            width: 100
//            height: 200
//            anchors.fill: imageItem
//            anchors.margins: 5
//            color: 'blue'
//            opacity: 0.5
//            layer.enabled: true
//        }

        DropArea {
            anchors.fill: parent
            onEntered: {
    //            console.log('drop area entered');
                drag.accept(Qt.CopyAction);
            }
            onDropped: {
                image.read(drop.urls[0])
                imageItem.image = image
            }
    //        onExited: {
    //            console.log('drop area exited');
    //        }
        }
    }
}
