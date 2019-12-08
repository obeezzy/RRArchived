import QtQuick 2.13
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../singletons"

RRUi.DataTableView {
    id: stockReportTableView

    property alias busy: stockReportModel.busy
    property alias autoQuery: stockReportModel.autoQuery

    signal success(int successCode)
    signal error(int errorCode)

    function refresh() { stockReportModel.refresh(); }

    model: RRModels.StockReportModel {
        id: stockReportModel
        tableViewWidth: stockReportTableView.widthWithoutMargins
        onSuccess: stockReportTableView.success(successCode);
        onError: stockReportTableView.error(errorCode);
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: stockReportTableView.contentHeight > stockReportTableView.height
    }

    flickableDirection: TableView.VerticalFlick
    clip: true

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.StockReportModel.CategoryColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: stockReportTableView.columnHeader.children[column].width
                implicitHeight: stockReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: category
                    elide: Text.ElideRight
                }
            }
        }

        DelegateChoice {
            column: RRModels.StockReportModel.ItemColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: stockReportTableView.columnHeader.children[column].width
                implicitHeight: stockReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: item
                    elide: Text.ElideRight
                }
            }
        }

        DelegateChoice {
            column: RRModels.StockReportModel.OpeningStockQuantityColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: stockReportTableView.columnHeader.children[column].width
                implicitHeight: stockReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: opening_stock_quantity + " " + unit
                    elide: Text.ElideRight
                }
            }
        }

        DelegateChoice {
            column: RRModels.StockReportModel.QuantitySoldColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: stockReportTableView.columnHeader.children[column].width
                implicitHeight: stockReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: quantity_sold + " " + unit
                    elide: Text.ElideRight
                }
            }
        }

        DelegateChoice {
            column: RRModels.StockReportModel.QuantityBoughtColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: stockReportTableView.columnHeader.children[column].width
                implicitHeight: stockReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: quantity_bought + " " + unit
                    elide: Text.ElideRight
                }
            }
        }

        DelegateChoice {
            column: RRModels.StockReportModel.QuantityInStockColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: stockReportTableView.columnHeader.children[column].width
                implicitHeight: stockReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: quantity_in_stock + " " + unit
                    elide: Text.ElideRight
                }
            }
        }
    }
}
