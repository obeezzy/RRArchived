import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels

ListView {
    id: categoryListView

    enum FilterColumn {
        Category,
        Item
    }

    property Component buttonRow: null
    property string filterText: ""
    property int filterColumn: -1
    property int sortColumn: -1
    property alias busy: stockCategoryModel.busy

    signal success(int successCode)
    signal error(int errorCode)
    signal modelReset

    function refresh() { categoryListView.model.refresh(); }
    function undoLastCommit() { stockCategoryModel.unarchiveItem(privateProperties.lastRemovedItemId); }

    QtObject {
        id: privateProperties

        property int lastRemovedItemId: -1
    }

    topMargin: 20
    bottomMargin: 20
    clip: true
    visible: !model.busy

    model: RRModels.StockCategoryModel {
        id: stockCategoryModel
        filterText: categoryListView.filterColumn === -1 ? categoryListView.filterText : ""
        itemFilterText: categoryListView.filterColumn === RRModels.StockItemModel.ItemColumn ? categoryListView.filterText : ""
        onSuccess: {
            switch (successCode) {
            case RRModels.StockCategoryModel.RemoveStockCategorySuccess:
                categoryListView.success(RRModels.StockItemModel.RemoveStockItemSuccess);
                break;
            case RRModels.StockCategoryModel.UnarchiveItemSuccess:
                privateProperties.lastRemovedItemId = -1;
                categoryListView.success(RRModels.StockItemModel.UndoRemoveStockItemSuccess);
                break;
            }
        }

        onModelReset: categoryListView.modelReset();
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: categoryListView.contentHeight > categoryListView.height
    }

    delegate: RRUi.Card {
        id: categoryCard

        readonly property int row: index

        width: ListView.view.width
        height: column.height
        Material.elevation: 0

        Connections {
            target: stockCategoryModel
            onDataChanged: if (topLeft.row === categoryCard.row) itemTableView.refresh();
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

            ItemTableView {
                id: itemTableView
                anchors {
                    left: parent.left
                    right: parent.right
                    topMargin: 20
                    bottomMargin: 20
                }

                categoryId: category_id
                filterText: categoryListView.filterText
                filterColumn: categoryListView.filterColumn
                sortColumn: categoryListView.sortColumn
                buttonRow: categoryListView.buttonRow
                onItemRemoved: privateProperties.lastRemovedItemId = itemId;
                onSuccess: {
                    switch (successCode) {
                    case RRModels.StockItemModel.RemoveItemSuccess:
                        if (itemTableView.rows === 1)
                            stockCategoryModel.removeCategory(categoryCard.row);
                    }

                    categoryListView.success(successCode);
                }
                onError: categoryListView.error(errorCode);
                onModelReset: categoryListView.modelReset();
            }
        }
    }
}
