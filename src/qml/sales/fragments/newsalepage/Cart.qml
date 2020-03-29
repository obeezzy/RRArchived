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

RRUi.Card {
    id: cart

    readonly property string note: ""
    readonly property int count: cartListView.count
    readonly property bool busy: cartListView.busy
    readonly property alias model: cartListView.model
    readonly property real totalCost: cartListView.totalCost

    property string customerName: ""
    property int transactionId: 0

    signal success(var result)
    signal error(var result)
    signal editRequested(var product)
    signal viewRequested(int productId)

    function addProduct(product) { cartListView.addProduct(product); }
    function updateProduct(productId, product) { cartListView.updateProduct(productId, product); }
    function suspendTransaction(params) { cartListView.suspendTransaction(params); }
    function undoLastTransaction() { cartListView.undoLastTransaction(); }
    function submitTransaction(paymentInfo) { cartListView.submitTransaction(paymentInfo); }

    function clearAll() {
        cartListView.clearAll();
        transactionId = 0;
    }


    padding: 0
    leftPadding: 8
    rightPadding: 8
    Material.elevation: 2

    CartListView {
        id: cartListView
        anchors.fill: parent
        customerName: cart.customerName
        transactionId: cart.transactionId

        onViewRequested: cart.viewRequested(productId);
        onEditRequested: cart.editRequested(product);

        onSuccess: cart.success(result);

        onError: {
            var errorString = "";
            switch (result.code) {
            case CartListView.ConnectionError:
                errorString = qsTr("Failed to connect to the database.");
                break;
            case CartListView.SuspendTransactionError:
                errorString = qsTr("Failed to suspend transaction.");
                break;
            case CartListView.SubmitTransactionError:
                errorString = qsTr("Failed to submit transaction.");
                break;
            case CartListView.UndoSubmitTransactionError:
                errorString = qsTr("Failed to undo transaction.");
                break;
            case CartListView.RetrieveTransactionError:
                errorString = qsTr("Failed to retrieve transaction.");
                break;
            case CartListView.SuspendTransactionError:
                errorString = qsTr("Failed to suspend transaction.");
                break;
            default:
                errorString = qsTr("An unknown error occurred.");
                break;
            }

            result.extra = errorString;
            cart.error(result);
        }
    }
}
