import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels

RRUi.DataTableView {
    id: productTableView

    property int categoryId: -1
    property int filterColumn: -1
    property string filterText: ""
    property int sortColumn: -1
    property Component buttonRow: null
    signal success(int successCode)
    signal error(int errorCode)
    signal productRemoved(int productId)
    signal modelReset

    function removeProduct(row) { stockProductModel.removeProduct(row); }
    function refresh() { stockProductModel.refresh(); }

    height: contentHeight + topMargin + bottomMargin
    columnSpacing: 8
    flickableDirection: TableView.VerticalFlick
    interactive: false

    model: RRModels.StockProductModel {
        id: stockProductModel
        tableViewWidth: productTableView.contentItem.width - productTableView.leftMargin * 2
        categoryId: productTableView.categoryId
        filterText: productTableView.filterText
        filterColumn: productTableView.filterColumn
        sortColumn: productTableView.sortColumn
        onProductRemoved: productTableView.productRemoved(productId);
        onSuccess: productTableView.success(successCode);
        onError: productTableView.error(errorCode);
        onModelReset: productTableView.modelReset();
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.StockProductModel.ImageColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: productTableView.columnHeader.children[column].width
                implicitHeight: productTableView.rowHeader.children[row].height

                RRUi.LetterCircleImage {
                    anchors.centerIn: parent
                    name: model.product
                    source: model.image_url
                    sourceSize: Qt.size(width, height)
                }
            }
        }

        DelegateChoice {
            column: RRModels.StockProductModel.ProductColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: productTableView.columnHeader.children[column].width
                implicitHeight: productTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: product
                }
            }
        }

        DelegateChoice {
            column: RRModels.StockProductModel.QuantityColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: productTableView.columnHeader.children[column].width
                implicitHeight: productTableView.rowHeader.children[row].height

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
            column: RRModels.StockProductModel.CostPriceColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: productTableView.columnHeader.children[column].width
                implicitHeight: productTableView.rowHeader.children[row].height

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
            column: RRModels.StockProductModel.ActionColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: productTableView.columnHeader.children[column].width
                implicitHeight: productTableView.rowHeader.children[row].height

                Loader {
                    readonly property var modelData: {
                        "category_id": category_id,
                        "category": category,
                        "product_id": model.product_id,
                        "product": model.product,
                        "unit_id": model.unit_id,
                        "unit": model.unit,
                        "quantity": model.quantity,
                        "retail_price": model.retail_price,
                        "cost_price": model.cost_price,
                        "row": row,
                        "productTableView": productTableView
                    }

                    anchors.centerIn: parent
                    sourceComponent: productTableView.buttonRow
                }
            }
        }
    }
}
