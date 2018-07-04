import QtQuick 2.9
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import com.gecko.rr.models 1.0 as RRModels
import "../../rrui" as RRUi
import "../../common"

RRUi.Page {
    id: homePage
    objectName: "debtor/homePage"
    title: qsTr("Debtors")
    topPadding: 10
    bottomPadding: 10
    leftPadding: 20
    rightPadding: 20

    actions: [
        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("navigation/more_vert")
            text: qsTr("Add a new debtor.")
            onTriggered: bottomSheet.open();
            toolTip: qsTr("More options")
        }
    ]

    //background: Rectangle { }

    QtObject {
        id: privateProperties

        property int filterIndex: 0
        property int sortIndex: 0
        property var filterModel: ["Search by item name", "Search by category name"]
        property var sortModel: ["Sort in ascending order", "Sort in descending order"]
    }

    contentItem: FocusScope {
        FluidControls.Card {
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: parent.bottom
            }

            topPadding: 4
            bottomPadding: 0
            leftPadding: 4
            rightPadding: 4

            width: 800

            contentItem: FocusScope {
                focus: true

                RRUi.SearchBar {
                    id: searchBar
                    focus: true
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

                    //        onClicked: {
                    //            switch (index) {
                    //            case 0:
                    //                filterColumnDialogLoader.active = true;
                    //                break;
                    //            case 1:
                    //                sortOrderDialogLoader.active = true;
                    //                break;
                    //            }
                    //        }
                }

                DebtorListView {
                    id: debtorListView
                    anchors {
                        top: filterChipListView.bottom
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }

                    filterText: searchBar.text
                    filterColumn: RRModels.DebtorModel.PreferredNameColumn

                    onSuccess: {
                        switch (successCode) {
                        case RRModels.DebtorModel.DebtorRemoved:
                            snackBar.open(qsTr("Debtor removed"), qsTr("Undo"));
                            break;
                        case RRModels.DebtorModel.UndoDebtorRemoved:
                            snackBar.open(qsTr("Undo successful"), "");
                            break;
                        }
                    }

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
                            onClicked: deleteConfirmationDialog.show(parent.parent.modelData);
                        }
                    }
                }
            }
        }
    }

    QQC2.BusyIndicator {
        anchors.centerIn: parent
        visible: debtorListView.model.busy
    }

    FluidControls.FloatingActionButton {
        Material.background: Material.LightGreen
        Material.foreground: "white"
        icon.source: FluidControls.Utils.iconUrl("content/add")

        onClicked: homePage.push(Qt.resolvedUrl("NewDebtorPage.qml"),
                                 { "debtorModel": debtorListView.model });

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
                text: qsTr("Add a debtor.")
                onTriggered: homePage.push(Qt.resolvedUrl("NewDebtorPage.qml"));
            },

            FluidControls.Action {
                icon.source: FluidControls.Utils.iconUrl("image/edit")
                text: qsTr("Manage debtors.")
            }
        ]
    }

    /********************** ON-DEMAND ITEMS *****************************/
    FluidControls.SubheadingLabel {
        visible: debtorListView.count == 0
        anchors.centerIn: parent
        color: Material.color(Material.Grey)
        text: /*debtorListView.model.filterText ? */qsTr("No results for this search query.")
        //: qsTr("You have no items.\nClick the <img src='%1' width='20' height='20'/> button below to add items.").arg(FluidControls.Utils.iconUrl("content/add"))
        horizontalAlignment: Qt.AlignHCenter
    }

    RRUi.AlertDialog {
        id: deleteConfirmationDialog

        property var modelData: null

        width: 300
        text: qsTr("Are you sure you want to remove this debtor?");
        standardButtons: RRUi.AlertDialog.Yes | RRUi.AlertDialog.No
        onAccepted: {
            debtorListView.model.removeDebtor(modelData.debtor_id);
            deleteConfirmationDialog.modelData = null;
        }

        function show(modelData) {
            if (Object(modelData).hasOwnProperty("debtor_id")) {
                text = qsTr("Are you sure you want to remove '%1' from the list?").arg(modelData.preferred_name);
                deleteConfirmationDialog.modelData = modelData;
                open();
            }
        }
    }

    RRUi.SnackBar {
        id: snackBar
        onClicked: debtorListView.undoLastCommit();
    }
}
