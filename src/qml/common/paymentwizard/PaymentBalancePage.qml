import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import "../../singletons"

QQC2.Page {
    id: paymentBalancePage
    objectName: "paymentBalancePage"

    property string selectedOption: repeater.model.get(repeater.currentIndex).option
    property real totalCost: 0
    property real balance: 0
    property bool hasDebt: false
    property bool hasCredit: false
    property bool canAcceptAlternatePaymentMethod: false
    property bool isCashPayment: false

    title: qsTr("Handle %1").arg(hasCredit ? "credit" : "debt")
    padding: FluidControls.Units.smallSpacing

    contentItem: FocusScope {
        QQC2.ButtonGroup { id: buttonGroup }

        Column {
            anchors {
                left: parent.left
                right: parent.right
            }

            spacing: FluidControls.Units.smallSpacing

            FluidControls.SubheadingLabel {
                anchors {
                    left: parent.left
                    right: parent.right
                }

                text: {
                    if (paymentBalancePage.hasCredit)
                        return qsTr("You owe the customer <b>%1</b>. How would you like to handle this?")
                    .arg(Number(Math.abs(paymentBalancePage.balance))
                         .toLocaleCurrencyString(Qt.locale(GlobalSettings.localeName)));

                    return qsTr("The customer owes you <b>%1</b>. How would you like to handle this?")
                    .arg(Number(Math.abs(paymentBalancePage.balance))
                         .toLocaleCurrencyString(Qt.locale(GlobalSettings.localeName)));
                }

                wrapMode: Text.Wrap
            }

            Column {
                id: modelColumn
                anchors {
                    left: parent.left
                    right: parent.right
                }
                Repeater {
                    id: repeater
                    property int currentIndex: 0

                    model: hasCredit ? creditorOptionModel : debtorOptionModel

                    delegate: QQC2.RadioButton {
                        id: radioButton
                        width: modelColumn.width
                        text: model.text
                        checked: index === 0
                        QQC2.ButtonGroup.group: buttonGroup

                        contentItem: FluidControls.BodyLabel {
                            text: radioButton.text
                            font: radioButton.font
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: radioButton.indicator.width + radioButton.spacing
                            wrapMode: Text.Wrap
                        }

                        onClicked: repeater.currentIndex = index;
                    }
                }
            }
        }

        DebtorOptionModel {
            id: debtorOptionModel
            canAcceptAlternatePaymentMethod: paymentBalancePage.canAcceptAlternatePaymentMethod
        }

        CreditorOptionModel {
            id: creditorOptionModel
            canAcceptAlternatePaymentMethod: paymentBalancePage.canAcceptAlternatePaymentMethod
        }
    }
}
