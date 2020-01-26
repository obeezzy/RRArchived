import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../stock" as Stock
import "fragments/newsalepage" as Fragments
import "../common"
import "../singletons"
import "../wizard"

RRUi.Page {
    id: newSalePage
    title: qsTr("New sale entry")
    leftPadding: 10
    rightPadding: 10

    onGoBack: {
        if (privateProperties.dirty)
            event.accepted = true;
    }

    QtObject {
        id: privateProperties

        property bool dirty: false
    }

    actions: [
        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("action/pan_tool")
            toolTip: qsTr("Suspend transaction")
            text: qsTr("Suspend transaction")
            onTriggered: {
                if (transitionView.currentItem.itemCount === 0) {
                    errorDialog.show(qsTr("There are no items in your cart."),
                                     qsTr("Failed to suspend transaction"));
                } else if (transitionView.currentItem.customerName === "") {
                    errorDialog.show(qsTr("Customer name cannot be empty."),
                                     qsTr("Failed to suspend transaction"));
                } else {
                    suspendTransactionDialog.open();
                }
            }
        },

        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("content/unarchive")
            toolTip: qsTr("Load suspended transaction")
            text: qsTr("Load suspended transaction")
            onTriggered: retrieveTransactionDialog.open();
        },

        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("action/note_add")
            toolTip: qsTr("Add note")
            text: qsTr("Add note")
        },

        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("action/delete")
            toolTip: qsTr("Clear entry")
            text: qsTr("Clear entry")
            onTriggered: if (!transitionView.currentItem.cartEmpty) clearEntryConfirmationDialog.open();
        }

    ]

    Connections {
        target: MainWindow.snackBar
        onClicked: transitionView.currentItem.undoLastTransaction();
    }

    SuspendTransactionDialog {
        id: suspendTransactionDialog
        transactionId: transitionView.currentItem.transactionId
        note: transitionView.currentItem.note
        onAccepted: transitionView.currentItem.suspendTransaction( { note } );
    }

    RetrieveTransactionDialog {
        id: retrieveTransactionDialog
        transactionId: transitionView.currentItem.transactionId
        onAccepted: privateProperties.transactionId = transactionId;
    }

    RRUi.AlertDialog {
        id: clearEntryConfirmationDialog
        title: Stylesheet.padText(qsTr("Clear entry"))
        text: qsTr("Are you sure you want to clear this entry?")
        standardButtons: QQC2.Dialog.Yes | QQC2.Dialog.No
        onAccepted: transitionView.currentItem.clearCart();
    }

    RRUi.ErrorDialog { id: errorDialog }

    Stock.ItemDetailPopup {
        id: itemDetailPopup
        editButtonVisible: false
    }

    contentItem: RRUi.TransitionView {
        id: transitionView

        component: Item {
            id: saleContentItem

            readonly property int itemCount: cartSection.itemCount
            readonly property string customerName: cartSection.customerName
            readonly property string customerPhoneNumber: cartSection.customerPhoneNumber
            readonly property int transactionId: cartSection.transactionId
            readonly property string note: cartSection.note
            readonly property bool cartEmpty: cartSection.cartEmpty
            readonly property alias cartModel: cartSection.model

            function clearCart() {
                cartSection.clear();
                MainWindow.snackBar.show(qsTr("Entry cleared."));
            }

            function validateUserInput() {
                if (cartSection.customerName.trim() === "") {
                    errorDialog.show(qsTr("Customer name cannot be empty."),
                                     qsTr("Failed to complete transaction"));
                    return false;
                } else if (cartSection.cartEmpty) {
                    errorDialog.show(qsTr("There are no items in your cart."),
                                     qsTr("Failed to complete transaction"));
                    return false;
                }

                return true;
            }

            function suspendTransaction(params) { cartSection.suspendTransaction(params); }
            function undoLastTransaction() { cartSection.undoLastTransaction(); }
            function submitTransaction(params) { cartSection.submitTransaction(params); }

            QQLayouts.RowLayout {
                anchors.fill: parent

                Fragments.ItemSelectionSection {
                    id: itemSelectionSection
                    QQLayouts.Layout.fillWidth: true
                    QQLayouts.Layout.fillHeight: true

                    padding: 20
                    bottomPadding: 0
                    Material.elevation: 2

                    onAddRequested: cartSection.addItem(item);
                    onViewRequested: itemDetailPopup.show(itemId);
                }

                Fragments.CartSection {
                    id: cartSection
                    QQLayouts.Layout.leftMargin: 4
                    QQLayouts.Layout.fillHeight: true
                    onSuccess: {
                        if (paymentWizard.opened)
                            paymentWizard.accept();

                        itemSelectionSection.refresh();
                    }
                    onError: {
                        if (!paymentWizard.opened) {
                            switch (errorCode) {
                            default:
                                errorDialog.show(qsTr("An unknown error has occurred."),
                                                 qsTr("Failed to save transaction"));
                                break;
                            }
                        }
                    }

                    onViewRequested: itemDetailPopup.show(itemId);
                    onCheckoutRequested: if (saleContentItem.validateUserInput()) paymentWizard.open();
                }
            }
        }
    }

    PaymentWizard {
        id: paymentWizard
        reason: PaymentWizard.Sales
        cartModel: transitionView.currentItem.cartModel
        onAccepted: {
            console.log(`Due date=${paymentWizard.dueDate}, action=${paymentWizard.action}`);
            transitionView.currentItem.submitTransaction({ "due_date": paymentWizard.dueDate,
                                                                     "action": paymentWizard.action });
        }
    }
}
