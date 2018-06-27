import QtQuick 2.9
import QtQuick.Controls 2.2 as QQC2
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
            text: qsTr("Add a new item.")
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

            FluidControls.ToolButton {
                id: editButton
                icon.source: FluidControls.Utils.iconUrl("image/edit")
                width: FluidControls.Units.iconSizes.medium
                height: width
            }

            FluidControls.ToolButton {
                id: deleteButton
                icon.source: FluidControls.Utils.iconUrl("action/delete")
                width: FluidControls.Units.iconSizes.medium
                height: width
                onClicked: categoryListView.model.removeItem(modelData.item_id);
            }
        }

        onSuccess: {
            switch (successCode) {
            case RRModels.StockCategoryItemModel.ItemRemoved:
                snackBar.open(qsTr("Item removed successfully."), qsTr("Undo"));
                break;
            case RRModels.StockCategoryItemModel.UndoSuccessful:
                snackBar.open(qsTr("Undo successful."), "");
                break;
            }
        }
    }

    RRUi.SnackBar {
        id: snackBar
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

    FluidControls.FloatingActionButton {
        Material.background: Material.LightGreen
        Material.foreground: "white"
        icon.source: FluidControls.Utils.iconUrl("content/add")

        onClicked: homePage.push(Qt.resolvedUrl("NewItemPage.qml"));

        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 24
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

    QQC2.StackView.onActivated: categoryListView.refresh();
}
