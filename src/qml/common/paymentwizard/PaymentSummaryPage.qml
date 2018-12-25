import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import Fluid.Controls 1.0 as FluidControls
import "../../rrui" as RRUi
import "../../singletons"

RRUi.Page {
    id: paymentSummaryPage
    objectName: "paymentSummaryPage"

    property string customerName: ""
    property string customerPhoneNumber: ""
    property var paymentModel: null
    property real totalCost: 0
    property real amountPaid: 0
    readonly property real balance: totalCost - amountPaid

    padding: FluidControls.Units.smallSpacing
    title: qsTr("View payment details")

    contentItem: Item {
        QQLayouts.GridLayout {
            anchors.centerIn: parent
            columns: 2
            columnSpacing: FluidControls.Units.largeSpacing * 2

            FluidControls.SubheadingLabel {
                text: qsTr("Customer name:")
                visible: paymentSummaryPage.customerName
            }

            FluidControls.SubheadingLabel {
                text: paymentSummaryPage.customerName
                horizontalAlignment: Qt.AlignRight
            }

            FluidControls.SubheadingLabel {
                text: qsTr("Customer phone number:")
                visible: paymentSummaryPage.customerPhoneNumber
            }

            FluidControls.SubheadingLabel {
                QQLayouts.Layout.fillWidth: true
                visible: paymentSummaryPage.customerPhoneNumber
                text: paymentSummaryPage.customerPhoneNumber
                horizontalAlignment: Qt.AlignRight
            }

            FluidControls.SubheadingLabel {
                text: qsTr("Total cost:")
            }

            FluidControls.SubheadingLabel {
                QQLayouts.Layout.fillWidth: true
                text: Number(paymentSummaryPage.totalCost).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
                horizontalAlignment: Qt.AlignRight
            }

            FluidControls.SubheadingLabel {
                text: qsTr("Amount paid:")
            }

            FluidControls.SubheadingLabel {
                QQLayouts.Layout.fillWidth: true
                text: Number(paymentSummaryPage.amountPaid).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
                horizontalAlignment: Qt.AlignRight
            }

            FluidControls.SubheadingLabel {
                text: qsTr("Balance:")
            }

            FluidControls.SubheadingLabel {
                QQLayouts.Layout.fillWidth: true
                text: Number(paymentSummaryPage.balance).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
                horizontalAlignment: Qt.AlignRight
            }
        }
    }
}
