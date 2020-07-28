import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2

FluidControls.AlertDialog {
    id: errorDialog

    function show(message, title) {
        if (message !== undefined)
            errorDialog.text = message;

        if (title !== undefined)
            errorDialog.title = title + "                                                                   ";

        open();
    }

    title: qsTr("Error                                                                                ")
    text: qsTr("An unknown error occurred.")
    standardButtons: QQC2.Dialog.Ok
    closePolicy: QQC2.Popup.CloseOnEscape
    onClosed: {
        title = qsTr("Error                                                                                ");
        text = qsTr("An unknown error occurred.");
    }
    onAboutToShow: footer.contentItem.focus = true
}
