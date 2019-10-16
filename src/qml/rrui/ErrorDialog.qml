import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls

FluidControls.AlertDialog {
    id: errorDialog

    property int errorCode: 0

    function show(message, title, errorCode) {
        if (message !== undefined)
            errorDialog.text = message;
        if (title !== undefined)
            errorDialog.title = title + "                                                                   ";
        if (errorCode !== undefined)
            errorDialog.errorCode = errorCode;

        open();
    }

    title: qsTr("Error                                                                                ")
    text: qsTr("Unknown error occurred (%1).").arg(errorCode)
    parent: MainWindow !== undefined ? MainWindow.contentItem : null
    standardButtons: QQC2.Dialog.Ok
    closePolicy: QQC2.Popup.CloseOnEscape
    onClosed: {
        title = qsTr("Error                                                                                ");
        text = qsTr("Unknown error occurred (%1).").arg(errorCode);
    }
}
