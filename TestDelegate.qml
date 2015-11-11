import QtQuick 2.0

Item {
    id: delegate
    width: delegate.ListView.view.width
    height: 30
    clip: true
    anchors.margins: 4
    Row {
        anchors.margins: 4
        anchors.fill: parent
        spacing: 4
        Text {
            text: gamma
            width: 150
        }
        Text {
            text: tint
            width: 300
        }
    }
}

