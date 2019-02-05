import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import Qt.labs.qmlmodels 1.0 as QQModels
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../singletons"

TableView {
    id: saleReportTableView

    property alias busy: saleReportModel.busy
    property alias autoQuery: saleReportModel.autoQuery
    property Component buttonRow: null

    enum Columns {
        RowIdColumn,
        CategoryColumn,
        ItemColumn,
        QuantitySoldColumn,
        TotalAmountColumn,
        ActionColumn
    }

    signal success(int successCode)
    signal error(int errorCode)

    function refresh() { saleReportModel.refresh(); }

    flickableDirection: TableView.VerticalFlick

    model: RRModels.SaleReportModel {
        id: saleReportModel
        onSuccess: saleReportTableView.success(successCode);
        onError: saleReportTableView.error(errorCode);
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: saleReportTableView.contentHeight > saleReportTableView.height
    }

    delegate: QQModels.DelegateChooser {
        QQModels.DelegateChoice {
            column: SaleReportTableView.RowIdColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: row + 1
            }
        }

        QQModels.DelegateChoice {
            column: SaleReportTableView.CategoryColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignLeft
                verticalAlignment: Qt.AlignVCenter
                text: category
            }
        }

        QQModels.DelegateChoice {
            column: SaleReportTableView.ItemColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignLeft
                verticalAlignment: Qt.AlignVCenter
                text: item
            }
        }

        QQModels.DelegateChoice {
            column: SaleReportTableView.QuantitySoldColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: quantity_sold + " " + unit
            }
        }

        QQModels.DelegateChoice {
            column: SaleReportTableView.TotalAmountColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: Number(total_amount).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
            }
        }

        QQModels.DelegateChoice {
            column: SaleReportTableView.ActionColumn
            delegate: Loader {
                readonly property var modelData: {
                    "client_id": model.client_id,
                    "transaction_id": model.transaction_id
                }

                sourceComponent: saleReportTableView.buttonRow
            }
        }
    }
}
