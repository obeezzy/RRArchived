import QtQuick 2.12

Text {
    anchors {
        left: parent.left
        right: parent.right
    }
    font {
        family: "dotty"
        pixelSize: 50
    }

    visible: text !== ""
}
