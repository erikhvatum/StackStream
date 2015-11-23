import QtQuick 2.0
import StackStream 1.0

SSLayer {
    id: draggableLayer
    Drag.active: mouseArea.drag.active
    Drag.hotSpot.x: 10
    Drag.hotSpot.y: 10
    property alias mouseArea: mouseArea
    property int flaf: 10
    property SSImage image

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: parent
    }
}

