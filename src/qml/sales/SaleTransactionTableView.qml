import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import "../singletons"

RRUi.DataTableView {
    id: saleTransactionTableView

    property Component buttonRow: null
    property string filterText: ""
    property int filterColumn: -1
    property int keys: RRModels.SaleTransactionModel.Completed
    property date from: new Date()
    property date to: new Date()

    signal success(int successCode)
    signal error(int errorCode)


    function refresh() { saleTransactionModel.refresh(); }
    function undoLastCommit() { saleTransactionModel.undoLastCommit(); }

    flickableDirection: TableView.VerticalFlick
    bottomMargin: 20
    columnSpacing: 8
    clip: true
    visible: !saleTransactionModel.busy

    function removeTransaction(transactionId) { saleTransactionModel.removeTransaction(transactionId); }

    FluidControls.Placeholder {
        visible: saleTransactionTableView.count === 0 && !saleTransactionModel.busy
        anchors.centerIn: parent
        icon.source: Qt.resolvedUrl("qrc:/icons/coin.svg")
        text: qsTr("No transactions took place on this day.")
    }

    QQC2.BusyIndicator {
        anchors.centerIn: parent
        visible: saleTransactionModel.busy
    }

    model: RRModels.SaleTransactionModel {
        id: saleTransactionModel
        tableViewWidth: saleTransactionTableView.widthWithoutMargins
        filterText: saleTransactionTableView.filterText
        filterColumn: saleTransactionTableView.filterColumn
        keys: saleTransactionTableView.keys
        from: saleTransactionTableView.from
        to: saleTransactionTableView.to
        onSuccess: saleTransactionTableView.success(successCode);
        onError: saleTransactionTableView.error(errorCode);
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: saleTransactionTableView.contentHeight > saleTransactionTableView.height
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.SaleTransactionModel.TransactionIdColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: saleTransactionTableView.columnHeader.children[column].width
                implicitHeight: saleTransactionTableView.rowHeader.children[row].height

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
            column: RRModels.SaleTransactionModel.CustomerNameColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: saleTransactionTableView.columnHeader.children[column].width
                implicitHeight: saleTransactionTableView.rowHeader.children[row].height

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
            column: RRModels.SaleTransactionModel.TotalCostColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: saleTransactionTableView.columnHeader.children[column].width
                implicitHeight: saleTransactionTableView.rowHeader.children[row].height

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
            column: RRModels.SaleTransactionModel.ActionColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: saleTransactionTableView.columnHeader.children[column].width
                implicitHeight: saleTransactionTableView.rowHeader.children[row].height

                Loader {
                    readonly property var modelData: {
                        "client_id": model.client_id,
                        "customer_name": model.customer_name,
                        "transaction_id": model.transaction_id,
                        "row": index
                    }

                    anchors.centerIn: parent
                    sourceComponent: saleTransactionTableView.buttonRow
                }
            }
        }
    }
}
