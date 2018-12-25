import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../../rrui" as RRUi

RRUi.AlertDialog {
    id: removeConfirmationDialog

    property int debtIndex: -1
    property int paymentIndex: -1
    readonly property bool isPayment: debtIndex > -1 && paymentIndex > -1

    onClosed: {
        debtIndex = -1;
        paymentIndex = -1;
    }

    title: qsTr("Remove item?                                                  ")
    text: qsTr("Are you sure you would like to remove this item?")
    standardButtons: QQC2.Dialog.Yes | QQC2.Dialog.No
    closePolicy: QQC2.Popup.CloseOnEscape

    function show(itemName, debtIndex, paymentIndex) {
        if (itemName !== undefined)
            text = qsTr("Are you sure you would like to remove %1?").arg(itemName);
        if (debtIndex !== undefined)
            removeConfirmationDialog.debtIndex = debtIndex;
        if (paymentIndex !== undefined)
            removeConfirmationDialog.paymentIndex = paymentIndex;

        open();
    }
}
