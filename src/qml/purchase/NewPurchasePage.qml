import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../stock" as Stock
import "../common"
import "../singletons"

RRUi.Page {
    id: newPurchasePage
    title: qsTr("New purchase entry")
    leftPadding: 10
    rightPadding: 10

    onGoBack: {
        if (transitionView.currentItem.isDirty) {
            leaveConfirmationDialog.open();
            event.accepted = true;
        }
    }

    QtObject {
        id: privateProperties

        property int filterIndex: 0
        property int sortIndex: 0
        property var filterModel: ["Search by item name", "Search by category name"]
        property var sortModel: ["Sort in ascending order", "Sort in descending order"]

        property int transactionId: -1
    }

    actions: [
        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("action/pan_tool")
            toolTip: qsTr("Suspend transaction")
            text: qsTr("Suspend transaction")
            onTriggered: {
                if (transitionView.currentItem.itemCount === 0) {
                    errorDialog.show(qsTr("There are no items in your cart."), qsTr("Failed to suspend transaction"));
                } else if (transitionView.currentItem.customerName === "") {
                    errorDialog.show(qsTr("Customer name cannot be empty."), qsTr("Failed to suspend transaction"));
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
            icon.source: FluidControls.Utils.iconUrl("action/print")
            toolTip: qsTr("Print last entry")
            text: qsTr("Print last entry")
        },

        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("action/delete")
            toolTip: qsTr("Clear entry")
            text: qsTr("Clear entry")
            onTriggered: if (!transitionView.currentItem.isCartEmpty) clearEntryConfirmationDialog.open();
        }
    ]

    contentItem: RRUi.TransitionView {
        id: transitionView

        component: Item {
            id: purchaseContentItem

            readonly property int itemCount: cartListView.count
            readonly property string customerName: customerNameField.text
            readonly property string customerPhoneNumber: customerPhoneNumberField.text
            readonly property int transactionId: cartListView.transactionId
            readonly property string note: cartListView.note
            readonly property bool isCartEmpty: cartListView.count === 0
            readonly property bool isDirty: !isCartEmpty

            function clearCart() {
                cartListView.clearAll();
                privateProperties.transactionId = -1;
                customerNameField.clear();
                newPurchasePage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Entry cleared."));
            }

            function validateUserInput() {
                if (customerNameField.text.trim() === "") {
                    errorDialog.show(qsTr("Customer name cannot be empty."), qsTr("Failed to complete transaction"));
                    return false;
                } else if (cartListView.count == 0) {
                    errorDialog.show(qsTr("There are no items in your cart."), qsTr("Failed to complete transaction"));
                    return false;
                }

                return true;
            }

            function suspendTransaction(params) { cartListView.suspendTransaction(params); }

            RRUi.Card {
                id: stockItemCard
                width: parent.width * .66 - 8
                anchors {
                    left: parent.left
                    top: parent.top
                    bottom: parent.bottom
                }

                padding: 20
                bottomPadding: 0
                Material.elevation: 2

                FocusScope {
                    anchors.fill: parent

                    RRUi.SearchBar {
                        id: searchBar
                        anchors {
                            top: parent.top
                            left: parent.left
                            right: parent.right
                        }
                    }

                    RRUi.ChipListView {
                        id: filterChipListView
                        height: 30
                        anchors {
                            top: searchBar.bottom
                            left: parent.left
                            right: parent.right
                        }

                        model: [
                            privateProperties.filterModel[privateProperties.filterIndex],
                            privateProperties.sortModel[privateProperties.sortIndex]
                        ]

                        onClicked: {
                            switch (index) {
                            case 0:
                                filterColumnDialog.open();
                                break;
                            case 1:
                                sortOrderDialog.open();
                                break;
                            }
                        }
                    }

                    Stock.CategoryListView {
                        id: categoryListView
                        anchors {
                            top: filterChipListView.bottom
                            left: parent.left
                            right: parent.right
                            bottom: parent.bottom
                        }

                        filterText: searchBar.text
                        filterColumn: RRModels.StockItemModel.ItemColumn

                        buttonRow: Row {
                            spacing: 0

                            RRUi.ToolButton {
                                id: addToCartButton
                                icon.source: FluidControls.Utils.iconUrl("action/add_shopping_cart")
                                text: qsTr("Add to cart")
                                visible: parent.parent.modelData.quantity > 0
                                onClicked: cartListView.addItem(parent.parent.modelData);
                            }

                            RRUi.ToolButton {
                                id: viewButton
                                icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                                text: qsTr("View details")
                                onClicked: itemDetailPopup.show(parent.parent.modelData.item_id);
                            }
                        }
                    }

                    FluidControls.Placeholder {
                        visible: categoryListView.count == 0 && categoryListView.model.filterText !== ""
                        anchors.centerIn: parent
                        icon.source: FluidControls.Utils.iconUrl("action/search")
                        text: qsTr("There are no results for this search query.")
                    }

                    FluidControls.Placeholder {
                        visible: categoryListView.count == 0 && categoryListView.model.filterText === ""
                        anchors.centerIn: parent
                        icon.source: Qt.resolvedUrl("qrc:/icons/truck.svg")
                        text: qsTr("No products available.")
                    }
                }
            }

            FocusScope {
                width: parent.width *.33
                anchors {
                    left: stockItemCard.right
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                    leftMargin: 4
                }

                QQLayouts.ColumnLayout {
                    anchors.fill: parent

                    RRUi.Card {
                        id: customerInfoCard

                        QQLayouts.Layout.fillWidth: true
                        QQLayouts.Layout.preferredHeight: textFieldColumn.height

                        padding: 0
                        leftPadding: 4
                        rightPadding: 4

                        Column {
                            id: textFieldColumn
                            anchors {
                                top: parent.top
                                left: parent.left
                                right: parent.right
                                topMargin: 0
                            }

                            QQLayouts.RowLayout {
                                spacing: 2
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }

                                RRUi.IconTextField {
                                    id: customerNameField
                                    QQLayouts.Layout.fillWidth: true
                                    focus: true
                                    icon.source: FluidControls.Utils.iconUrl("social/person")
                                    textField.placeholderText: qsTr("Customer name")

                                    Connections {
                                        target: customerNameField.textField
                                        onTextEdited: cartListView.customerName = customerNameField.text;
                                    }
                                }

                                RRUi.ToolButton {
                                    id: customerOptionButton
                                    icon.source: FluidControls.Utils.iconUrl("navigation/more_vert")
                                    text: qsTr("More")
                                }
                            }

                            RRUi.IconTextField {
                                id: customerPhoneNumberField
                                visible: false
                                width: 300
                                icon.source: FluidControls.Utils.iconUrl("communication/phone")
                                textField.placeholderText: qsTr("Customer phone number")
                            }
                        }
                    }

                    RRUi.Card {
                        id: cartCard

                        QQLayouts.Layout.fillWidth: true
                        QQLayouts.Layout.fillHeight: true

                        padding: 0
                        leftPadding: 8
                        rightPadding: 8
                        Material.elevation: 2

                        CartListView {
                            id: cartListView
                            anchors.fill: parent
                            customerName: customerNameField.text
                            transactionId: privateProperties.transactionId

                            onViewRequested: itemDetailPopup.show(itemId);
                            onEditRequested: cartItemEditorDialog.show(itemInfo);

                            onSuccess: {
                                if (paymentWizard.opened)
                                    paymentWizard.accept();

                                searchBar.clear();
                                categoryListView.refresh();

                                switch (successCode) {
                                case RRModels.PurchaseCartModel.RetrieveTransactionSuccess:
                                    customerNameField.text = cartListView.customerName;
                                    newPurchasePage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Transaction retrieved."));
                                    break;
                                case RRModels.PurchaseCartModel.SuspendTransactionSuccess:
                                    customerNameField.clear();
                                    privateProperties.transactionId = -1;
                                    newPurchasePage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Transaction suspended."), qsTr("Undo"));
                                    break;
                                case RRModels.PurchaseCartModel.SubmitTransactionSuccess:
                                    customerNameField.clear();
                                    privateProperties.transactionId = -1;
                                    newPurchasePage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Transaction submitted."), qsTr("Undo"));
                                    break;
                                case RRModels.PurchaseCartModel.UndoSubmitTransactionSuccess:
                                    newPurchasePage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Undo successul."));
                                    break;
                                }
                            }
                            onError: {
                                var errorString = "";
                                switch (errorCode) {
                                case CartListView.ConnectionError:
                                    errorString = qsTr("Failed to connect to the database.");
                                    break;
                                case CartListView.SuspendTransactionError:
                                    errorString = qsTr("Failed to suspend transaction.");
                                    break;
                                case CartListView.SubmitTransactionError:
                                    errorString = qsTr("Failed to submit transaction.");
                                    break;
                                case CartListView.RetrieveTransactionError:
                                    errorString = qsTr("Failed to retrieve transaction.");
                                    break;
                                case CartListView.SuspendTransactionError:
                                    errorString = qsTr("Failed to suspend transaction.");
                                    break;
                                case CartListView.UnknownError:
                                    errorString = qsTr("An unknown error occurred.");
                                    break;
                                }

                                if (paymentWizard.opened) {
                                    paymentWizard.displayError(errorString);
                                } else {
                                    switch (errorCode) {
                                    default:
                                        errorDialog.show(qsTr("An unknown error has occurred."), qsTr("Failed to save transaction"), errorCode);
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    RRUi.Card {
                        id: checkoutCard

                        QQLayouts.Layout.fillWidth: true
                        QQLayouts.Layout.preferredHeight: totalsColumn.height

                        padding: 2
                        bottomPadding: 0

                        Column {
                            id: totalsColumn
                            anchors {
                                left: parent.left
                                right: parent.right
                            }

                            FluidControls.SubheadingLabel {
                                id: totalCostLabel
                                anchors.right: parent.right
                                text: qsTr("Total cost: %1").arg(Number(cartListView.totalCost).toLocaleCurrencyString(Qt.locale("en_NG")))
                            }

                            QQC2.Button {
                                id: checkoutButton
                                anchors.right: parent.right
                                text: qsTr("Proceed to Checkout")
                                onClicked: if (purchaseContentItem.validateUserInput()) paymentWizard.open();
                            }
                        }
                    }
                }

                RRUi.BusyOverlay { visible: cartListView.busy }

                PaymentWizard {
                    id: paymentWizard
                    reason: PaymentWizard.Purchase
                    cartModel: cartListView.model
                    onAccepted: cartListView.submitTransaction({ "due_date": paymentWizard.dueDate,
                                                                   "action": paymentWizard.action });
                }

                RRUi.RadioButtonDialog {
                    id: filterColumnDialog
                    title: qsTr("Choose filter sort")
                    model: privateProperties.filterModel
                    currentIndex: privateProperties.filterIndex
                    onAccepted: {
                        privateProperties.filterIndex = currentIndex;
                        filterChipListView.model = [
                                    privateProperties.filterModel[privateProperties.filterIndex],
                                    privateProperties.sortModel[privateProperties.sortIndex]
                                ];
                    }
                }

                RRUi.RadioButtonDialog {
                    id: sortOrderDialog
                    title: qsTr("Choose sort order")
                    model: privateProperties.sortModel
                    currentIndex: privateProperties.sortIndex
                    onAccepted: {
                        privateProperties.sortIndex = currentIndex;
                        filterChipListView.model = [
                                    privateProperties.filterModel[privateProperties.filterIndex],
                                    privateProperties.sortModel[privateProperties.sortIndex]
                                ];
                    }
                }

                CartItemEditorDialog {
                    id: cartItemEditorDialog
                    onAccepted: cartListView.updateItem(itemId, { "quantity": quantity,
                                                            "unit_price": unitPrice,
                                                            "cost": cost });
                }
            }

            Connections {
                target: newPurchasePage.RRUi.ApplicationWindow.window.snackBar !== undefined ?
                            newPurchasePage.RRUi.ApplicationWindow.window.snackBar : null
                onClicked: cartListView.undoLastTransaction();
            }
        }
    }

    SuspendTransactionDialog {
        id: suspendTransactionDialog
        transactionId: transitionView.currentItem.transactionId
        note: transitionView.currentItem.note
        onAccepted: transitionView.currentItem.suspendTransaction( { "note": note } );
    }

    RetrieveTransactionDialog {
        id: retrieveTransactionDialog
        onAccepted: privateProperties.transactionId = transactionId;
    }

    RRUi.AlertDialog {
        id: clearEntryConfirmationDialog
        text: qsTr("Clear entry?")
        standardButtons: QQC2.Dialog.Yes | QQC2.Dialog.No
        onAccepted: transitionView.currentItem.clearCart();

        FluidControls.BodyLabel { text: qsTr("Are you sure you want to clear this entry?") }
    }

    RRUi.AlertDialog {
        id: leaveConfirmationDialog
        text: qsTr("Leave?")
        standardButtons: QQC2.Dialog.Yes | QQC2.Dialog.No
        onAccepted: newPurchasePage.forcePop();

        FluidControls.BodyLabel { text: qsTr("Are you sure you want to leave?") }
    }

    RRUi.ErrorDialog { id: errorDialog }

    Stock.ItemDetailPopup {
        id: itemDetailPopup
        editButtonVisible: false
    }
}
