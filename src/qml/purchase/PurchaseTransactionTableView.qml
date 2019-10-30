import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import "../singletons"

RRUi.DataTableView {
    id: purchaseTransactionTableView

    property Component buttonRow: null
    property string filterText: ""
    property int filterColumn: -1
    property int keys: RRModels.PurchaseTransactionModel.Completed
    property date from: new Date()
    property date to: new Date()

    signal success(int successCode)
    signal error(int errorCode)

    function refresh() { purchaseTransactionModel.refresh(); }
    function undoLastCommit() { purchaseTransactionModel.undoLastCommit(); }
    function removeTransaction(transactionId) { purchaseTransactionModel.removeTransaction(transactionId); }

    bottomMargin: 20
    clip: true

    FluidControls.Placeholder {
        visible: purchaseTransactionTableView.count === 0 && !purchaseTransactionModel.busy
        anchors.centerIn: parent
        icon.source: Qt.resolvedUrl("qrc:/icons/cart.svg")
        text: qsTr("No transactions took place on this day.")
    }

    QQC2.BusyIndicator {
        anchors.centerIn: parent
        visible: purchaseTransactionModel.busy
    }

    model: RRModels.PurchaseTransactionModel {
        id: purchaseTransactionModel
        tableViewWidth: purchaseTransactionTableView.widthWithoutMargins
        filterText: purchaseTransactionTableView.filterText
        filterColumn: purchaseTransactionTableView.filterColumn
        keys: purchaseTransactionTableView.keys
        from: purchaseTransactionTableView.from
        to: purchaseTransactionTableView.to
        onSuccess: purchaseTransactionTableView.success(successCode);
        onError: purchaseTransactionTableView.error(errorCode);
    }


    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: purchaseTransactionTableView.contentHeight > purchaseTransactionTableView.height
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.PurchaseTransactionModel.TransactionIdColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseTransactionTableView.columnHeader.children[column].width
                implicitHeight: purchaseTransactionTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: ('0000000' + transaction_id).slice(-6)
                    font.italic: true
                }
            }
        }

        DelegateChoice {
            column: RRModels.PurchaseTransactionModel.CustomerNameColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseTransactionTableView.columnHeader.children[column].width
                implicitHeight: purchaseTransactionTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: customer_name
                }
            }
        }

        DelegateChoice {
            column: RRModels.PurchaseTransactionModel.TotalCostColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseTransactionTableView.columnHeader.children[column].width
                implicitHeight: purchaseTransactionTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: Number(total_cost).toLocaleCurrencyString(Qt.locale(GlobalSettings.localeName))
                }
            }
        }

        DelegateChoice {
            column: RRModels.PurchaseTransactionModel.ActionColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseTransactionTableView.columnHeader.children[column].width
                implicitHeight: purchaseTransactionTableView.rowHeader.children[row].height

                Loader {
                    readonly property var modelData: {
                        "client_id": model.client_id,
                        "customer_name": model.customer_name,
                        "transaction_id": model.transaction_id
                    }

                    anchors.centerIn: parent
                    sourceComponent: purchaseTransactionTableView.buttonRow
                }
            }
        }
    }
}
