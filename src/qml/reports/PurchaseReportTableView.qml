import "../rrui" as RRUi
import "../singletons"
import Fluid.Controls 1.0 as FluidControls
import Qt.labs.qmlmodels 1.0
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

RRUi.DataTableView {
    id: purchaseReportTableView

    property alias busy: purchaseReportModel.busy
    property alias autoQuery: purchaseReportModel.autoQuery

    signal success(var result)
    signal error(var result)

    function refresh() {
        purchaseReportModel.refresh();
    }

    flickableDirection: TableView.VerticalFlick

    model: RRModels.PurchaseReportModel {
        id: purchaseReportModel

        tableViewWidth: purchaseReportTableView.widthWithoutMargins
        onSuccess: purchaseReportTableView.success(result)
        onError: purchaseReportTableView.error(result)
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: purchaseReportTableView.contentHeight > purchaseReportTableView.height
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.PurchaseReportModel.CategoryColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseReportTableView.columnHeader.children[column].width
                implicitHeight: purchaseReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: category

                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

        DelegateChoice {
            column: RRModels.PurchaseReportModel.ProductColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseReportTableView.columnHeader.children[column].width
                implicitHeight: purchaseReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: product

                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

        DelegateChoice {
            column: RRModels.PurchaseReportModel.QuantityBoughtColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseReportTableView.columnHeader.children[column].width
                implicitHeight: purchaseReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: quantity_bought + " " + unit

                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

        DelegateChoice {
            column: RRModels.PurchaseReportModel.TotalAmountColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseReportTableView.columnHeader.children[column].width
                implicitHeight: purchaseReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: Number(total_amount).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))

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
