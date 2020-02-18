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
    id: incomeTransactionTableView

    property Component buttonRow: null
    property string filterText: ""
    property int filterColumn: -1
    property int keys: RRModels.IncomeTransactionModel.Completed
    property date from: new Date()
    property date to: new Date()

    signal success(var result)
    signal error(var result)

    function removeTransaction(transactionId) { incomeTransactionTableView.model.removeTransaction(transactionId); }
    function refresh() { incomeTransactionTableView.model.refresh(); }
    function undoLastCommit() { incomeTransactionTableView.model.undoLastCommit(); }

    bottomMargin: 20
    clip: true
    visible: !model.busy
    flickableDirection: TableView.VerticalFlick
    columnSpacing: 8

    FluidControls.Placeholder {
        visible: incomeTransactionTableView.rows === 0 && !incomeTransactionTableView.model.busy
        anchors.centerIn: parent
        icon.source: Qt.resolvedUrl("qrc:/icons/cash-multiple.svg")
        text: qsTr("No transactions took place on this day.")
    }

    QQC2.BusyIndicator {
        anchors.centerIn: parent
        visible: incomeTransactionTableView.model.busy
    }

    model: RRModels.IncomeTransactionModel {
        filterText: incomeTransactionTableView.filterText
        filterColumn: incomeTransactionTableView.filterColumn
        keys: incomeTransactionTableView.keys
        from: incomeTransactionTableView.from
        to: incomeTransactionTableView.to
        onSuccess: incomeTransactionTableView.success(result);
        onError: incomeTransactionTableView.error(result);
    }

    QQC2.ScrollBar.vertical: QQC2.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: incomeTransactionTableView.contentHeight > incomeTransactionTableView.height
        size: .3
        width: 5
        contentItem: Rectangle {
            color: Material.color(Material.Grey)
            radius: width / 2
        }
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.IncomeTransactionModel.TransactionIdColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: incomeTransactionTableView.columnHeader.children[RRModels.IncomeTransactionModel.TransactionIdColumn].width
                implicitHeight: incomeTransactionTableView.rowHeader.children[0].height

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
            column: RRModels.IncomeTransactionModel.ClientNameColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: incomeTransactionTableView.columnHeader.children[RRModels.IncomeTransactionModel.ClientNameColumn].width
                implicitHeight: incomeTransactionTableView.rowHeader.children[0].height

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
            column: RRModels.IncomeTransactionModel.AmountColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: incomeTransactionTableView.columnHeader.children[RRModels.IncomeTransactionModel.AmountColumn].width
                implicitHeight: incomeTransactionTableView.rowHeader.children[0].height

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
            column: RRModels.IncomeTransactionModel.ActionColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: incomeTransactionTableView.columnHeader.children[RRModels.IncomeTransactionModel.ActionColumn].width
                implicitHeight: incomeTransactionTableView.rowHeader.children[0].height

                Loader {
                    readonly property var modelData: {
                        "client_id": model.client_id,
                        "transaction_id": model.transaction_id
                    }

                    anchors.centerIn: parent
                    sourceComponent: incomeTransactionTableView.buttonRow
                }
            }
        }
    }
}
