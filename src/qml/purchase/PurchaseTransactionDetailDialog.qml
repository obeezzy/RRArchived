import "../common"
import "../rrui" as RRUi
import "../singletons"
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

RRUi.Dialog {
    id: purchaseTransactionItemDialog

    property int transactionId: 0
    property string customerName: ""
    property string customerPhoneNumber: ""

    function show(transactionInfo) {
        if (Object(transactionInfo).hasOwnProperty("transaction_id"))
            purchaseTransactionItemDialog.transactionId = transactionInfo.transaction_id;

        if (Object(transactionInfo).hasOwnProperty("customer_name"))
            purchaseTransactionItemDialog.customerName = transactionInfo.customer_name;

        open();
    }

    implicitWidth: 840
    implicitHeight: 640
    title: qsTr("Transaction details")
    standardButtons: RRUi.Dialog.Ok
    onAboutToShow: tableView.refresh()

    contentItem: FocusScope {
        focus: true

        Column {
            id: dialogHeader

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            Row {
                spacing: 4

                FluidControls.SubheadingLabel {
                    text: qsTr("Transaction ID:")
                }

                FluidControls.SubheadingLabel {
                    text: purchaseTransactionItemDialog.transactionId
                }

            }

            Row {
                FluidControls.SubheadingLabel {
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Customer name:")
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }

                // Spacer
                Item {
                    width: 8
                    height: 1
                }

                FluidControls.SubheadingLabel {
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: purchaseTransactionItemDialog.customerName
                }

                // Spacer
                Item {
                    width: 16
                    height: 1
                }

                RRUi.ToolButton {
                    visible: false
                    anchors.verticalCenter: parent.verticalCenter
                    icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                    width: FluidControls.Units.iconSizes.medium
                    height: width
                }

            }

            Row {
                visible: purchaseTransactionItemDialog.customerPhoneNumber !== ""

                FluidControls.SubheadingLabel {
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Customer phone number:")
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }

                // Spacer
                Item {
                    width: 8
                    height: 1
                }

                FluidControls.SubheadingLabel {
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: purchaseTransactionItemDialog.customerPhoneNumber
                }

                // Spacer
                Item {
                    width: 16
                    height: 1
                }

                RRUi.ToolButton {
                    anchors.verticalCenter: parent.verticalCenter
                    icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                    width: FluidControls.Units.iconSizes.medium
                    height: width
                }

            }

        }

        PurchaseTransactionDetailTableView {
            id: tableView

            anchors {
                top: dialogHeader.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                topMargin: 24
            }

        }

    }

}
