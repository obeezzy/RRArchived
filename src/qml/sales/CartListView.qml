import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import "../singletons"
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels

ListView {
    id: cartListView

    readonly property bool busy: model.busy
    property alias customerName: saleCartModel.customerName
    property alias customerPhoneNumber: saleCartModel.customerPhoneNumber
    property alias transactionId: saleCartModel.transactionId
    property alias note: saleCartModel.note
    readonly property real totalCost: model.totalCost
    property real amountPaid: model.amountPaid

    signal viewRequested(int productId)
    signal editRequested(var product)
    signal success(int successCode)
    signal error(int errorCode)

    function addProduct(product) { cartListView.model.addProduct(product); }
    function submitTransaction(paymentInfo) { cartListView.model.submitTransaction(paymentInfo); }
    function suspendTransaction(params) { cartListView.model.suspendTransaction(params); }
    function undoLastTransaction() { cartListView.model.undoLastCommit(); }
    function clearAll() { cartListView.model.clearAll(); }
    function updateProduct(productId, product) { cartListView.model.updateProduct(productId, product); }
    function addPayment(amount, method) { cartListView.model.addPayment(amount, method); }
    function clearPayments() { cartListView.model.clearPayments(); }

    spacing: 10
    clip: true

    topMargin: 4
    bottomMargin: 10

    model: RRModels.SaleCartModel {
        id: saleCartModel
        onSuccess: cartListView.success(successCode);
        onError: cartListView.error(errorCode);
    }

    delegate: Item {
        width: ListView.view.width
        height: delegateColumn.height + quantitySpinBox.height + 10

        Column {
            id: delegateColumn
            anchors {
                left: parent.left
                top: parent.top
            }

            spacing: 2

            FluidControls.HeadlineLabel {
                text: product
            }

            FluidControls.SubheadingLabel {
                text: "From <i>" + category + "</i><br/>"
                      + available_quantity + " "
                      + unit + " left<br/>Costs "
                      + Number(unit_price).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName)) + " each"
            }
        }

        Row {
            anchors {
                right: parent.right
                top: parent.top
            }

            RRUi.ToolButton {
                id: viewButton
                icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                text: qsTr("View details")
                onClicked: cartListView.viewRequested(product_id);
            }

            RRUi.ToolButton {
                id: editButton
                icon.source: FluidControls.Utils.iconUrl("image/edit")
                text: qsTr("Edit product")
                onClicked: cartListView.editRequested({ "product_index": index,
                                                          "product_id": product_id,
                                                          "product": product,
                                                          "quantity": quantity,
                                                          "unit_price": unit_price,
                                                          "cost": cost,
                                                          "available_quantity": available_quantity,
                                                          "unit": unit });
            }

            RRUi.ToolButton {
                id: deleteButton
                icon.source: FluidControls.Utils.iconUrl("action/delete")
                text: qsTr("Delete product")
                onClicked: cartListView.model.removeItem(product_id);
            }
        }

        FluidControls.SubheadingLabel {
            anchors {
                right: parent.right
                bottom: parent.bottom
            }

            text: Number(cost).toLocaleCurrencyString(Qt.locale("en_NG"))
        }

        RRUi.QuantitySpinBox {
            id: quantitySpinBox
            quantity: model.quantity
            maximumQuantity: model.available_quantity
            unit: model.unit
            anchors {
                bottomMargin: 4
                bottom: parent.bottom
            }

            onQuantityUpdated: cartListView.model.setProductQuantity(model.product_id, newQuantity);
        }

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            color: "lightgray"
            height: 1
            visible: index < cartListView.count - 1
        }
    }

    FluidControls.Placeholder {
        anchors.centerIn: parent
        visible: cartListView.count == 0
        icon.source: Qt.resolvedUrl("qrc:/icons/cart-outline.svg")
        text: qsTr("Your cart is empty.\nAdd products from the view in the left to get started.");
    }
}
