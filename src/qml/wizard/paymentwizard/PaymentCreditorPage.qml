import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import "../../rrui" as RRUi
import ".."

RRUi.WizardPage {
    id: paymentCreditorPage
    objectName: "paymentCreditorPage"

    property real totalCost: 0
    //property real amountPaid: parseFloat(amountPaidField.text == "" ? 0 : amountPaidField.text)
    readonly property real balance: 0 //totalCost - amountPaid
    readonly property bool hasDebt: balance > 0
    readonly property bool hasCredit: balance < 0

    padding: FluidControls.Units.smallSpacing
    title: qsTr("Enter card details")
    hasNext: true
}
