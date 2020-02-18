import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import com.gecko.rr.models 1.0 as RRModels
import com.gecko.rr.components 1.0 as RRComponents
import "../../rrui" as RRUi
import "../../common"
import "../../wizard"

RRUi.Page {
    id: newDebtorPage

    property RRModels.DebtorModel model: null
    property int debtorId: -1
    readonly property bool isExistingDebtor: debtorId > 0

    title: newDebtorPage.isExistingDebtor ? qsTr("Edit debtor") : qsTr("Add debtor")
    topPadding: 10
    bottomPadding: 10
    leftPadding: 20
    rightPadding: 20

    contentItem: FocusScope {
        focus: true

        RRUi.TransitionView {
            id: transitionView
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: parent.bottom
            }
            width: 800

            component: RRUi.Card {
                id: transitionItem
                padding: 0
                bottomPadding: 0

                QQC2.TabBar {
                    id: tabBar
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    currentIndex: swipeView.currentIndex

                    QQC2.TabButton { text: qsTr("Client details") }
                    QQC2.TabButton { text: qsTr("Transactions") }
                }

                QQC2.SwipeView {
                    id: swipeView
                    clip: true
                    currentIndex: tabBar.currentIndex
                    interactive: false

                    anchors {
                        top: tabBar.bottom
                        left: parent.left
                        right: parent.right
                        bottom: pageFooter.top
                    }

                    ClientDetailSubView { id: clientDetailSubView; debtorId: newDebtorPage.debtorId }
                    DebtTransactionSubView { id: debtTransactionSubView; debtorId: newDebtorPage.debtorId }
                }

                Column {
                    id: pageFooter
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                        leftMargin: 4
                        rightMargin: 4
                    }

                    spacing: 0

                    FluidControls.ThinDivider {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                    }

                    QQLayouts.RowLayout {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        QQC2.Button {
                            visible: tabBar.currentIndex === 1
                            QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                            text: qsTr("Add transaction")
                            onClicked: newDebtTransactionWizard.show();
                        }

                        QQC2.Button {
                            QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                            text: newDebtorPage.isExistingDebtor ? qsTr("Update debtor") : qsTr("Add debtor")
                            onClicked: {
                                debtTransactionSubView.listView.preferredName = clientDetailSubView.preferredName;
                                debtTransactionSubView.listView.phoneNumber = clientDetailSubView.phoneNumber;
                                debtTransactionSubView.listView.submit();
                            }
                        }
                    }
                }

                NewDebtTransactionWizard {
                    id: newDebtTransactionWizard
                    onAccepted: {
                        if (isExistingDebt)
                            debtModelData.due_date = dueDate;
                        else
                            debtTransactionSubView.listView.addDebt(amountOwed, dueDate);
                    }
                }

                AmountPaidDialog {
                    id: amountPaidDialog
                    onAccepted: {
                        if (isExistingPayment)
                            paymentModelData.amount_paid = amountPaid;
                        else
                            paymentModel.addPayment(amountPaid);
                    }
                }

                RemoveDebtConfirmationDialog {
                    id: removeDebtConfirmationDialog
                    onAccepted: {
                        if (isPayment)
                            paymentModel.removePayment(paymentIndex);
                        else
                            debtTransactionSubView.listView.removeDebt(debtIndex);
                    }
                }

                RRUi.ErrorDialog {
                    id: errorDialog
                    onAboutToHide: {
                        switch (result.code) {
                        case RRModels.DebtTransactionModel.NoPreferredNameError:
                        case RRModels.DebtTransactionModel.NoPhoneNumberError:
                            tabBar.currentIndex = 0;
                            break;
                        case RRModels.DebtTransactionModel.NoDebtError:
                        case RRModels.DebtTransactionModel.NoPaymentError:
                            tabBar.currentIndex = 1;
                            break;
                        }
                    }
                }

                Connections {
                    target: debtTransactionSubView.listView

                    onSuccess: {
                        switch (result.code) {
                        case RRModels.DebtTransactionModel.AddDebtorSuccess:
                            transitionView.trigger();
                            newDebtorPage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Debtor added successfully."));
                            break;
                        case RRModels.DebtTransactionModel.UpdateDebtorSuccess:
                            newDebtorPage.pop();
                            newDebtorPage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Debtor updated successfully."));
                            break;
                        }
                    }

                    onError: {
                        switch (result.code) {
                        case RRModels.DebtTransactionModel.NoPreferredNameError:
                            errorDialog.show(qsTr("The debtor's name must be provided."),
                                             qsTr("Error"));
                            break;
                        case RRModels.DebtTransactionModel.NoPhoneNumberError:
                            errorDialog.show(qsTr("The debtor's primary phone number must be provided."),
                                             qsTr("Error"));
                            break;
                        case RRModels.DebtTransactionModel.DataUnchangedError:
                            errorDialog.show(qsTr("No changes have been made to this debtor."),
                                             qsTr("Error"));
                            break;
                        case RRModels.DebtTransactionModel.DuplicateEntryError:
                            errorDialog.show(qsTr("This debtor already exists."));
                            break;
                        case RRModels.DebtTransactionModel.AmountOverpaidError:
                            errorDialog.show(qsTr("The amount paid by the debtor has exceeded the amount owed."));
                            break;
                        case RRModels.DebtTransactionModel.InvalidDueDateError:
                            errorDialog.show(qsTr("The due date provided is earlier than today's date. "
                                                  + "Please update all exceeded due dates (highlighted in red) and try again."));
                            break;
                        default:
                            errorDialog.show();
                        }
                    }

                    onEditTransactionRequested: newDebtTransactionWizard.show(debtModelData);

                    onRemoveTransactionRequested: removeDebtConfirmationDialog.show("Transaction #" + (debtIndex + 1),
                                                                                    debtIndex);

                    onAddPaymentRequested: amountPaidDialog.show(null, paymentModel);

                    onEditPaymentRequested: amountPaidDialog.show(paymentModelData);

                    onRemovePaymentRequested: removeDebtConfirmationDialog.show("Payment #" + (paymentIndex + 1),
                                                                                debtIndex,
                                                                                paymentIndex,
                                                                                paymentModel);
                }
            }
        }
    }
}
