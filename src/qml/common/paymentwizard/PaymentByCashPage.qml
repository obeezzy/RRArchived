import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.components 1.0 as RRComponents
import "../../rrui" as RRUi
import "../../singletons"
import ".."

QQC2.Page {
    id: paymentByCashPage
    objectName: "paymentByCashPage"

    property real totalCost: 0
    property real amountPaid: parseFloat(amountPaidField.text === "" ? 0 : amountPaidField.text)
    property int reason: PaymentWizard.Sales
    readonly property real balance: totalCost - amountPaid
    readonly property bool hasDebt: balance > 0
    readonly property bool hasCredit: balance < 0

    padding: FluidControls.Units.smallSpacing
    title: qsTr("Enter amount paid")

    contentItem: FocusScope {
        Column {
            anchors {
                left: parent.left
                right: parent.right
            }
            spacing: FluidControls.Units.largeSpacing

            FluidControls.SubheadingLabel {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                wrapMode: Text.Wrap
                text: qsTr("The total cost is <b>%1</b>. Please enter the amount paid by the customer below.").arg(
                          Number(paymentByCashPage.totalCost).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName)))
            }

            Row {
                spacing: FluidControls.Units.smallSpacing

                FluidControls.Icon {
                    anchors.verticalCenter: parent.verticalCenter
                    source: FluidControls.Utils.iconUrl("editor/attach_money")
                    size: FluidControls.Units.iconSizes.smallMedium
                }

                RRUi.TextField {
                    id: amountPaidField
                    focus: true
                    width: 300
                    placeholderText: qsTr("Amount paid")
                    validator: RRComponents.DoubleValidator {
                        bottom: 0
                        decimals: 2
                    }
                }
            }

            FluidControls.SubheadingLabel {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                wrapMode: Text.Wrap
                color: paymentByCashPage.hasDebt
                       ? Stylesheet.debtorRed
                       : paymentByCashPage.hasCredit
                         ? Stylesheet.creditorYellow
                         : Material.theme === Material.Dark ? "white" : "black"
                text: {
                    if (paymentByCashPage.hasDebt)
                        return qsTr("You owe the %1 ").arg(paymentByCashPage.reason === PaymentWizard.Sales ? "customer" : "vendor")
                                + "<b>%2</b>.".arg(Number(Math.abs(paymentByCashPage.balance))
                                                   .toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName)));
                    else if (paymentByCashPage.hasCredit)
                        return qsTr("The %1 owes you ").arg(paymentByCashPage.reason === PaymentWizard.Sales ? "customer" : "vendor")
                                + "<b>%2</b>.".arg(Number(Math.abs(paymentByCashPage.balance))
                                                   .toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName)));

                    return qsTr("The amounts are balanced.");
                }
            }
        }
    }
}
