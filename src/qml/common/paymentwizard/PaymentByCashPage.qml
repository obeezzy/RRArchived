import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.components 1.0 as RRComponents
import "../../rrui" as RRUi
import "../../singletons"

QQC2.Page {
    id: paymentByCashPage
    objectName: "paymentByCashPage"

    property real totalCost: 0
    property real amountPaid: parseFloat(amountPaidField.text === "" ? 0 : amountPaidField.text)
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
                text: qsTr("The total cost is <b>%1</b>. Please enter the amount paid by the customer below.")
                        .arg(Number(paymentByCashPage.totalCost).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName)))
            }

            Row {
                spacing: FluidControls.Units.smallSpacing

                FluidControls.Icon {
                    anchors.verticalCenter: parent.verticalCenter
                    name: "editor/attach_money"
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
                color: paymentByCashPage.balance < 0 ? Stylesheet.creditorYellow : paymentByCashPage.balance > 0 ? Stylesheet.debtorRed : "black"
                text: {
                    if (paymentByCashPage.balance < 0)
                        return qsTr("You owe the customer ")
                                + "<b>%1</b>.".arg(Number(Math.abs(paymentByCashPage.balance))
                                                   .toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName)));
                    else if (paymentByCashPage.balance > 0)
                        return qsTr("The customer owes you ")
                                + "<b>%1</b>.".arg(Number(Math.abs(paymentByCashPage.balance))
                                                   .toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName)));

                    return qsTr("The amounts are balanced.");
                }
            }
        }
    }
}
