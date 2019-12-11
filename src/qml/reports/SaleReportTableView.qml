import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../singletons"

RRUi.DataTableView {
    id: saleReportTableView

    property alias busy: saleReportModel.busy
    property alias autoQuery: saleReportModel.autoQuery

    signal success(int successCode)
    signal error(int errorCode)

    function refresh() { saleReportModel.refresh(); }

    flickableDirection: TableView.VerticalFlick

    model: RRModels.SaleReportModel {
        id: saleReportModel
        tableViewWidth: saleReportTableView.widthWithoutMargins
        onSuccess: saleReportTableView.success(successCode);
        onError: saleReportTableView.error(errorCode);
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
            column: RRModels.SaleReportModel.ItemColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: saleReportTableView.columnHeader.children[column].width
                implicitHeight: saleReportTableView.rowHeader.children[row].height

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
            column: RRModels.SaleReportModel.QuantitySoldColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: saleReportTableView.columnHeader.children[column].width
                implicitHeight: saleReportTableView.rowHeader.children[row].height

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
            column: RRModels.SaleReportModel.TotalAmountColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: saleReportTableView.columnHeader.children[column].width
                implicitHeight: saleReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: Number(total_amount).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
                    elide: Text.ElideRight
                }
            }
        }
    }
}
