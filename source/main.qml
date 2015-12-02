import QtQuick 2.5
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import StackStream 1.0

ApplicationWindow {
    id: stackStreamMainWindow
    objectName: "stackStreamMainWindow"
    visible: true
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
        anchors.fill: parent
        anchors.margins: 10
        onIsValidChanged: console.log(isValid)
    }

    FileDialog {
        id: fileDialog
        visible: false
        modality: Qt.WindowModal
        sidebarVisible: true
        selectMultiple: false
        onAccepted: {
            /*if(layer_.image == null) {
                layer_.image = Qt.createQmlObject('import StackStream 1.0; SSImage {}', layer_, 'apath')
            }*/
            layer_.image.read(fileUrls[0]);
        }
    }
}
