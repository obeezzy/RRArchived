import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import com.gecko.rr.components 1.0 as RRComponents
import "../../singletons"

RRUi.Dialog {
    id: amountPaidDialog

    property real maxPayableAmount: 0
    property var paymentModelData: null
    property var paymentModel: null
    readonly property real amountPaid: amountPaidTextField.text === "" ? 0 : parseFloat(amountPaidTextField.text)
    readonly property bool isExistingPayment: paymentModelData !== null

    function show(paymentModelData, paymentModel) {
        if (paymentModelData !== undefined)
            amountPaidDialog.paymentModelData = paymentModelData;
        if (paymentModel !== undefined)
            amountPaidDialog.paymentModel = paymentModel;

        if (paymentModelData && Object(paymentModelData).hasOwnProperty("amount_paid"))
            amountPaidTextField.text = Number(paymentModelData.amount_paid).toFixed(2);
        else
            amountPaidTextField.text = Number(0).toFixed(2);

        if (paymentModelData && Object(paymentModelData).hasOwnProperty("max_payable_amount")) {
            amountPaidDialog.totalDebt = paymentModelData.max_payable_amount;
        } else if (paymentModel && paymentModel.rowCount() > 0
                 && Object(paymentModel.get(paymentModel.rowCount() - 1)).hasOwnProperty("max_payable_amount")) {
            amountPaidDialog.maxPayableAmount = paymentModel.get(paymentModel.rowCount() - 1).max_payable_amount;
        }

        amountPaidDialog.open();
    }

    width: 480
    title: isExistingPayment ? qsTr("Edit payment") : qsTr("New payment")

    onAboutToShow: amountPaidTextField.selectAll();
    onClosed: {
        maxPayableAmount = 0;
        paymentModelData = null;
        paymentModel = null;
    }

    contentItem: FocusScope {
        focus: amountPaidDialog.activeFocus

        Row {
            anchors.centerIn: parent
            spacing: 16

            FluidControls.SubheadingLabel {
                text: qsTr("Amount paid:")
                height: amountPaidTextField.height
                verticalAlignment: Qt.AlignBottom
                bottomPadding: 8
            }

            RRUi.TextField {
                id: amountPaidTextField
                text: Number(0).toFixed(2)
                focus: amountPaidDialog.activeFocus
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                bottomPadding: 12
                validator: RRComponents.DoubleValidator { bottom: 0; top: amountPaidDialog.maxPayableAmount }
            }
        }
    }
}
