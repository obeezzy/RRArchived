import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Layouts 1.3 as QQLayouts
import "../rrui" as RRUi
import com.gecko.rr.widgets 1.0 as RRWidgets
import com.gecko.rr.models 1.0 as RRModels
import "paymentwizard"

RRUi.Dialog {
    id: paymentWizard

    property var cartModel: null
    property string action: ""
    property date dueDate: new Date()
    property int reason: PaymentWizard.Sales

    enum Reason {
        Sales,
        Purchase
    }

    title: ""
    standardButtons: RRUi.Dialog.NoButton
    width: 600
    height: 540
    closePolicy: QQC2.Popup.CloseOnEscape

    QtObject {
        id: privateProperties

        property string previouslySetCustomerName: ""
        property string previouslySetCustomerPhoneNumber: ""

        function resetCustomerDetails() {
            cartModel.customerName = privateProperties.previouslySetCustomerName;
            cartModel.customerPhoneNumber = privateProperties.previouslySetCustomerPhoneNumber;
        }
    }

    onClosed: {
        privateProperties.resetCustomerDetails();

        paymentWizard.cartModel.clearPayments();
        stackView.clear();
    }

    onAboutToShow: {
        privateProperties.previouslySetCustomerName = cartModel.customerName;
        privateProperties.previouslySetCustomerPhoneNumber = cartModel.customerPhoneNumber;

        stackView.replace(null, Qt.resolvedUrl("paymentwizard/PaymentMethodPage.qml"),
                          { "canAcceptCash": paymentWizard.cartModel.canAcceptCash,
                              "canAcceptCard": paymentWizard.cartModel.canAcceptCard });
    }

    QQLayouts.ColumnLayout {
        anchors.fill: parent

        Row {
            QQLayouts.Layout.preferredHeight: 25

            RRUi.ToolButton {
                anchors.verticalCenter: parent.verticalCenter
                width: FluidControls.Units.iconSizes.large
                height: width
                icon.source: FluidControls.Utils.iconUrl("navigation/arrow_back")
                text: qsTr("Back")
                visible: stackView.depth > 1 && stackView.currentObjectName !== "paymentFinishPage"
                onClicked: {
                    if (stackView.currentObjectName === "paymentCustomerDetailPage")
                        privateProperties.resetCustomerDetails();

                    stackView.pop();

                    if (stackView.currentObjectName === "paymentByCashPage")
                        paymentWizard.cartModel.removePayment(paymentWizard.cartModel.count - 1);
                    else if (stackView.currentObjectName === "paymentByCardPage")
                        paymentWizard.cartModel.removePayment(paymentWizard.cartModel.count - 1);
                }
            }

            FluidControls.DialogLabel {
                id: dialogLabel
                anchors.verticalCenter: parent.verticalCenter

                wrapMode: Text.Wrap
                text: stackView.currentItem !== null ? stackView.currentItem.title : ""
            }
        }

        RRUi.StackView {
            id: stackView

            QQLayouts.Layout.fillWidth: true
            QQLayouts.Layout.fillHeight: true
            clip: true
        }

        Row {
            spacing: 8
            QQLayouts.Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

            QQC2.CheckBox {
                id: printCheckBox
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Print receipt")
                visible: stackView.currentObjectName === "paymentSummaryPage"
            }

            QQC2.Button {
                flat: true
                text: qsTr("Cancel")
                onClicked: paymentWizard.reject();
            }

            QQC2.Button {
                visible: stackView.currentObjectName != "paymentFinishPage"
                enabled: stackView.currentObjectName != "paymentCustomerDetailPage"
                         || (stackView.currentObjectName == "paymentCustomerDetailPage"
                             && stackView.currentItem.detailValid)
                text: {
                    if (stackView.currentObjectName == "paymentMethodPage") {
                        switch (stackView.currentItem.selectedOption) {
                        case "cash":
                        case "none":
                            return qsTr("Next");
                        case "card":
                            return qsTr("Pay");
                        }
                    } else if (stackView.currentObjectName == "paymentByCashPage") {
                        return qsTr("Pay");
                    } else if (stackView.currentObjectName == "paymentBalancePage") {
                        return qsTr("Next");
                    } else if (stackView.currentObjectName == "paymentCustomerDetailPage") {
                        return qsTr("Next");
                    } else if (stackView.currentObjectName == "paymentDueDatePage") {
                        return qsTr("Next");
                    }

                    return qsTr("Finish");
                }

                onClicked: {
                    if (stackView.currentObjectName == "paymentMethodPage") {
                        switch (stackView.currentItem.selectedOption) {
                        case "cash":
                            stackView.push(Qt.resolvedUrl("paymentwizard/PaymentByCashPage.qml"),
                                           { "totalCost": paymentWizard.cartModel.balance, "reason": paymentWizard.reason });
                            break;
                        case "card":
                            console.warn("TODO: Handle card payment!");
                            break;
                        default:
                            stackView.push(Qt.resolvedUrl("paymentwizard/PaymentCustomerDetailPage.qml"), {
                                               "customerName": paymentWizard.cartModel.customerName,
                                               "customerPhoneNumber": paymentWizard.cartModel.customerPhoneNumber,
                                               "paymentModel": paymentWizard.cartModel.paymentModel
                                           });
                            break;
                        }
                    } else if (stackView.currentObjectName == "paymentByCashPage") {
                        paymentWizard.cartModel.addPayment(stackView.currentItem.amountPaid, RRModels.SaleCartModel.Cash);

                        if (stackView.currentItem.hasDebt || stackView.currentItem.hasCredit) {
                            stackView.push(Qt.resolvedUrl("paymentwizard/PaymentBalancePage.qml"), {
                                               "hasDebt": stackView.currentItem.hasDebt,
                                               "hasCredit": stackView.currentItem.hasCredit,
                                               "balance": paymentWizard.cartModel.balance,
                                               "canAcceptAlternatePaymentMethod": paymentWizard.cartModel.canAcceptCash
                                                                                  || paymentWizard.canAcceptCard,
                                               "isCashPayment": true,
                                               "totalCost": paymentWizard.cartModel.totalCost
                                           });
                        } else if ((paymentWizard.cartModel.customerName.trim() === ""
                                    || paymentWizard.cartModel.customerPhoneNumber.trim() === "")
                                   && (stackView.currentItem.hasDebt || stackView.currentItem.hasCredit)) {
                            stackView.push(Qt.resolvedUrl("paymentwizard/PaymentCustomerDetailPage.qml"), {
                                               "customerName": paymentWizard.cartModel.customerName,
                                               "customerPhoneNumber": paymentWizard.cartModel.customerPhoneNumber
                                           });
                        } else {
                            stackView.push(Qt.resolvedUrl("paymentwizard/PaymentSummaryPage.qml"), {
                                               "customerName": paymentWizard.cartModel.customerName,
                                               "customerPhoneNumber": paymentWizard.cartModel.customerPhoneNumber,
                                               "totalCost": paymentWizard.cartModel.totalCost,
                                               "amountPaid": paymentWizard.cartModel.amountPaid,
                                               "paymentModel": paymentWizard.cartModel.paymentModel
                                           });
                        }
                    } else if (stackView.currentObjectName == "paymentByCardPage") {
                        paymentWizard.cartModel.addPayment(stackView.currentItem.amountPaid,
                                                           stackView.currentItem.paymentMethod === "debit_card"
                                                           ? RRModels.SaleCartModel.DebitCard : RRModels.SaleCartModel.CreditCard);

                        if (stackView.currentItem.hasDebt || stackView.currentItem.hasCredit) {
                            stackView.push(Qt.resolvedUrl("paymentwizard/PaymentBalancePage.qml"), {
                                               "hasDebt": stackView.currentItem.hasDebt,
                                               "hasCredit": stackView.currentItem.hasCredit,
                                               "balance": paymentWizard.cartModel.balance,
                                               "canAcceptAlternatePaymentMethod": paymentWizard.cartModel.canAcceptCash
                                                                                  || paymentWizard.canAcceptCard,
                                               "isCashPayment": false,
                                               "totalCost": paymentWizard.cartModel.totalCost
                                           });
                        } else if (paymentWizard.cartModel.customerName.trim() === ""
                                   || paymentWizard.cartModel.customerPhoneNumber.trim() === "") {
                            stackView.push(Qt.resolvedUrl("paymentwizard/PaymentCustomerDetailPage.qml"), {
                                               "customerName": paymentWizard.cartModel.customerName,
                                               "customerPhoneNumber": paymentWizard.cartModel.customerPhoneNumber,
                                               "paymentModel": paymentWizard.cartModel.paymentModel
                                           });
                        } else {
                            stackView.push(Qt.resolvedUrl("paymentwizard/PaymentSummaryPage.qml"), {
                                               "customerName": paymentWizard.cartModel.customerName,
                                               "customerPhoneNumber": paymentWizard.cartModel.customerPhoneNumber,
                                               "totalCost": paymentWizard.cartModel.totalCost,
                                               "amountPaid": paymentWizard.cartModel.amountPaid,
                                               "paymentModel":paymentWizard.cartModel.paymentModel
                                           });
                        }
                    } else if (stackView.currentObjectName == "paymentBalancePage") {
                        switch (stackView.currentItem.selectedOption) {
                        case "pay_another_way":
                            if (paymentWizard.cartModel.canAcceptCard || paymentWizard.cartModel.canAcceptCash) {
                                paymentWizard.action = stackView.currentItem.selectedOption;
                                stackView.push(Qt.resolvedUrl("paymentwizard/PaymentMethodPage.qml"), {
                                                   "canAcceptCash": paymentWizard.cartModel.canAcceptCash,
                                                   "canAcceptCard": paymentWizard.cartModel.canAcceptCard
                                               });
                            }
                            break;
                        case "overlook_balance":
                            paymentWizard.action = stackView.currentItem.selectedOption;
                            stackView.push(Qt.resolvedUrl("paymentwizard/PaymentFinishPage.qml"));
                            break;
                        case "give_change":
                            paymentWizard.action = stackView.currentItem.selectedOption;
                            stackView.push(Qt.resolvedUrl("paymentwizard/PaymentChangePage.qml"),
                                           { "changeDue": stackView.currentItem.balance });
                            break;
                        case "create_debtor":
                            paymentWizard.action = stackView.currentItem.selectedOption;
                            if (paymentWizard.cartModel.customerName.trim() === ""
                                    || paymentWizard.cartModel.customerPhoneNumber.trim() === "")
                                stackView.push(Qt.resolvedUrl("paymentwizard/PaymentCustomerDetailPage.qml"), {
                                                   "customerName": paymentWizard.cartModel.customerName,
                                                   "customerPhoneNumber": paymentWizard.cartModel.customerPhoneNumber,
                                                   "paymentModel": paymentWizard.cartModel.paymentModel
                                               });
                            else
                                stackView.push(Qt.resolvedUrl("paymentwizard/PaymentDueDatePage.qml"));
                            break;
                        case "create_creditor":
                            paymentWizard.action = stackView.currentItem.selectedOption;
                            if (paymentWizard.cartModel.customerName.trim() === ""
                                    || paymentWizard.cartModel.customerPhoneNumber.trim() === "")
                                stackView.push(Qt.resolvedUrl("paymentwizard/PaymentCustomerDetailPage.qml"), {
                                                   "customerName": paymentWizard.cartModel.customerName,
                                                   "customerPhoneNumber": paymentWizard.cartModel.customerPhoneNumber,
                                                   "paymentModel": paymentWizard.cartModel.paymentModel
                                               });
                            else
                                stackView.push(Qt.resolvedUrl("paymentwizard/PaymentDueDatePage.qml"));
                            break;
                        }
                    } else if (stackView.currentObjectName == "paymentCustomerDetailPage") {
                        paymentWizard.cartModel.customerName = stackView.currentItem.customerName;
                        paymentWizard.cartModel.customerPhoneNumber = stackView.currentItem.customerPhoneNumber;

                        if (paymentWizard.cartModel.balance !== 0)
                            stackView.push(Qt.resolvedUrl("paymentwizard/PaymentDueDatePage.qml"));
                        else
                            stackView.push(Qt.resolvedUrl("paymentwizard/PaymentSummaryPage.qml"), {
                                               "customerName": paymentWizard.cartModel.customerName,
                                               "customerPhoneNumber": paymentWizard.cartModel.customerPhoneNumber,
                                               "totalCost": paymentWizard.cartModel.totalCost,
                                               "amountPaid": paymentWizard.cartModel.amountPaid,
                                               "paymentModel": paymentWizard.cartModel.paymentModel
                                           });
                    } else if (stackView.currentObjectName == "paymentDueDatePage") {
                        paymentWizard.dueDate = stackView.currentItem.dueDate;
                        stackView.push(Qt.resolvedUrl("paymentwizard/PaymentSummaryPage.qml"), {
                                           "customerName": paymentWizard.cartModel.customerName,
                                           "customerPhoneNumber": paymentWizard.cartModel.customerPhoneNumber,
                                           "totalCost": paymentWizard.cartModel.totalCost,
                                           "amountPaid": paymentWizard.cartModel.amountPaid,
                                           "paymentModel": paymentWizard.cartModel.paymentModel
                                       });
                    } else if (stackView.currentObjectName == "paymentSummaryPage" || stackView.currentObjectName == "paymentChangePage") {
                        if (printCheckBox.checked)
                            RRWidgets.Dialogs.showPrintDialog();
                        stackView.push(Qt.resolvedUrl("paymentwizard/PaymentFinishPage.qml"));
                        paymentWizard.accept();
                    } else if (stackView.currentObjectName == "paymentFinishPage") {
                    }
                }
            }
        }
    }

    function displayError(message) {
        if (stackView.currentObjectName === "paymentFinishPage") {
            stackView.currentItem.errorMessage = message;
        }
    }
}
