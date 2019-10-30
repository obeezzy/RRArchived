import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../singletons"

RRUi.DataTableView {
    id: purchaseTransactionDetailTableView

    property int transactionId: -1
    property alias busy: purchaseTransactionItemModel.busy
    property alias autoQuery: purchaseTransactionItemModel.autoQuery

    signal success(int successCode)
    signal error(int errorCode)

    function refresh() { purchaseTransactionItemModel.refresh(); }

    flickableDirection: TableView.VerticalFlick

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: purchaseTransactionDetailTableView.contentHeight > purchaseTransactionDetailTableView.height
    }

    model: RRModels.PurchaseTransactionItemModel {
        id: purchaseTransactionItemModel
        tableViewWidth: purchaseTransactionDetailTableView.widthWithoutMargins
        transactionId: purchaseTransactionDetailTableView.transactionId
        onSuccess: purchaseTransactionDetailTableView.success(successCode);
        onError: purchaseTransactionDetailTableView.error(errorCode);
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.PurchaseTransactionItemModel.CategoryColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: purchaseTransactionDetailTableView.rowHeader.children[row].height

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
            column: RRModels.PurchaseTransactionItemModel.ItemColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: purchaseTransactionDetailTableView.rowHeader.children[row].height

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
            column: RRModels.PurchaseTransactionItemModel.QuantityColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: purchaseTransactionDetailTableView.rowHeader.children[row].height

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
            column: RRModels.PurchaseTransactionItemModel.UnitPriceColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: purchaseTransactionDetailTableView.rowHeader.children[row].height

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
            column: RRModels.PurchaseTransactionItemModel.CostColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: purchaseTransactionDetailTableView.columnHeader.children[column].width
                implicitHeight: purchaseTransactionDetailTableView.rowHeader.children[row].height

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
