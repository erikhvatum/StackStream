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

GroupBox {
    id: pointsGroupBox
    title: "Points"
    Layout.minimumWidth: 1
    Layout.margins: 5
    width: 400
    clip: true

    Column {
        id: "column"
        anchors.fill: parent
        z: 1

        TableView {
            id: tableView
            anchors.margins: 5
            anchors.left: parent.left
            anchors.right: parent.right
            model: pointListModel
            selectionMode: SelectionMode.ExtendedSelection
            activeFocusOnTab: true

            TableViewColumn {
                role: "x_"
                title: "x"
                width: column.width / 2 - 20
                movable: false
            }

            TableViewColumn {
                role: "y_"
                title: "y"
                width: column.width / 2 - 20
                movable: false
            }

//            Keys.onPressed: {
//                console.log("pressed")
//                tableView.selection.forEach(function(row){console.log(row)})
//            }

//            onFocusChanged: {
//                console.log(focus)
//            }

//            onClicked: {
//                console.log("clicked")
//                parent.parent.forceActiveFocus()
//                parent.parent.focus = true
//            }
        }

        Repeater {
            id: spinBoxRepeater
            model: pointListModel
            delegate: Row {
                spacing: 5

                Label {
                    text: index.toString()
                }

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
