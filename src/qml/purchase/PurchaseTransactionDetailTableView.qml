import "../rrui" as RRUi
import "../singletons"
import Fluid.Controls 1.0 as FluidControls
import Qt.labs.qmlmodels 1.0
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import com.gecko.rr.models 1.0 as RRModels

RRUi.DataTableView {
    id: purchaseTransactionDetailTableView

    property int transactionId: 0
    property alias busy: purchaseTransactionItemModel.busy
    property alias autoQuery: purchaseTransactionItemModel.autoQuery

    signal success(var result)
    signal error(var result)

    function refresh() {
        purchaseTransactionItemModel.refresh();
    }

    flickableDirection: TableView.VerticalFlick

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: purchaseTransactionDetailTableView.contentHeight > purchaseTransactionDetailTableView.height
    }

    model: RRModels.PurchaseTransactionItemModel {
        id: purchaseTransactionItemModel

        tableViewWidth: purchaseTransactionDetailTableView.widthWithoutMargins
        transactionId: purchaseTransactionDetailTableView.transactionId
        onSuccess: purchaseTransactionDetailTableView.success(result)
        onError: purchaseTransactionDetailTableView.error(result)
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.PurchaseTransactionItemModel.CategoryColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: purchaseTransactionDetailTableView.rowHeader.children[row].height

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
            column: RRModels.PurchaseTransactionItemModel.ItemColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: purchaseTransactionDetailTableView.rowHeader.children[row].height

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
            column: RRModels.PurchaseTransactionItemModel.QuantityColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: purchaseTransactionDetailTableView.rowHeader.children[row].height

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
            column: RRModels.PurchaseTransactionItemModel.UnitPriceColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: purchaseTransactionDetailTableView.rowHeader.children[row].height

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
            column: RRModels.PurchaseTransactionItemModel.CostColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: purchaseTransactionDetailTableView.rowHeader.children[row].height

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
