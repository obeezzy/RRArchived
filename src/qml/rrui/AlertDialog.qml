import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls

FluidControls.AlertDialog {
    parent: QQC2.ApplicationWindow.contentItem

    function show() { open(); }
}
