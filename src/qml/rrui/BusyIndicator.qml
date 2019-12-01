import QtQuick 2.12
import QtQuick.Controls 2.12
import Fluid.Controls 1.0 as FluidControls

Item {
    id: busyIndicator

    property string text: ""

    implicitWidth: column.width
    implicitHeight: column.height

    Column {
        id: column
        spacing: 16

        BusyIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
            running: true
        }

        FluidControls.SubheadingLabel {
            anchors.horizontalCenter: parent.horizontalCenter
            text: busyIndicator.text
        }
    }
}
