import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import "../../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import "../../singletons"
import ".."

RRUi.WizardPage {
    id: paymentBalancePage
    objectName: "paymentBalancePage"

    property string selectedOption: repeater.model.get(repeater.currentIndex).option
    property real totalCost: 0
    property real balance: 0
    property bool hasDebt: false
    property bool hasCredit: false
    property bool canAcceptAlternatePaymentMethod: false
    property bool isCashPayment: false
    property RRModels.SaleCartModel cartModel: null

    title: qsTr("Handle %1").arg(hasCredit ? "credit" : "debt")
    padding: FluidControls.Units.smallSpacing
    hasNext: true

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

    onNext: {
        switch (paymentBalancePage.selectedOption) {
        case "pay_another_way":
            if (paymentBalancePage.cartModel.canAcceptCard || paymentBalancePage.cartModel.canAcceptCash) {
                paymentBalancePage.QQC2.StackView.view.wizard.action = paymentBalancePage.selectedOption;
                paymentBalancePage.nextPage.component = Qt.resolvedUrl("PaymentMethodPage.qml");
                paymentBalancePage.nextPage.properties = {
                    "canAcceptCash": paymentBalancePage.cartModel.canAcceptCash,
                    "canAcceptCard": paymentBalancePage.cartModel.canAcceptCard
                };
            }
            break;
        case "overlook_balance":
            paymentBalancePage.QQC2.StackView.view.wizard.action = paymentBalancePage.selectedOption;
            paymentBalancePage.nextPage.component = Qt.resolvedUrl("PaymentFinishPage.qml");
            break;
        case "give_change":
            paymentBalancePage.QQC2.StackView.view.wizard.action = paymentBalancePage.selectedOption;
            paymentBalancePage.nextPage.component = Qt.resolvedUrl("PaymentChangePage.qml");
            paymentBalancePage.nextPage.properties = { "changeDue": paymentBalancePage.balance };
            break;
        case "create_debtor":
            paymentBalancePage.QQC2.StackView.view.wizard.action = paymentBalancePage.selectedOption;
            if (paymentBalancePage.cartModel.customerName.trim() === ""
                    || paymentBalancePage.cartModel.customerPhoneNumber.trim() === "") {
                paymentBalancePage.nextPage.component = Qt.resolvedUrl("PaymentCustomerDetailPage.qml");
                paymentBalancePage.nextPage.properties = {
                    "customerName": paymentBalancePage.cartModel.customerName,
                    "customerPhoneNumber": paymentBalancePage.cartModel.customerPhoneNumber,
                    "paymentModel": paymentBalancePage.cartModel.paymentModel,
                    "cartModel": paymentBalancePage.cartModel
                };
            }
            else {
                paymentBalancePage.nextPage.component = Qt.resolvedUrl("PaymentDueDatePage.qml");
                paymentBalancePage.nextPage.properties = {
                    "cartModel": paymentBalancePage.cartModel
                }
            }
            break;
        case "create_creditor":
            paymentBalancePage.QQC2.StackView.view.wizard.action = paymentBalancePage.selectedOption;
            if (paymentBalancePage.cartModel.customerName.trim() === ""
                    || paymentBalancePage.cartModel.customerPhoneNumber.trim() === "") {
                paymentBalancePage.nextPage.component = Qt.resolvedUrl("PaymentCustomerDetailPage.qml");
                paymentBalancePage.nextPage.properties = {
                    "customerName": paymentBalancePage.cartModel.customerName,
                    "customerPhoneNumber": paymentBalancePage.cartModel.customerPhoneNumber,
                    "paymentModel": paymentBalancePage.cartModel.paymentModel,
                    "cartModel": paymentBalancePage.cartModel
                };
            } else {
                paymentBalancePage.component = Qt.resolvedUrl("PaymentDueDatePage.qml");
                paymentBalancePage.nextPage.properties = {
                    "cartModel": paymentBalancePage.cartModel
                }
            }
            break;
        }
    }
}
