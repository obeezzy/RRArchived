import QtQuick 2.12
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi

FluidControls.ApplicationWindow {
    id: applicationWindow

    readonly property alias snackBar: snackBar

    width: 640
    height: 480
    visible: true
    title: qsTr("Record Rack")
    appBar.visible: false
    color: Qt.lighter("lightgray")

    Material.accent: Material.LightGreen

    RRUi.SnackBar { id: snackBar }
}
