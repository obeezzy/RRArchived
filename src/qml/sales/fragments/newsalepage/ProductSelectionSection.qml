import "../../../rrui" as RRUi
import "../../../stock" as Stock
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.13
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

RRUi.Card {
    id: productSelectionSection

    signal addRequested(var product)
    signal viewRequested(int productId)

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

        filterModel: ["Search by product name", "Search by category name"]
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
            model: [viewPreferences.filterModel[viewPreferences.filterIndex], viewPreferences.sortOrderModel[viewPreferences.sortOrderIndex]]
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

            anchors {
                top: searchBar.bottom
                left: parent.left
                right: parent.right
            }

        }

        Stock.ProductCategoryListView {
            id: categoryListView

            filterText: searchBar.text
            filterColumn: RRModels.StockProductModel.ProductColumn

            anchors {
                top: filterChipListView.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            buttonRow: Row {
                RRUi.ToolButton {
                    id: addToCartButton

                    width: FluidControls.Units.iconSizes.medium
                    height: width
                    icon.source: FluidControls.Utils.iconUrl("action/add_shopping_cart")
                    text: qsTr("Add to cart")
                    visible: modelData.quantity > 0
                    onClicked: productSelectionSection.addRequested(modelData)
                }

                RRUi.ToolButton {
                    id: viewButton

                    width: FluidControls.Units.iconSizes.medium
                    height: width
                    icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                    text: qsTr("View details")
                    onClicked: productSelectionSection.viewRequested(modelData.product_id)
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
