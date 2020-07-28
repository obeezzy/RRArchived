import "../rrui" as RRUi
import "../singletons"
import Fluid.Controls 1.0 as FluidControls
import Qt.labs.qmlmodels 1.0
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import com.gecko.rr.models 1.0 as RRModels

RRUi.DataTableView {
    id: saleTransactionDetailTableView

    property int transactionId: 0
    property alias busy: saleTransactionItemModel.busy
    property alias autoQuery: saleTransactionItemModel.autoQuery

    signal success(var result)
    signal error(var result)

    function refresh() {
        saleTransactionItemModel.refresh();
    }

    flickableDirection: TableView.VerticalFlick

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: saleTransactionDetailTableView.contentHeight > saleTransactionDetailTableView.height
    }

    model: RRModels.SaleTransactionItemModel {
        id: saleTransactionItemModel

        tableViewWidth: saleTransactionDetailTableView.widthWithoutMargins
        transactionId: saleTransactionDetailTableView.transactionId
        onSuccess: saleTransactionDetailTableView.success(result)
        onError: saleTransactionDetailTableView.error(result)
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.SaleTransactionItemModel.CategoryColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: saleTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: saleTransactionDetailTableView.rowHeader.children[row].height

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
            column: RRModels.SaleTransactionItemModel.ItemColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: saleTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: saleTransactionDetailTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: item

                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

        DelegateChoice {
            column: RRModels.SaleTransactionItemModel.QuantityColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: saleTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: saleTransactionDetailTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: quantity + " " + unit

                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

        DelegateChoice {
            column: RRModels.SaleTransactionItemModel.UnitPriceColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: saleTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: saleTransactionDetailTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: Number(unit_price).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))

                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

        DelegateChoice {
            column: RRModels.SaleTransactionItemModel.CostColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: saleTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: saleTransactionDetailTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: Number(cost).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))

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
