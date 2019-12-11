import QtQuick 2.13
import Fluid.Controls 1.0 as FluidControls
import "../../../rrui" as RRUi
import "../../../stock" as Stock
import com.gecko.rr.models 1.0 as RRModels
import QtQuick.Controls.Material 2.3

RRUi.Card {
    id: itemSelectionSection

    signal addRequested(var item)
    signal viewRequested(int itemId)

    function refresh() {
        searchBar.clear();
        categoryListView.refresh();
    }

    implicitWidth: 800
    implicitHeight: 300

    padding: 20
    bottomPadding: 0
    Material.elevation: 2

    RRUi.ViewPreferences {
        id: viewPreferences
        filterModel: ["Search by item name", "Search by category name"]
    }

    FocusScope {
        anchors.fill: parent
        focus: true

        RRUi.SearchBar {
            id: searchBar
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
        }

        RRUi.ChipListView {
            id: filterChipListView
            height: 30
            anchors {
                top: searchBar.bottom
                left: parent.left
                right: parent.right
            }

            model: [
                viewPreferences.filterModel[viewPreferences.filterIndex],
                viewPreferences.sortOrderModel[viewPreferences.sortOrderIndex]
            ]

            onClicked: {
                switch (index) {
                case 0:
                    filterColumnDialog.open();
                    break;
                case 1:
                    sortOrderDialog.open();
                    break;
                }
            }
        }

        Stock.CategoryListView {
            id: categoryListView
            anchors {
                top: filterChipListView.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            filterText: searchBar.text
            filterColumn: RRModels.StockItemModel.ItemColumn

            buttonRow: Row {
                RRUi.ToolButton {
                    id: addToCartButton
                    width: FluidControls.Units.iconSizes.medium
                    height: width
                    icon.source: FluidControls.Utils.iconUrl("action/add_shopping_cart")
                    text: qsTr("Add to cart")
                    visible: modelData.quantity > 0
                    onClicked: itemSelectionSection.addRequested(modelData);
                }

                RRUi.ToolButton {
                    id: viewButton
                    width: FluidControls.Units.iconSizes.medium
                    height: width
                    icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                    text: qsTr("View details")
                    onClicked: itemSelectionSection.viewRequested(modelData.item_id);
                }
            }
        }

        FluidControls.Placeholder {
            visible: categoryListView.count === 0 && categoryListView.model.filterText !== ""
            anchors.centerIn: parent
            icon.source: FluidControls.Utils.iconUrl("action/search")
            text: qsTr("There are no results for this search query.")
        }

        FluidControls.Placeholder {
            visible: categoryListView.count === 0
            anchors.centerIn: parent
            icon.source: Qt.resolvedUrl("qrc:/icons/truck.svg")
            text: qsTr("No products available.")
        }
    }
}
