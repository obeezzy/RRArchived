import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../singletons"

RRUi.DataTableView {
    id: purchaseReportTableView

    property alias busy: purchaseReportModel.busy
    property alias autoQuery: purchaseReportModel.autoQuery

    signal success(int successCode)
    signal error(int errorCode)

    function refresh() { purchaseReportModel.refresh(); }

    flickableDirection: TableView.VerticalFlick

    model: RRModels.PurchaseReportModel {
        id: purchaseReportModel
        tableViewWidth: purchaseReportTableView.widthWithoutMargins
        onSuccess: purchaseReportTableView.success(successCode);
        onError: purchaseReportTableView.error(errorCode);
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
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: category
                }
            }
        }

        DelegateChoice {
            column: RRModels.PurchaseReportModel.ItemColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseReportTableView.columnHeader.children[column].width
                implicitHeight: purchaseReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: item
                }
            }
        }

        DelegateChoice {
            column: RRModels.PurchaseReportModel.QuantityBoughtColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseReportTableView.columnHeader.children[column].width
                implicitHeight: purchaseReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: quantity_bought + " " + unit
                }
            }
        }

        DelegateChoice {
            column: RRModels.PurchaseReportModel.TotalAmountColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseReportTableView.columnHeader.children[column].width
                implicitHeight: purchaseReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: Number(total_amount).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
                }
            }
        }
    }
}
