import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import Qt.labs.qmlmodels 1.0 as QQModels
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../singletons"

TableView {
    id: expenseReportTableView

    property alias busy: expenseReportModel.busy
    property alias autoQuery: expenseReportModel.autoQuery
    property Component buttonRow: null

    enum Columns {
        RowIdColumn,
        PurposeColumn,
        AmountColumn,
        ActionColumn
    }

    signal success(int successCode)
    signal error(int errorCode)

    function refresh() { expenseReportModel.refresh(); }

    flickableDirection: TableView.VerticalFlick

    model: RRModels.ExpenseReportModel {
        id: expenseReportModel
        onSuccess: expenseReportTableView.success(successCode);
        onError: expenseReportTableView.error(errorCode);
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: expenseReportTableView.contentHeight > expenseReportTableView.height
    }

    delegate: QQModels.DelegateChooser {
        QQModels.DelegateChoice {
            column: ExpenseReportTableView.RowIdColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: row + 1
            }
        }

        QQModels.DelegateChoice {
            column: ExpenseReportTableView.PurposeColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignLeft
                verticalAlignment: Qt.AlignVCenter
                text: purpose
            }
        }

        QQModels.DelegateChoice {
            column: ExpenseReportTableView.AmountColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: Number(amount).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
            }
        }

        QQModels.DelegateChoice {
            column: ExpenseReportTableView.ActionColumn
            delegate: Loader {
                readonly property var modelData: {
                    "client_id": model.client_id,
                    "transaction_id": model.transaction_id
                }

                sourceComponent: expenseReportTableView.buttonRow
            }
        }
    }
}
