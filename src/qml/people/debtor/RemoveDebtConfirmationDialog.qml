import "../../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3

RRUi.AlertDialog {
    id: removeDebtConfirmationDialog

    property int debtIndex: -1
    property int paymentIndex: -1
    property var paymentModel: null
    readonly property bool isPayment: debtIndex > -1 && paymentIndex > -1

    function show(paymentName, debtIndex, paymentIndex, paymentModel) {
        if (paymentName !== undefined)
            text = qsTr("Are you sure you would like to remove %1?").arg(paymentName);

        if (debtIndex !== undefined)
            removeDebtConfirmationDialog.debtIndex = debtIndex;

        if (paymentIndex !== undefined)
            removeDebtConfirmationDialog.paymentIndex = paymentIndex;

        if (paymentModel !== undefined)
            removeDebtConfirmationDialog.paymentModel = paymentModel;

        open();
    }

    onClosed: {
        debtIndex = -1;
        paymentIndex = -1;
        paymentModel = null;
    }
    title: qsTr("Remove payment?                                                  ")
    text: qsTr("Are you sure you would like to remove this payment?")
    standardButtons: QQC2.Dialog.Yes | QQC2.Dialog.No
    closePolicy: QQC2.Popup.CloseOnEscape
}
