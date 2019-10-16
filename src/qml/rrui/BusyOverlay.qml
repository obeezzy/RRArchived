import QtQuick 2.12
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi

FluidControls.OverlayView {
    id: busyOverlay

    property string text: ""

    closePolicy: QQC2.Popup.NoAutoClose
    modal: true
    parent: MainWindow ? MainWindow.contentItem : null

    RRUi.Card {
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: Math.max(200, row.width + 24)
        height: 100

        Row {
            id: row
            anchors.centerIn: parent
            spacing: 8

            QQC2.BusyIndicator { running: true }

            FluidControls.SubheadingLabel {
                anchors.verticalCenter: parent.verticalCenter
                visible: busyOverlay.text !== ""
                text: busyOverlay.text
            }
        }
    }
}
