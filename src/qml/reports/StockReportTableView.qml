import "../rrui" as RRUi
import "../singletons"
import Fluid.Controls 1.0 as FluidControls
import Qt.labs.qmlmodels 1.0
import QtQuick 2.13
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

RRUi.DataTableView {
    id: stockReportTableView

    property alias busy: stockReportModel.busy
    property alias autoQuery: stockReportModel.autoQuery

    signal success(var result)
    signal error(var result)

    function refresh() {
        stockReportModel.refresh();
    }

    flickableDirection: TableView.VerticalFlick
    clip: true

    model: RRModels.StockReportModel {
        id: stockReportModel

        tableViewWidth: stockReportTableView.widthWithoutMargins
        onSuccess: stockReportTableView.success(result)
        onError: stockReportTableView.error(result)
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: stockReportTableView.contentHeight > stockReportTableView.height
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.StockReportModel.CategoryColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: stockReportTableView.columnHeader.children[column].width
                implicitHeight: stockReportTableView.rowHeader.children[row].height

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
            column: RRModels.StockReportModel.ProductColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: stockReportTableView.columnHeader.children[column].width
                implicitHeight: stockReportTableView.rowHeader.children[row].height

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
            column: RRModels.StockReportModel.OpeningStockQuantityColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: stockReportTableView.columnHeader.children[column].width
                implicitHeight: stockReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: opening_stock_quantity + " " + unit
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
            column: RRModels.StockReportModel.QuantitySoldColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: stockReportTableView.columnHeader.children[column].width
                implicitHeight: stockReportTableView.rowHeader.children[row].height

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
            column: RRModels.StockReportModel.QuantityBoughtColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: stockReportTableView.columnHeader.children[column].width
                implicitHeight: stockReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: quantity_bought + " " + unit
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
            column: RRModels.StockReportModel.QuantityInStockColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: stockReportTableView.columnHeader.children[column].width
                implicitHeight: stockReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: quantity_in_stock + " " + unit
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
