import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls

Loader {
    id: failureAlertDialogLoader

    property string title: ""
    property string message: ""

    active: false
    x: (MainWindow.width - width) / 2
    y: (MainWindow.height - height) / 2

    sourceComponent: FluidControls.AlertDialog {
        parent: MainWindow.contentItem
        text: failureAlertDialogLoader.title
        standardButtons: QQC2.Dialog.Ok
        closePolicy: QQC2.Popup.CloseOnEscape
        onClosed: failureAlertDialogLoader.active = false;

        FluidControls.BodyLabel {
            text: failureAlertDialogLoader.message
        }
    }
    onLoaded: item.open();

    function create() { active = true; }
}
