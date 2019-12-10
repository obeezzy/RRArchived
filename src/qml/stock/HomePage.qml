import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../common"
import "../singletons"

RRUi.Page {
    id: homePage
    title: qsTr("Stock")
    topPadding: 10
    bottomPadding: 0
    leftPadding: 20
    rightPadding: 20
    canGoBack: homePage.QQC2.StackView.view.depth

    actions: [
        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("navigation/more_vert")
            text: qsTr("More options")
            onTriggered: bottomSheet.open();
            toolTip: qsTr("More options")
        }
    ]

    QtObject {
        id: privateProperties

        property int filterIndex: 0
        property int sortIndex: 0
        property var filterModel: ["Filter by item", "Filter by category"]
        property var sortModel: ["Sort in ascending order", "Sort in descending order"]
    }

    RRModels.StockItemCountRecord {
        id: stockItemCountRecord
        filterText: categoryListView.filterText
        filterColumn: categoryListView.filterColumn
    }

    RRUi.Card {
        width: 840
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            bottom: parent.bottom
        }

        RRUi.SearchBar {
            id: searchBar
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                margins: 8
            }
        }

        RRUi.ChipListView {
            id: filterChipListView
            height: 30
            anchors {
                top: searchBar.bottom
                left: parent.left
                right: parent.right
                margins: 8
            }

            model: [
                privateProperties.filterModel[privateProperties.filterIndex],
                privateProperties.sortModel[privateProperties.sortIndex]
            ]

            onClicked: {
                switch (index) {
                case 0:
                    filterColumnDialogLoader.active = true;
                    break;
                case 1:
                    sortOrderDialogLoader.active = true;
                    break;
                }
            }
        }

        FluidControls.SubheadingLabel {
            anchors {
                top: searchBar.bottom
                right: categoryListView.right
                margins: 8
            }
            //visible: searchBar.text.trim() != ""
            text: qsTr("%1 result%2 found.").arg(stockItemCountRecord.itemCount).arg(stockItemCountRecord.itemCount === 1 ? "" : "s")
            font.bold: true
        }

        CategoryListView {
            id: categoryListView
            anchors {
                top: filterChipListView.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                margins: 8
            }

            filterText: searchBar.text
            filterColumn: RRModels.StockItemModel.ItemColumn
            sortColumn: RRModels.StockItemModel.ItemColumn
            bottomMargin: 100
            clip: true

            onModelReset: stockItemCountRecord.refresh();

            buttonRow: Row {
                RRUi.ToolButton {
                    id: viewButton
                    width: FluidControls.Units.iconSizes.medium
                    height: width
                    icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                    text: qsTr("View details")
                    onClicked: itemDetailPopup.show(modelData.item_id);
                }

                RRUi.ToolButton {
                    id: editButton
                    width: FluidControls.Units.iconSizes.medium
                    height: width
                    icon.source: FluidControls.Utils.iconUrl("image/edit")
                    text: qsTr("Edit item")
                    onClicked: homePage.push(Qt.resolvedUrl("NewItemPage.qml"), { "itemId": modelData.item_id });
                }

                RRUi.ToolButton {
                    id: deleteButton
                    width: FluidControls.Units.iconSizes.medium
                    height: width
                    icon.source: FluidControls.Utils.iconUrl("action/delete")
                    text: qsTr("Delete item")
                    onClicked: modelData.itemTableView.removeItem(modelData.row);
                }
            }

            onSuccess: {
                switch (successCode) {
                case RRModels.StockItemModel.RemoveItemSuccess:
                    MainWindow.snackBar.show(qsTr("Item removed successfully."), qsTr("Undo"));
                    break;
                case RRModels.StockItemModel.UndoRemoveItemSuccess:
                    MainWindow.snackBar.show(qsTr("Undo successful."));
                    break;
                }
            }
        }

        RRUi.FloatingActionButton {
            anchors {
                right: parent.right
                bottom: parent.bottom
                margins: 24
            }

            text: qsTr("New product")
            icon.source: FluidControls.Utils.iconUrl("content/add")
            onClicked: homePage.push(Qt.resolvedUrl("NewItemPage.qml"));
        }
    }

    Connections {
        target: MainWindow.snackBar
        onClicked: categoryListView.undoLastCommit();
    }

    QQC2.BusyIndicator {
        anchors.centerIn: parent
        visible: categoryListView.busy
    }

    ItemDetailPopup {
        id: itemDetailPopup
        onEditRequested: {
            close();
            homePage.push(Qt.resolvedUrl("NewItemPage.qml"), { "itemId": itemId });
        }
    }

    FluidControls.BottomSheetList {
        id: bottomSheet
        title: qsTr("What would you like to do?")

        actions: [
            FluidControls.Action {
                icon.source: FluidControls.Utils.iconUrl("content/add")
                text: qsTr("Add a new item.")
                onTriggered: homePage.push(Qt.resolvedUrl("NewItemPage.qml"));
            },

            FluidControls.Action {
                icon.source: FluidControls.Utils.iconUrl("image/edit")
                text: qsTr("View all items.")
            }
        ]
    }

    FluidControls.Placeholder {
        visible: !categoryListView.busy && categoryListView.count == 0 && searchBar.text !== ""
        anchors.centerIn: parent
        icon.source: FluidControls.Utils.iconUrl("action/search")
        text: qsTr("No results for this search query.")
    }

    FluidControls.Placeholder {
        visible: !categoryListView.busy && categoryListView.count == 0 && searchBar.text === ""
        anchors.centerIn: parent
        icon.source: Qt.resolvedUrl("qrc:/icons/truck.svg")
        text: qsTr("No products available.")
    }

    QQC2.StackView.onActivating: {
        searchBar.clear();
        categoryListView.refresh();
    }
}
