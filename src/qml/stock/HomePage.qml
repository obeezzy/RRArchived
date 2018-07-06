import QtQuick 2.9
import QtQuick.Controls 2.3 as QQC2
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

    background: Rectangle { }

    QtObject {
        id: privateProperties

        property int filterIndex: 0
        property int sortIndex: 0
        property var filterModel: ["Search by item name", "Search by category name"]
        property var sortModel: ["Sort in ascending order", "Sort in descending order"]
    }

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
            right: parent.right
        }
        text: qsTr("%1 result%2 found.").arg(categoryListView.model.totalItems).arg(categoryListView.model.totalItems === 1 ? "" : "s")
        font.bold: true
    }

    CategoryListView {
        id: categoryListView
        anchors {
            top: filterChipListView.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        filterText: searchBar.text
        filterColumn: RRModels.StockCategoryItemModel.ItemColumn
        bottomMargin: 100

        buttonRow: Row {
            spacing: 0

            RRUi.ToolButton {
                id: viewButton
                icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                width: FluidControls.Units.iconSizes.medium
                height: width
                text: qsTr("View details")
            }

            RRUi.ToolButton {
                id: editButton
                icon.source: FluidControls.Utils.iconUrl("image/edit")
                width: FluidControls.Units.iconSizes.medium
                height: width
                text: qsTr("Edit item")
                onClicked: homePage.push(Qt.resolvedUrl("NewItemPage.qml"), { "itemId": modelData.item_id });
            }

            RRUi.ToolButton {
                id: deleteButton
                icon.source: FluidControls.Utils.iconUrl("action/delete")
                width: FluidControls.Units.iconSizes.medium
                height: width
                text: qsTr("Delete item")
                onClicked: categoryListView.model.removeItem(modelData.item_id);
            }
        }

        onSuccess: {
            switch (successCode) {
            case RRModels.StockCategoryItemModel.RemoveItemSuccess:
                homePage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Item removed successfully."), qsTr("Undo"));
                break;
            case RRModels.StockCategoryItemModel.UndoRemoveItemSuccess:
                homePage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Undo successful."));
                break;
            }
        }
    }

    Connections {
        target: homePage.RRUi.ApplicationWindow.window.snackBar
        onClicked: categoryListView.undoLastCommit();
    }

    QQC2.BusyIndicator {
        anchors.centerIn: parent
        visible: categoryListView.model.busy
    }

    Loader {
        active: categoryListView.count == 0
        anchors.centerIn: parent
        sourceComponent: FluidControls.SubheadingLabel {
            color: Material.color(Material.Grey)
            text: /*categoryListView.model.filterText ? */qsTr("No results for this search query.")
                                                    //: qsTr("You have no items.\nClick the <img src='%1' width='20' height='20'/> button below to add items.").arg(FluidControls.Utils.iconUrl("content/add"))
            horizontalAlignment: Qt.AlignHCenter
        }
    }

    RRUi.FloatingActionButton {
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 24
        }

        text: qsTr("New stock item")
        icon.source: FluidControls.Utils.iconUrl("content/add")
        onClicked: homePage.push(Qt.resolvedUrl("NewItemPage.qml"));
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

    QQC2.StackView.onActivating: {
        searchBar.clear();
        categoryListView.refresh();
    }
}
