import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels

RRUi.DataTableView {
    id: itemTableView

    property int categoryId: -1
    property int filterColumn: -1
    property string filterText: ""
    property int sortColumn: -1
    property Component buttonRow: null
    signal success(int successCode)
    signal error(int errorCode)
    signal itemRemoved(int itemId)
    signal modelReset

    function removeItem(row) { stockItemModel.removeItem(row); }
    function refresh() { stockItemModel.refresh(); }

    height: contentHeight + topMargin + bottomMargin
    columnSpacing: 8
    flickableDirection: TableView.VerticalFlick
    interactive: false

    model: RRModels.StockItemModel {
        id: stockItemModel
        tableViewWidth: itemTableView.contentItem.width - itemTableView.leftMargin * 2
        categoryId: itemTableView.categoryId
        filterText: itemTableView.filterText
        filterColumn: itemTableView.filterColumn
        sortColumn: itemTableView.sortColumn
        onItemRemoved: itemTableView.itemRemoved(itemId);
        onSuccess: itemTableView.success(successCode);
        onError: itemTableView.error(errorCode);
        onModelReset: itemTableView.modelReset();
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.StockItemModel.ImageColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: itemTableView.columnHeader.children[column].width
                implicitHeight: itemTableView.rowHeader.children[row].height

                RRUi.LetterCircleImage {
                    anchors.centerIn: parent
                    name: model.item
                    source: model.image_url
                    sourceSize: Qt.size(width, height)
                }
            }
        }

        DelegateChoice {
            column: RRModels.StockItemModel.ItemColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: itemTableView.columnHeader.children[column].width
                implicitHeight: itemTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: item
                }
            }
        }

        DelegateChoice {
            column: RRModels.StockItemModel.QuantityColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: itemTableView.columnHeader.children[column].width
                implicitHeight: itemTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: quantity + " " + unit
                }
            }
        }

        DelegateChoice {
            column: RRModels.StockItemModel.CostPriceColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: itemTableView.columnHeader.children[column].width
                implicitHeight: itemTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: Number(cost_price).toLocaleCurrencyString(Qt.locale("en_NG"))
                }
            }
        }

        DelegateChoice {
            column: RRModels.StockItemModel.ActionColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: itemTableView.columnHeader.children[column].width
                implicitHeight: itemTableView.rowHeader.children[row].height

                Loader {
                    readonly property var modelData: {
                        "category_id": category_id,
                        "category": category,
                        "item_id": model.item_id,
                        "item": model.item,
                        "unit_id": model.unit_id,
                        "unit": model.unit,
                        "quantity": model.quantity,
                        "retail_price": model.retail_price,
                        "cost_price": model.cost_price,
                        "row": row,
                        "itemTableView": itemTableView
                    }

                    anchors.centerIn: parent
                    sourceComponent: itemTableView.buttonRow
                }
            }
        }
    }
}
