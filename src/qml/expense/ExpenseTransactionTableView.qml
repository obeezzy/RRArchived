import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import "../singletons"

RRUi.DataTableView {
    id: expenseTransactionTableView

    property Component buttonRow: null
    property string filterText: ""
    property int filterColumn: -1
    property int keys: RRModels.ExpenseTransactionModel.Completed
    property date from: new Date()
    property date to: new Date()

    signal success(int successCode)
    signal error(int errorCode)

    function removeTransaction(transactionId) { expenseTransactionTableView.model.removeTransaction(transactionId); }
    function refresh() { expenseTransactionTableView.model.refresh(); }
    function undoLastCommit() { expenseTransactionTableView.model.undoLastCommit(); }

    bottomMargin: 20
    clip: true
    visible: !model.busy
    flickableDirection: TableView.VerticalFlick
    columnSpacing: 8

    FluidControls.Placeholder {
        visible: expenseTransactionTableView.rows === 0 && !expenseTransactionTableView.model.busy
        anchors.centerIn: parent
        icon.source: Qt.resolvedUrl("qrc:/icons/cash-multiple.svg")
        text: qsTr("No transactions took place on this day.")
    }

    QQC2.BusyIndicator {
        anchors.centerIn: parent
        visible: expenseTransactionTableView.model.busy
    }

    model: RRModels.ExpenseTransactionModel {
        filterText: expenseTransactionTableView.filterText
        filterColumn: expenseTransactionTableView.filterColumn
        keys: expenseTransactionTableView.keys
        from: expenseTransactionTableView.from
        to: expenseTransactionTableView.to
        onSuccess: expenseTransactionTableView.success(successCode);
        onError: expenseTransactionTableView.error(errorCode);
    }

    QQC2.ScrollBar.vertical: QQC2.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: expenseTransactionTableView.contentHeight > expenseTransactionTableView.height
        size: .3
        width: 5
        contentItem: Rectangle {
            color: Material.color(Material.Grey)
            radius: width / 2
        }
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.ExpenseTransactionModel.TransactionIdColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: expenseTransactionTableView.columnHeader.children[RRModels.ExpenseTransactionModel.TransactionIdColumn].width
                implicitHeight: expenseTransactionTableView.rowHeader.children[0].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: transaction_id
                }
            }
        }

        DelegateChoice {
            column: RRModels.ExpenseTransactionModel.ClientNameColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: expenseTransactionTableView.columnHeader.children[RRModels.ExpenseTransactionModel.ClientNameColumn].width
                implicitHeight: expenseTransactionTableView.rowHeader.children[0].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: client_name
                }
            }
        }

        DelegateChoice {
            column: RRModels.ExpenseTransactionModel.AmountColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: expenseTransactionTableView.columnHeader.children[RRModels.ExpenseTransactionModel.AmountColumn].width
                implicitHeight: expenseTransactionTableView.rowHeader.children[0].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: Number(amount).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
                }
            }
        }

        DelegateChoice {
            column: RRModels.ExpenseTransactionModel.ActionColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: expenseTransactionTableView.columnHeader.children[RRModels.ExpenseTransactionModel.ActionColumn].width
                implicitHeight: expenseTransactionTableView.rowHeader.children[0].height

                Loader {
                    readonly property var modelData: {
                        "client_id": model.client_id,
                        "transaction_id": model.transaction_id
                    }

                    anchors.centerIn: parent
                    sourceComponent: expenseTransactionTableView.buttonRow
                }
            }
        }
    }
}
