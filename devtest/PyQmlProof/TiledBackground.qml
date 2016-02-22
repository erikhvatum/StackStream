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

ShaderEffect {
    anchors.fill: parent
//    layer.enabled: true

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
