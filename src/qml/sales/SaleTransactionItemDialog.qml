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

    contentItem: FocusScope {
        focus: true

        Grid {
            id: dialogHeader
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            columns: 2

            FluidControls.SubheadingLabel {
                text: qsTr("Transaction ID:")
            }

            FluidControls.SubheadingLabel {
                text: transactionId
            }

            FluidControls.SubheadingLabel {
                text: qsTr("Customer name:")
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
            }

            Row {
                FluidControls.SubheadingLabel {
                    anchors.verticalCenter: parent.verticalCenter
                    text: customerName
                }

                RRUi.ToolButton {
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
                text: styleData.title
                height: 40
                verticalAlignment: Qt.AlignVCenter
            }

            itemDelegate: FluidControls.SubheadingLabel {
                text: {
                    switch (styleData.column) {
                    case tableView.columns.length - 1:
                        GlobalSettings.toCurrencyString(styleData.modelData);
                        break;
                    default:
                        styleData.modelData;
                    }
                }
                height: 40
                elide: Text.ElideRight
                verticalAlignment: Qt.AlignVCenter
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
                    title: qsTr("Revenue")
                    width: tableView.width / tableView.columns.length
                    role: "cost"
                }
            ]
        }
    }
}
