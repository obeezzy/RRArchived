import ".."
import "../../rrui" as RRUi
import "../../singletons"
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import com.gecko.rr 1.0 as RR
import com.gecko.rr.models 1.0 as RRModels

RRUi.WizardPage {
    id: paymentSummaryPage

    property string customerName: ""
    property string customerPhoneNumber: ""
    property var paymentModel: null
    property RRModels.SaleCartModel cartModel: null
    property real totalCost: 0
    property real amountPaid: 0
    readonly property real balance: totalCost - amountPaid

    objectName: "paymentSummaryPage"
    padding: FluidControls.Units.smallSpacing
    title: qsTr("View payment details")
    hasNext: false
    onNext: {
        if (printCheckBox.checked)
            receiptPrinter.print(cartModel.toPrintableFormat());

        paymentSummaryPage.nextPage.component = Qt.resolvedUrl("PaymentFinishPage.qml");
        paymentSummaryPage.finish();
    }

    RR.ReceiptPrinter {
        id: receiptPrinter
    }

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

        QQC2.CheckBox {
            id: printCheckBox

            text: qsTr("Print receipt")

            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }

        }

    }

}
