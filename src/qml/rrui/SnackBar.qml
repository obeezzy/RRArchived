import "../singletons"
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3

FluidControls.SnackBar {
    id: snackBar

    function show(text, buttonText, duration) {
        if (buttonText === undefined)
            buttonText = "";

        if (duration === undefined)
            duration = GlobalSettings.shortToastDuration;

        snackBar.duration = duration;
        snackBar.open(text, buttonText);
    }

    parent: FluidControls.ApplicationWindow.contentItem
    duration: GlobalSettings.shortToastDuration
    fullWidth: false
    onClicked: close()
}
