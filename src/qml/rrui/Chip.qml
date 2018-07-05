import QtQuick 2.10
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.3 as QQC2
import Fluid.Controls 1.0 as FluidControls

Item {
    id: chip

    property string text: qsTr("Chip")
    signal clicked

    implicitWidth: row.width + 20
    implicitHeight: row.height + 10
    scale: mouseArea.containsPress ? 0.95 : mouseArea.containsMouse ? 1.1 : 1

    Rectangle {
        anchors.fill: parent
        color: "#eeeeee"
        radius: width / 2
    }

    Row {
        id: row
        anchors.centerIn: parent

        FluidControls.CaptionLabel {
            text: chip.text
            color: "#444444"
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: chip.clicked();
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
    }

    Behavior on scale { NumberAnimation { duration: 100; property: "scale" } }
}
