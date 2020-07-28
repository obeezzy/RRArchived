import "../rrui" as RRUi
import "../singletons"
import Fluid.Controls 1.0 as FluidControls
import Qt.labs.qmlmodels 1.0
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

RRUi.DataTableView {
    id: saleReportTableView

    property alias busy: saleReportModel.busy
    property alias autoQuery: saleReportModel.autoQuery

    signal success(var result)
    signal error(var result)

    function refresh() {
        saleReportModel.refresh();
    }

    flickableDirection: TableView.VerticalFlick

    model: RRModels.SaleReportModel {
        id: saleReportModel

        tableViewWidth: saleReportTableView.widthWithoutMargins
        onSuccess: saleReportTableView.success(result)
        onError: saleReportTableView.error(result)
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: saleReportTableView.contentHeight > saleReportTableView.height
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.SaleReportModel.CategoryColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: saleReportTableView.columnHeader.children[column].width
                implicitHeight: saleReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: category
                    elide: Text.ElideRight

                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

        DelegateChoice {
            column: RRModels.SaleReportModel.ProductColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: saleReportTableView.columnHeader.children[column].width
                implicitHeight: saleReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: product
                    elide: Text.ElideRight

                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

        DelegateChoice {
            column: RRModels.SaleReportModel.QuantitySoldColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: saleReportTableView.columnHeader.children[column].width
                implicitHeight: saleReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: quantity_sold + " " + unit
                    elide: Text.ElideRight

                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

        DelegateChoice {
            column: RRModels.SaleReportModel.TotalAmountColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: saleReportTableView.columnHeader.children[column].width
                implicitHeight: saleReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: Number(total_amount).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
                    elide: Text.ElideRight

                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

    }

}
