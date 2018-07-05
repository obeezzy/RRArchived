import QtQuick 2.10
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.3 as QQC2
import Fluid.Controls 1.0 as FluidControls

FluidControls.OverlayView {
    id: busyOverlay

    closePolicy: QQC2.Popup.NoAutoClose
    modal: true
    parent: FluidControls.ApplicationWindow.contentItem

    FluidControls.Card {
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 200
        height: 100

        QQC2.BusyIndicator {
            anchors.centerIn: parent
            running: true
        }
    }
}
