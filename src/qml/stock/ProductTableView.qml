import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../singletons"

RRUi.DataTableView {
    id: productTableView

    property int categoryId: 0
    property int filterColumn: -1
    property string filterText: ""
    property int sortColumn: -1
    property Component buttonRow: null
    signal success(var result)
    signal error(var result)
    signal productRemoved(int productId)
    signal modelReset

    function removeProduct(row) { productModel.removeProduct(row); }
    function refresh() { productModel.refresh(); }

    height: contentHeight + topMargin + bottomMargin
    columnSpacing: 8
    flickableDirection: TableView.VerticalFlick
    interactive: false

    model: RRModels.ProductModel {
        id: productModel
        tableViewWidth: productTableView.contentItem.width - productTableView.leftMargin * 2
        categoryId: productTableView.categoryId
        filterText: productTableView.filterText
        filterColumn: productTableView.filterColumn
        sortColumn: productTableView.sortColumn
        onProductRemoved: productTableView.productRemoved(productId);
        onSuccess: productTableView.success(result);
        onError: productTableView.error(result);
        onModelReset: productTableView.modelReset();
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.ProductModel.ImageColumn
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
            column: RRModels.ProductModel.ProductColumn
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
            column: RRModels.ProductModel.QuantityColumn
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
                    text: Number(quantity).toLocaleString(Qt.locale(GlobalSettings.localeName)) + " " + product_unit
                }
            }
        }

        DelegateChoice {
            column: RRModels.ProductModel.CostPriceColumn
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
                    text: Number(cost_price).toLocaleCurrencyString(Qt.locale(GlobalSettings.localeName))
                }
            }
        }

        DelegateChoice {
            column: RRModels.ProductModel.ActionColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: productTableView.columnHeader.children[column].width
                implicitHeight: productTableView.rowHeader.children[row].height

                Loader {
                    readonly property var modelData: {
                        "category_id": product_category_id,
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
