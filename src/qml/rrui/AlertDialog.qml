import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls

FluidControls.AlertDialog {
    id: alertDialog

    function show(message, title) {
        if (message !== undefined)
            alertDialog.text = message;
        if (title !== undefined)
            alertDialog.title = title + "                                                                   ";

        open();
    }

    title: qsTr("Alert                                                                                ")
    parent: MainWindow !== undefined ? MainWindow.contentItem : null
    standardButtons: QQC2.Dialog.Ok
    closePolicy: QQC2.Popup.CloseOnEscape
    onClosed: {
        title = qsTr("Alert                                                                                ");
    }
}

