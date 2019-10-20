import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import com.gecko.rr.models 1.0 as RRModels
import "../../rrui" as RRUi
import "../../common"

RRUi.Page {
    id: homePage
    objectName: "creditor/homePage"
    title: qsTr("Creditors")
    topPadding: 10
    bottomPadding: 10
    leftPadding: 20
    rightPadding: 20

    actions: [
        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("navigation/more_vert")
            text: qsTr("Add a new creditor.")
            onTriggered: bottomSheet.open();
            toolTip: qsTr("More options")
        }
    ]

    QtObject {
        id: privateProperties

        property int filterIndex: 0
        property int sortIndex: 0
        property var filterModel: ["Search by item name", "Search by category name"]
        property var sortModel: ["Sort in ascending order", "Sort in descending order"]
    }

    QQC2.StackView.onActivating: creditorListView.refresh();

    contentItem: FocusScope {
        RRUi.Card {
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
                }

                CreditorListView {
                    id: creditorListView
                    anchors {
                        top: filterChipListView.bottom
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }

                    autoQuery: false
                    filterText: searchBar.text
                    filterColumn: RRModels.CreditorModel.PreferredNameColumn

                    onSuccess: {
                        switch (successCode) {
                        case RRModels.CreditorModel.RemoveCreditorSuccess:
                            homePage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Creditor removed"), qsTr("Undo"));
                            break;
                        case RRModels.CreditorModel.UndoRemoveCreditorSuccess:
                            homePage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Undo successful"));
                            break;
                        }
                    }

                    buttonRow: Row {
                        RRUi.ToolButton {
                            id: editButton
                            icon.source: FluidControls.Utils.iconUrl("image/edit")
                            text: qsTr("Edit creditor")
                            onClicked: homePage.push(Qt.resolvedUrl("NewCreditorPage.qml"),
                                                     { "creditorId": parent.parent.modelData.creditor_id });
                        }

                        RRUi.ToolButton {
                            id: deleteButton
                            icon.source: FluidControls.Utils.iconUrl("action/delete")
                            text: qsTr("Delete creditor")
                            onClicked: deleteConfirmationDialog.show(parent.parent.modelData);
                        }
                    }
                }
            }

            RRUi.FloatingActionButton {
                icon.source: FluidControls.Utils.iconUrl("content/add")
                onClicked: homePage.push(Qt.resolvedUrl("NewCreditorPage.qml"));

                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    margins: 24
                }
            }
        }
    }

    QQC2.BusyIndicator {
        anchors.centerIn: parent
        visible: creditorListView.model.busy
    }

    FluidControls.BottomSheetList {
        id: bottomSheet
        title: qsTr("What would you like to do?")

        actions: [
            FluidControls.Action {
                icon.source: FluidControls.Utils.iconUrl("content/add")
                text: qsTr("Add a creditor.")
                onTriggered: homePage.push(Qt.resolvedUrl("NewCreditorPage.qml"));
            },

            FluidControls.Action {
                icon.source: FluidControls.Utils.iconUrl("image/edit")
                text: qsTr("Manage creditors.")
            }
        ]
    }

    /********************** ON-DEMAND ITEMS *****************************/
    FluidControls.Placeholder {
        visible: creditorListView.count == 0 && searchBar.text !== ""
        anchors.centerIn: parent
        icon.source: FluidControls.Utils.iconUrl("action/search")
        text: qsTr("No results for this search query.")
    }

    FluidControls.Placeholder {
        visible: creditorListView.count == 0 && searchBar.text === ""
        anchors.centerIn: parent
        icon.source: FluidControls.Utils.iconUrl("social/person")
        text: qsTr("No creditor entry has been made.")
    }

//    RemoveCreditorConfirmationDialog {
//        id: removeCreditorConfirmationDialog
//        onAccepted: creditorListView.model.removeCreditor(modelData.creditor_id);
//    }

    Connections {
        enabled: homePage.RRUi.ApplicationWindow.window !== null
        target: homePage.RRUi.ApplicationWindow.window.snackBar
        onClicked: creditorListView.undoLastCommit();
    }
}
