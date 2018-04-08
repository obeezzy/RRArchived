import QtQuick 2.10
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Controls.Material 2.3
import "../singletons"

FluidControls.InfoBar {
    id: snackBar
    parent: FluidControls.ApplicationWindow.contentItem
    duration: GlobalSettings.shortToastDuration
    fullWidth: false
    onClicked: opened = false;

    function show(text, buttonText) {
        if (buttonText === undefined)
            buttonText = "";

        snackBar.buttonText = buttonText;
        snackBar.open(text);
    }
}
