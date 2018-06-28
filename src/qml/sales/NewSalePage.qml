import QtQuick 2.10
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../stock" as Stock
import "../common"
import "../singletons"

RRUi.Page {
    id: newSalePage
    title: qsTr("New sale entry")
    leftPadding: 10
    rightPadding: 10

    /*!
        \qmlsignal void goBack(var event)

        This signal is emitted when the back action is triggered or back key is released.

        By default, the page will be popped from the page stack. To change the default
        behavior, for example to show a confirmation dialog, listen for this signal using
        \c onGoBack and set \c event.accepted to \c true. To dismiss the page from your
        dialog without triggering this signal and re-showing the dialog, call
        \c page.forcePop().
    */

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
                if (animationStackView.currentItem.itemCount === 0) {
                    failureAlertDialogLoader.title = qsTr("Failed to suspend transaction");
                    failureAlertDialogLoader.message = qsTr("There are no items in your cart.               ");
                    failureAlertDialogLoader.create();
                } else if (animationStackView.currentItem.customerName === "") {
                    failureAlertDialogLoader.title = qsTr("Failed to suspend transaction");
                    failureAlertDialogLoader.message = qsTr("Customer name cannot be empty.               ");
                    failureAlertDialogLoader.create();
                } else {
                    suspendTransactionDialogLoader.active = true;
                }
            }
        },

        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("content/unarchive")
            toolTip: qsTr("Load suspended transaction")
            text: qsTr("Load suspended transaction")
            onTriggered: retrieveTransactionDialogLoader.active = true;
        },

        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("action/note_add")
            toolTip: qsTr("Add note")
            text: qsTr("Add note")
        },

        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("action/delete")
            toolTip: qsTr("Clear all")
            text: qsTr("Clear all")
            onTriggered: confirmationDialogLoader.active = true;
        }

    ]

    RRUi.SnackBar {
        id: snackBar
        onClicked: cartListView.undoLastTransaction();
    }

    Loader {
        id: suspendTransactionDialogLoader
        active: false
        sourceComponent: SuspendTransactionDialog {
            transactionId: animationStackView.currentItem.transactionId
            note: animationStackView.currentItem.note
            onAccepted: animationStackView.currentItem.suspendTransaction( { "note": note } );
            onClosed: suspendTransactionDialogLoader.active = false;
        }
        onLoaded: item.open();
    }

    Loader {
        id: retrieveTransactionDialogLoader
        active: false
        sourceComponent: RetrieveTransactionDialog {
            onClosed: retrieveTransactionDialogLoader.active = false;
            onAccepted: privateProperties.transactionId = transactionId;
        }
        onLoaded: item.open();
    }

    Loader {
        id: confirmationDialogLoader
        active: false
        sourceComponent: RRUi.AlertDialog {
            parent: QQC2.ApplicationWindow.contentItem
            text: qsTr("Clear all?")
            standardButtons: QQC2.Dialog.Yes | QQC2.Dialog.No
            onAccepted: animationStackView.currentItem.clearAll();
            onClosed: confirmationDialogLoader.active = false;

            FluidControls.BodyLabel { text: qsTr("Are you sure you want to clear this entry?") }
        }
        onLoaded: item.open();
    }

    RRUi.FailureAlertDialogLoader {
        id: failureAlertDialogLoader
        parent: QQC2.ApplicationWindow.contentItem
        title: qsTr("Failed to complete transaction")
    }

    contentItem: QQC2.StackView {
        id: animationStackView

        initialItem: Component {
            Item {
                id: saleContentItem

                readonly property int itemCount: cartListView.count
                readonly property string customerName: customerNameField.text
                readonly property string customerPhoneNumber: customerPhoneNumberField.text
                readonly property int transactionId: cartListView.transactionId
                readonly property string note: cartListView.note

                FluidControls.Card {
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
                                    filterColumnDialogLoader.active = true;
                                    break;
                                case 1:
                                    sortOrderDialogLoader.active = true;
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
                            filterColumn: RRModels.StockCategoryItemModel.ItemColumn

                            buttonRow: Row {
                                FluidControls.ToolButton {
                                    id: addToCartButton
                                    icon.name: "action/add_shopping_cart"
                                    visible: parent.parent.modelData.quantity > 0
                                    onClicked: cartListView.addItem(parent.parent.modelData);

                                    QQC2.ToolTip.visible: hovered
                                    QQC2.ToolTip.delay: 1500
                                    QQC2.ToolTip.text: qsTr("Add to cart")
                                }
                            }
                        }

                        FluidControls.BodyLabel {
                            anchors {
                                verticalCenter: parent.verticalCenter
                                left: parent.left
                                right: parent.right
                            }

                            wrapMode: Text.WordWrap
                            color: "#555555"
                            horizontalAlignment: Qt.AlignHCenter
                            visible: categoryListView.count == 0
                            text: categoryListView.model.filterText ? qsTr("There are no results for this search query.")
                                                                    : qsTr("You have no items.");
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

                        FluidControls.Card {
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

                                    FluidControls.Icon {
                                        anchors.verticalCenter: parent.verticalCenter
                                        name: "social/person"
                                        size: 20
                                        QQLayouts.Layout.preferredWidth: size
                                        QQLayouts.Layout.preferredHeight: size
                                    }

                                    Item { QQLayouts.Layout.preferredWidth: 8; QQLayouts.Layout.fillHeight: true }

                                    QQC2.TextField {
                                        id: customerNameField
                                        focus: true
                                        QQLayouts.Layout.fillWidth: true
                                        placeholderText: qsTr("Customer name")
                                    }

                                    FluidControls.ToolButton {
                                        id: customerOptionButton
                                        icon.source: FluidControls.Utils.iconUrl("navigation/more_vert")

                                        QQC2.ToolTip.visible: hovered
                                        QQC2.ToolTip.delay: 1500
                                        QQC2.ToolTip.text: qsTr("More")
                                    }
                                }

                                Row {
                                    visible: false
                                    spacing: 12

                                    FluidControls.Icon {
                                        anchors.verticalCenter: parent.verticalCenter
                                        name: "communication/phone"
                                        size: 20
                                    }

                                    QQC2.TextField {
                                        id: customerPhoneNumberField
                                        width: 300
                                        placeholderText: qsTr("Customer phone number")
                                    }
                                }
                            }
                        }

                        FluidControls.Card {
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

                                onSuccess: {
                                    if (paymentWizardLoader.active)
                                        paymentWizardLoader.item.accept();

                                    searchBar.clear();
                                    categoryListView.refresh();

                                    switch (successCode) {
                                    case RRModels.SaleCartModel.TransactionRetrieved:
                                        customerNameField.text = cartListView.customerName;
                                        snackBar.open(qsTr("Transaction retrieved."), "");
                                        break;
                                    case RRModels.SaleCartModel.TransactionSuspended:
                                        customerNameField.clear();
                                        privateProperties.transactionId = -1;
                                        snackBar.open(qsTr("Transaction suspended."), qsTr("Undo"));
                                        break;
                                    case RRModels.SaleCartModel.TransactionSubmitted:
                                        customerNameField.clear();
                                        privateProperties.transactionId = -1;
                                        snackBar.open(qsTr("Transaction submitted."), qsTr("Undo"));
                                        break;
                                    }
                                }
                                onError: {
                                    switch (errorCode) {
                                    case CartListView.FailedToConnect:
                                        break;
                                    }

                                    if (paymentWizardLoader.active) {
                                        paymentWizardLoader.item.displayError();
                                    } else {
                                        switch (errorCode) {
                                        default:
                                            failureAlertDialogLoader.title = qsTr("Failed to save transaction");
                                            failureAlertDialogLoader.message = qsTr("An unknown error has occurred.                      ");
                                            break;
                                        }
                                        failureAlertDialogLoader.create();
                                    }
                                }
                            }

                            FluidControls.BodyLabel {
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    left: parent.left
                                    right: parent.right
                                }

                                wrapMode: Text.WordWrap
                                color: "#555555"
                                horizontalAlignment: Qt.AlignHCenter
                                visible: cartListView.count == 0
                                text: qsTr("Your cart is empty.\nAdd items from the view in the left to get started.");
                            }
                        }

                        FluidControls.Card {
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
                                    onClicked: if (saleContentItem.validateUserInput()) paymentWizardLoader.active = true;
                                }
                            }
                        }
                    }
                }

                RRUi.BusyOverlay { visible: cartListView.busy }

                Loader {
                    id: paymentWizardLoader
                    active: false
                    sourceComponent: PaymentWizard {
                        totalCost: cartListView.totalCost
                        balance: cartListView.totalCost
                        customerName: customerNameField.text
                        onFinished: cartListView.submitTransaction(paymentInfo);
                        onClosed: paymentWizardLoader.active = false;
                    }

                    onLoaded: item.open();
                }

                Loader {
                    id: filterColumnDialogLoader
                    active: false
                    sourceComponent: RRUi.RadioButtonDialog {
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
                        onClosed: filterColumnDialogLoader.active = false;
                    }

                    onLoaded: item.open();
                }

                Loader {
                    id: sortOrderDialogLoader
                    active: false
                    sourceComponent: RRUi.RadioButtonDialog {
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
                        onClosed: sortOrderDialogLoader.active = false;
                    }

                    onLoaded: item.open();
                }

                function validateUserInput() {
                    if (customerNameField.text.trim() === "") {
                        failureAlertDialogLoader.title = qsTr("Failed to complete transaction");
                        failureAlertDialogLoader.message = qsTr("Customer name cannot be empty.                     ");
                        failureAlertDialogLoader.create();
                        return false;
                    } else if (cartListView.count == 0) {
                        failureAlertDialogLoader.title = qsTr("Failed to complete transaction");
                        failureAlertDialogLoader.message = qsTr("There are no items in your cart.                   ");
                        failureAlertDialogLoader.create();
                        return false;
                    }

                    return true;
                }

                function suspendTransaction(params) { cartListView.suspendTransaction(params); }

                function clearAll() {
                    cartListView.clearAll();
                    privateProperties.transactionId = -1;
                    customerNameField.clear();
                    snackBar.open(qsTr("Entry cleared."), "");
                }
            }
        }
    }
}
