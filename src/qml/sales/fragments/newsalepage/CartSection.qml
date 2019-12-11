import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../../../rrui" as RRUi
import "../../../stock" as Stock
import "../../../common"
import "../.."

FocusScope {
    id: cartSection

    readonly property int transactionId: cart.transactionId
    readonly property string note: cart.note
    readonly property bool cartEmpty: cart.count === 0
    readonly property int itemCount: cart.count
    readonly property alias model: cart.model
    property alias customerName: customerInfo.name
    property alias customerPhoneNumber: customerInfo.phoneNumber

    signal success(int successCode)
    signal error(int errorCode, string errorMessage)
    signal viewRequested(int itemId)
    signal checkoutRequested

    function addItem(modelData) { cart.addItem(modelData); }
    function suspendTransaction(params) { cart.suspendTransaction(params); }
    function submitTransaction(paymentInfo) { cart.submitTransaction(paymentInfo); }
    function undoLastTransaction() { cart.undoLastTransaction(); }

    function clear() {
        cart.clearAll();
        customerInfo.clearAll();
    }


    implicitWidth: 600
    implicitHeight: 400

    QQLayouts.ColumnLayout {
        anchors.fill: parent

        CustomerInfo {
            id: customerInfo
            QQLayouts.Layout.fillWidth: true
        }

        Cart {
            id: cart
            customerName: customerInfo.name
            QQLayouts.Layout.fillWidth: true
            QQLayouts.Layout.fillHeight: true
            onSuccess: {
                switch (successCode) {
                case RRModels.SaleCartModel.RetrieveTransactionSuccess:
                    customerInfo.name = customerName;
                    MainWindow.snackBar.show(qsTr("Transaction retrieved."));
                    break;
                case RRModels.SaleCartModel.SuspendTransactionSuccess:
                    customerInfo.clearAll();
                    cart.transactionId = -1;
                    MainWindow.snackBar.show(qsTr("Transaction suspended."), qsTr("Undo"));
                    break;
                case RRModels.SaleCartModel.SubmitTransactionSuccess:
                    customerInfo.clearAll();
                    transactionId = -1;
                    MainWindow.snackBar.show(qsTr("Transaction submitted."), qsTr("Undo"));
                    break;
                case RRModels.SaleCartModel.UndoSubmitTransactionSuccess:
                    customerInfo.clearAll();
                    transactionId = -1;
                    MainWindow.snackBar.show(qsTr("Transaction reverted."));
                }

                cartSection.success(successCode);
            }

            onError: cartSection.error(errorCode, errorMessage);
            onEditRequested: cartItemEditorDialog.show(itemInfo);
            onViewRequested: cartSection.viewRequested(itemId);
        }

        Checkout {
            QQLayouts.Layout.fillWidth: true
            totalCost: cart.totalCost
            onCheckoutRequested: cartSection.checkoutRequested();
        }
    }

    RRUi.BusyOverlay { visible: cart.busy }

    RRUi.RadioButtonDialog {
        title: qsTr("Choose filter sort")
    }

    RRUi.RadioButtonDialog {
        title: qsTr("Choose sort order")
    }

    CartItemEditorDialog {
        id: cartItemEditorDialog
        onAccepted: cart.updateItem(itemId, {
                                        "quantity": quantity,
                                        "unit_price": unitPrice,
                                        "cost": cost
                                    });
    }
}
