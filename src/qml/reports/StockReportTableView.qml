import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import Qt.labs.qmlmodels 1.0 as QQModels
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../singletons"

TableView {
    id: stockReportTableView

    property alias busy: stockReportModel.busy
    property alias autoQuery: stockReportModel.autoQuery
    property Component buttonRow: null

    enum Columns {
        RowIdColumn,
        CategoryColumn,
        ItemColumn,
        OpeningStockQuantityColumn,
        QuantitySoldColumn,
        QuantityBoughtColumn,
        QuantityInStockColumn,
        ActionColumn
    }

    signal success(int successCode)
    signal error(int errorCode)

    function refresh() { stockReportModel.refresh(); }

    flickableDirection: TableView.VerticalFlick

    model: RRModels.StockReportModel {
        id: stockReportModel
        onSuccess: stockReportTableView.success(successCode);
        onError: stockReportTableView.error(errorCode);
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: stockReportTableView.contentHeight > stockReportTableView.height
    }

    delegate: QQModels.DelegateChooser {
        QQModels.DelegateChoice {
            column: StockReportTableView.RowIdColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: row + 1
            }
        }

        QQModels.DelegateChoice {
            column: StockReportTableView.CategoryColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignLeft
                verticalAlignment: Qt.AlignVCenter
                text: category
            }
        }

        QQModels.DelegateChoice {
            column: StockReportTableView.ItemColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignLeft
                verticalAlignment: Qt.AlignVCenter
                text: item
            }
        }

        QQModels.DelegateChoice {
            column: StockReportTableView.OpeningStockQuantityColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: opening_stock_quantity + " " + unit
            }
        }

        QQModels.DelegateChoice {
            column: StockReportTableView.QuantitySoldColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: quantity_sold + " " + unit
            }
        }

        QQModels.DelegateChoice {
            column: StockReportTableView.QuantityBoughtColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: quantity_bought + " " + unit
            }
        }

        QQModels.DelegateChoice {
            column: StockReportTableView.QuantityInStockColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: quantity_in_stock + " " + unit
            }
        }

        QQModels.DelegateChoice {
            column: StockReportTableView.ActionColumn
            delegate: Loader {
                readonly property var modelData: {
                    "client_id": model.client_id,
                    "transaction_id": model.transaction_id
                }

                sourceComponent: stockReportTableView.buttonRow
            }
        }
    }
}
