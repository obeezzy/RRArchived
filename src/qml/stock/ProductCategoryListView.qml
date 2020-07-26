import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi

ListView {
    id: productCategoryListView

    enum FilterColumn {
        Category,
        Product
    }

    property Component buttonRow: null
    property string filterText: ""
    property int filterColumn: -1
    property int sortColumn: -1
    property alias busy: stockProductCategoryModel.busy

    signal success(var result)
    signal error(var result)
    signal modelReset

    function refresh() { productCategoryListView.model.refresh(); }
    function undoLastCommit() { stockProductCategoryModel.unarchiveProduct(privateProperties.lastRemovedProductId); }

    QtObject {
        id: privateProperties

        property int lastRemovedProductId: 0
    }

    topMargin: 20
    bottomMargin: 20
    clip: true
    visible: !model.busy

    model: RRModels.ProductCategoryModel {
        id: stockProductCategoryModel
        filterText: productCategoryListView.filterColumn === -1 ? productCategoryListView.filterText : ""
        productFilterText: productCategoryListView.filterColumn === RRModels.ProductModel.ProductColumn ? productCategoryListView.filterText
                                                                                                             : ""
        onSuccess: {
            switch (result.code) {
            case RRModels.ProductCategoryModel.RemoveCategorySuccess:
                productCategoryListView.success(RRModels.ProductModel.RemoveProductSuccess);
                break;
            case RRModels.ProductCategoryModel.UnarchiveProductSuccess:
                privateProperties.lastRemovedProductId = 0;
                productCategoryListView.success(RRModels.ProductModel.UndoRemoveProductSuccess);
                break;
            }
        }

        onModelReset: productCategoryListView.modelReset();
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: productCategoryListView.contentHeight > productCategoryListView.height
    }

    delegate: RRUi.Card {
        id: categoryCard

        readonly property int row: index

        width: ListView.view.width
        height: column.height
        Material.elevation: 0

        Connections {
            target: stockProductCategoryModel
            onDataChanged: if (topLeft.row === categoryCard.row) productTableView.refresh();
        }

        Column {
            id: column
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            Item {
                visible: categoryCard.row !== 0
                width: 1
                height: 24
            }

            FluidControls.HeadlineLabel {
                id: titleLabel
                text: category
            }

            ProductTableView {
                id: productTableView
                anchors {
                    left: parent.left
                    right: parent.right
                    topMargin: 20
                    bottomMargin: 20
                }

                categoryId: product_category_id
                filterText: productCategoryListView.filterText
                filterColumn: productCategoryListView.filterColumn
                sortColumn: productCategoryListView.sortColumn
                buttonRow: productCategoryListView.buttonRow
                onProductRemoved: privateProperties.lastRemovedProductId = productId;
                onSuccess: {
                    switch (result.code) {
                    case RRModels.ProductModel.RemoveProductSuccess:
                        if (productTableView.rows === 1)
                            stockProductCategoryModel.removeCategory(categoryCard.row);
                    }

                    productCategoryListView.success(result);
                }
                onError: productCategoryListView.error(result);
                onModelReset: productCategoryListView.modelReset();
            }
        }
    }
}
