import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import com.gecko.rr.components 1.0 as RRComponents

import "../../singletons"

RRUi.Dialog {
    id: amountPaidDialog

    property real totalDebt: 0
    property int debtIndex: -1
    property int paymentIndex: -1
    readonly property real amountPaid: amountPaidTextField.text === "" ? 0 : parseFloat(amountPaidTextField.text)
    readonly property bool isExistingPayment: paymentIndex >= 0

    function show(debtIndex, paymentIndex, totalDebt) {
        if (debtIndex !== undefined)
            amountPaidDialog.debtIndex = debtIndex;
        if (paymentIndex !== undefined)
            amountPaidDialog.paymentIndex = paymentIndex;
        if (totalDebt !== undefined)
            amountPaidDialog.totalDebt = totalDebt;

        amountPaidDialog.open();
    }

    width: 480

    onClosed: {
        totalDebt = 0;
        debtIndex = -1;
        paymentIndex = -1;
    }

    title: isExistingPayment ? qsTr("Edit payment") : qsTr("New payment")

    contentItem: Item {
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
                text: "0.00"
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                bottomPadding: 12
                validator: RRComponents.DoubleValidator { bottom: 0; top: amountPaidDialog.totalDebt }
            }
        }
    }
}
