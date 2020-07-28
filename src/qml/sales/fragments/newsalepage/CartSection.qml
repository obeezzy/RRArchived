import "../.."
import "../../../common"
import "../../../rrui" as RRUi
import "../../../stock" as Stock
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import com.gecko.rr.models 1.0 as RRModels

FocusScope {
    id: cartSection

    readonly property int transactionId: cart.transactionId
    readonly property string note: cart.note
    readonly property bool cartEmpty: cart.count === 0
    readonly property int itemCount: cart.count
    readonly property alias model: cart.model
    property alias customerName: customerInfo.name
    property alias customerPhoneNumber: customerInfo.phoneNumber

    signal success(var result)
    signal error(var result)
    signal viewRequested(int productId)
    signal checkoutRequested()

    function addProduct(modelData) {
        cart.addProduct(modelData);
    }

    function suspendTransaction(params) {
        cart.suspendTransaction(params);
    }

    function submitTransaction(payment) {
        cart.submitTransaction(payment);
    }

    function undoLastTransaction() {
        cart.undoLastTransaction();
    }

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
                switch (result.code) {
                case RRModels.SaleCartModel.RetrieveTransactionSuccess:
                    customerInfo.name = customerName;
                    MainWindow.snackBar.show(qsTr("Transaction retrieved."));
                    break;
                case RRModels.SaleCartModel.SuspendTransactionSuccess:
                    customerInfo.clearAll();
                    cart.transactionId = 0;
                    MainWindow.snackBar.show(qsTr("Transaction suspended."), qsTr("Undo"));
                    break;
                case RRModels.SaleCartModel.SubmitTransactionSuccess:
                    customerInfo.clearAll();
                    transactionId = 0;
                    MainWindow.snackBar.show(qsTr("Transaction submitted."), qsTr("Undo"));
                    break;
                case RRModels.SaleCartModel.UndoSubmitTransactionSuccess:
                    customerInfo.clearAll();
                    transactionId = 0;
                    MainWindow.snackBar.show(qsTr("Transaction reverted."));
                }
                cartSection.success(result);
            }
            onError: cartSection.error(result)
            onEditRequested: cartProductEditorDialog.show(product)
            onViewRequested: cartSection.viewRequested(productId)
        }

        Checkout {
            QQLayouts.Layout.fillWidth: true
            totalCost: cart.totalCost
            onCheckoutRequested: cartSection.checkoutRequested()
        }

    }

    RRUi.BusyOverlay {
        visible: cart.busy
    }

    RRUi.RadioButtonDialog {
        title: qsTr("Choose filter sort")
    }

    RRUi.RadioButtonDialog {
        title: qsTr("Choose sort order")
    }

    CartProductEditorDialog {
        id: cartProductEditorDialog

        onAccepted: cart.updateProduct(productId, {
            "quantity": quantity,
            "unit_price": unitPrice,
            "cost": cost
        })
    }

}
