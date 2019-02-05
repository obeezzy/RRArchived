import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import Qt.labs.qmlmodels 1.0 as QQModels
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../singletons"

TableView {
    id: purchaseReportTableView

    property alias busy: purchaseReportModel.busy
    property alias autoQuery: purchaseReportModel.autoQuery
    property Component buttonRow: null

    enum Columns {
        RowIdColumn,
        CategoryColumn,
        ItemColumn,
        QuantityBoughtColumn,
        TotalAmountColumn,
        ActionColumn
    }

    signal success(int successCode)
    signal error(int errorCode)

    function refresh() { purchaseReportModel.refresh(); }

    flickableDirection: TableView.VerticalFlick

    model: RRModels.PurchaseReportModel {
        id: purchaseReportModel
        onSuccess: purchaseReportTableView.success(successCode);
        onError: purchaseReportTableView.error(errorCode);
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: purchaseReportTableView.contentHeight > purchaseReportTableView.height
    }

    delegate: QQModels.DelegateChooser {
        QQModels.DelegateChoice {
            column: PurchaseReportTableView.RowIdColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: row + 1
            }
        }

        QQModels.DelegateChoice {
            column: PurchaseReportTableView.CategoryColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignLeft
                verticalAlignment: Qt.AlignVCenter
                text: category
            }
        }

        QQModels.DelegateChoice {
            column: PurchaseReportTableView.ItemColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignLeft
                verticalAlignment: Qt.AlignVCenter
                text: item
            }
        }

        QQModels.DelegateChoice {
            column: PurchaseReportTableView.QuantityBoughtColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: quantity_bought + " " + unit
            }
        }

        QQModels.DelegateChoice {
            column: PurchaseReportTableView.TotalAmountColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: Number(total_amount).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
            }
        }

        QQModels.DelegateChoice {
            column: PurchaseReportTableView.ActionColumn
            delegate: Loader {
                readonly property var modelData: {
                    "client_id": model.client_id,
                    "transaction_id": model.transaction_id
                }

                sourceComponent: purchaseReportTableView.buttonRow
            }
        }
    }
}
