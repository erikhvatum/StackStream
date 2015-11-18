import QtQuick 2.0

Image {
    id: draggableImage
    Drag.active: mouseArea.drag.active
    Drag.hotSpot.x: 10
    Drag.hotSpot.y: 10
    property alias mouseArea: mouseArea
    property int flaf: 10
    property Image otherImage

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: parent

        onPressed: {
            draggableImage.z = 10;
            draggableImage.otherImage.z = 0;
        }
    }
}

