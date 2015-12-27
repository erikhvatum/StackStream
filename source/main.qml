import QtQuick 2.5
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2
import StackStream 1.0

ApplicationWindow {
    id: stackStreamMainWindow
    objectName: "stackStreamMainWindow"
    visible: false
    width: 1024
    height: 768
    title: qsTr("StackStream")

    menuBar: MenuBar {
        id: foobar
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Open")
                onTriggered: fileDialog.open()
            }

            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: qsTr("Help")
            MenuItem {
                text: qsTr("About Qt")
                onTriggered: layer_.aboutQt()
            }
        }
    }

    RowLayout {
        anchors.fill: parent

        View {
            id: mainView
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width - layerPropertiesGroupBox.width - 20
            z: -1
//            layer.enabled: true

            ShaderEffect {
                id: tileBackground
                anchors.fill: parent

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

            SSLayer {
                id: layer_
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: (implicitWidth / implicitHeight) * height
                anchors.margins: 10
//                layer.enabled: true
//                Rectangle {
//                    x: 10
//                    y: 10
//                    color: Qt.rgba(1,0,0,1)
//                    width: 100
//                    height: 100
//                }
            }
        }

        GroupBox {
            id: layerPropertiesGroupBox
            title: "Layer Properties"
            anchors.margins: 10
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            layer.enabled: true

            GridLayout {
                columns: 3
                flow: GridLayout.LeftToRight
                anchors.fill: parent
                property bool _setting: false

                function _setMin(min_) {
                    if(!_setting) {
                        _setting = true;
                        layer_.min = min_;
                        _setting = false;
                    }
                }
                function _setMax(max_) {
                    if(!_setting) {
                        _setting = true;
                        layer_.max = max_;
                        _setting = false;
                    }
                }
                function _setGamma(gamma_) {
                    if(!_setting) {
                        _setting = true;
                        layer_.gamma = gamma_;
                        _setting = false;
                    }
                }

                Label { text: "Min: " }
                Slider {
                    id: minSlider
                    Layout.fillWidth: true
                    minimumValue: 0
                    maximumValue: 1
                    value: layer_.min
                    onValueChanged: { parent._setMin(value) }
                }
                SpinBox {
                    id: minSpinBox
                    decimals: 3
                    value: layer_.min
                    onValueChanged: { parent._setMin(value) }
                }
                Label { text: "Max: " }
                Slider {
                    id: maxSlider
                    Layout.fillWidth: true
                    minimumValue: 0
                    maximumValue: 1
                    value: layer_.max
                    onValueChanged: { parent._setMax(value) }
                }
                SpinBox {
                    id: maxSpinBox
                    decimals: 3
                    value: layer_.max
                    onValueChanged: { parent._setMax(value) }
                }
                Label { text: "γ: " }
                Slider {
                    id: gammaSlider
                    Layout.fillWidth: true
                    minimumValue: 0.01
                    maximumValue: 10
                    value: layer_.gamma
                    onValueChanged: { parent._setGamma(value) }
                }
                SpinBox {
                    id: gammaSpinBox
                    decimals: 3
                    minimumValue: 0.01
                    maximumValue: 10
                    value: layer_.gamma
                    onValueChanged: { parent._setGamma(value) }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.columnSpan: 3
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        visible: false
        modality: Qt.WindowModal
        sidebarVisible: true
        selectMultiple: false
        onAccepted: {
            if(layer_.image == null) {
                layer_.image = Qt.createQmlObject('import StackStream 1.0; SSImage {}', layer_, 'apath')
            }
            layer_.image.read(fileUrls[0]);
        }
    }
}
