import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../singletons"

RRUi.DataTableView {
    id: saleTransactionDetailTableView

    property int transactionId: -1

    property alias busy: saleTransactionItemModel.busy
    property alias autoQuery: saleTransactionItemModel.autoQuery

    signal success(int successCode)
    signal error(int errorCode)

    function refresh() { saleTransactionItemModel.refresh(); }

    flickableDirection: TableView.VerticalFlick

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: saleTransactionDetailTableView.contentHeight > saleTransactionDetailTableView.height
    }

    model: RRModels.SaleTransactionItemModel {
        id: saleTransactionItemModel
        tableViewWidth: saleTransactionDetailTableView.widthWithoutMargins
        transactionId: saleTransactionDetailTableView.transactionId
        onSuccess: saleTransactionDetailTableView.success(successCode);
        onError: saleTransactionDetailTableView.error(errorCode);
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.SaleTransactionItemModel.CategoryColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: saleTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: saleTransactionDetailTableView.rowHeader.children[row].height

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
            column: RRModels.SaleTransactionItemModel.ItemColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: saleTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: saleTransactionDetailTableView.rowHeader.children[row].height

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
            column: RRModels.SaleTransactionItemModel.QuantityColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: saleTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: saleTransactionDetailTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: quantity + " " + unit
                }
            }
        }

        DelegateChoice {
            column: RRModels.SaleTransactionItemModel.UnitPriceColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: saleTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: saleTransactionDetailTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: Number(unit_price).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
                }
            }
        }

        DelegateChoice {
            column: RRModels.SaleTransactionItemModel.CostColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: saleTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: saleTransactionDetailTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: Number(cost).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
                }
            }
        }
    }
}
