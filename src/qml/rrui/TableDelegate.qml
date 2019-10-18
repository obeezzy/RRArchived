import QtQuick 2.13
import "../singletons"

Item {
    implicitWidth: 100
    implicitHeight: 100

    Rectangle {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        color: Stylesheet.tableViewDivider
        height: 1
        z: 2
    }
}
