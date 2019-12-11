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
    property int transactionId: -1

    signal success(int successCode)
    signal error(int errorCode, string errorMessage)
    signal editRequested(var itemInfo)
    signal viewRequested(int itemId)

    function addItem(itemInfo) { cartListView.addItem(itemInfo); }
    function updateItem(itemId, itemInfo) { cartListView.updateItem(itemId, itemInfo); }
    function suspendTransaction(params) { cartListView.suspendTransaction(params); }
    function undoLastTransaction() { cartListView.undoLastTransaction(); }
    function submitTransaction(paymentInfo) { cartListView.submitTransaction(paymentInfo); }

    function clearAll() {
        cartListView.clearAll();
        transactionId = -1;
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

        onViewRequested: cart.viewRequested(itemId);
        onEditRequested: cart.editRequested(itemInfo);

        onSuccess: cart.success(successCode);

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
            case CartListView.UndoSubmitTransactionError:
                errorString = qsTr("Failed to undo transaction.");
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

            cart.error(errorCode, errorString);
        }
    }
}
