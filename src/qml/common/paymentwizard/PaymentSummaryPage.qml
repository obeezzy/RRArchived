import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import Fluid.Controls 1.0 as FluidControls

QQC2.Page {
    id: paymentSummaryPage
    objectName: "paymentSummaryPage"

    property string customerName: "Test Johnson"
    property string customerPhoneNumber: "234-555-2384"
    property real totalCost: 200
    property real amountPaid: 68
    readonly property real balance: totalCost - amountPaid

    padding: FluidControls.Units.smallSpacing
    title: qsTr("View payment details")
}
