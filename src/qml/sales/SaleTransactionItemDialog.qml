import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
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
    title: qsTr("View transaction details")
    standardButtons: RRUi.Dialog.Ok

    function show(transactionInfo) {
        if (Object(transactionInfo).hasOwnProperty("transaction_id"))
            saleTransactionItemDialog.transactionId = transactionInfo.transaction_id;
        if (Object(transactionInfo).hasOwnProperty("customer_name"))
            saleTransactionItemDialog.customerName = transactionInfo.customer_name;

        open();
    }

    onAboutToShow: tableView.model.refresh();

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

        RRUi.TableView {
            id: tableView
            anchors {
                top: dialogHeader.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                topMargin: 24
            }

            headerPositioning: ListView.OverlayHeader

            model: RRModels.SaleTransactionItemModel {
                transactionId: saleTransactionItemDialog.transactionId
            }

            headerDelegate: FluidControls.SubheadingLabel {
                text: headerData.title
                height: 40
                verticalAlignment: Qt.AlignVCenter
            }

            cellDelegate: FluidControls.SubheadingLabel {
                text: {
                    if (cellData.modelData === undefined)
                        return "";

                    switch (cellData.textRole) {
                    case "unit_price":
                    case "cost":
                        return GlobalSettings.toCurrencyString(cellData.modelData);
                    default:
                        return cellData.modelData;
                    }
                }
                height: 40
                elide: Text.ElideRight
                verticalAlignment: Qt.AlignVCenter

                MouseArea {
                    id: area
                    anchors.fill: parent
                    enabled: parent.truncated
                    hoverEnabled: parent.truncated
                    propagateComposedEvents: true
                }

                QQC2.ToolTip {
                    visible: area.containsMouse
                    delay: parent.text === "" ? 1500 : 300
                    text: parent.text
                }
            }

            columns: [
                RRUi.TableViewColumn {
                    title: qsTr("Category")
                    width: tableView.width / tableView.columns.length
                    role: "category"
                },

                RRUi.TableViewColumn {
                    title: qsTr("Item")
                    width: tableView.width / tableView.columns.length
                    role: "item"
                },

                RRUi.TableViewColumn {
                    title: qsTr("Quantity")
                    width: tableView.width / tableView.columns.length
                    role: "quantity"
                },

                RRUi.TableViewColumn {
                    title: qsTr("Unit")
                    width: tableView.width / tableView.columns.length
                    role: "unit"
                },

                RRUi.TableViewColumn {
                    title: qsTr("Unit Price")
                    width: tableView.width / tableView.columns.length
                    role: "unit_price"
                },

                RRUi.TableViewColumn {
                    title: qsTr("Cost")
                    width: tableView.width / tableView.columns.length
                    role: "cost"
                }
            ]
        }
    }
}
