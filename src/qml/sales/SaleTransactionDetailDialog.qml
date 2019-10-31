import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../common"
import "../singletons"

RRUi.Dialog {
    id: saleTransactionItemDialog

    property int transactionId: -1
    property string customerName: ""
    property string customerPhoneNumber: ""

    implicitWidth: 840
    implicitHeight: 640
    title: qsTr("Transaction details")
    standardButtons: RRUi.Dialog.Ok

    function show(transactionInfo) {
        if (Object(transactionInfo).hasOwnProperty("transaction_id"))
            saleTransactionItemDialog.transactionId = transactionInfo.transaction_id;
        if (Object(transactionInfo).hasOwnProperty("customer_name"))
            saleTransactionItemDialog.customerName = transactionInfo.customer_name;

        open();
    }

    onAboutToShow: tableView.refresh();

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
                    text: saleTransactionItemDialog.transactionId
                }
            }

            Row {
                FluidControls.SubheadingLabel {
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Customer name:")
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }

                Item { width: 8; height: 1 } // Spacer

                FluidControls.SubheadingLabel {
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: saleTransactionItemDialog.customerName
                }

                Item { width: 16; height: 1 } // Spacer

                RRUi.ToolButton {
                    visible: false
                    anchors.verticalCenter: parent.verticalCenter
                    icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                    width: FluidControls.Units.iconSizes.medium
                    height: width
                }
            }

            Row {
                visible: saleTransactionItemDialog.customerPhoneNumber !== ""

                FluidControls.SubheadingLabel {
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Customer phone number:")
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }

                Item { width: 8; height: 1 } // Spacer

                FluidControls.SubheadingLabel {
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: saleTransactionItemDialog.customerPhoneNumber
                }

                Item { width: 16; height: 1 } // Spacer

                RRUi.ToolButton {
                    anchors.verticalCenter: parent.verticalCenter
                    icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                    width: FluidControls.Units.iconSizes.medium
                    height: width
                }
            }
        }

        SaleTransactionDetailTableView {
            id: tableView
            anchors {
                top: dialogHeader.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                topMargin: 24
            }
            transactionId: saleTransactionItemDialog.transactionId
        }
    }
}
