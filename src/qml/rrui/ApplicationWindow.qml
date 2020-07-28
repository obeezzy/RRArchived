import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls.Material 2.3
import com.gecko.rr 1.0 as RR

FluidControls.ApplicationWindow {
    id: applicationWindow

    readonly property alias snackBar: snackBar

    width: 640
    height: 480
    visible: true
    title: qsTr("Record Rack")
    appBar.visible: false
    color: settings.darkModeActive ? Material.color(Material.Grey, Material.Shade900) : Qt.lighter("lightgray")
    Material.theme: settings.darkModeActive ? Material.Dark : Material.Light
    Material.accent: Material.LightGreen

    RRUi.SnackBar {
        id: snackBar
    }

    RR.Settings {
        id: settings
    }

}
