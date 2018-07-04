import QtQuick 2.10
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls

FluidControls.ApplicationWindow {
    id: applicationWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Record Rack")
    appBar.visible: false
    color: Qt.lighter("lightgray")

    Material.accent: Material.LightGreen
}
