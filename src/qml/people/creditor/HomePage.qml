import "../../common"
import "../../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

RRUi.Page {
    //    RemoveCreditorConfirmationDialog {
    //        id: removeCreditorConfirmationDialog
    //        onAccepted: creditorListView.model.removeCreditor(modelData.creditor_id);
    //    }

    id: homePage

    objectName: "creditor/homePage"
    title: qsTr("Creditors")
    topPadding: 10
    bottomPadding: 10
    leftPadding: 20
    rightPadding: 20
    QQC2.StackView.onActivating: creditorListView.refresh()
    actions: [
        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("navigation/more_vert")
            text: qsTr("Add a new creditor.")
            onTriggered: bottomSheet.open()
            toolTip: qsTr("More options")
        }
    ]

    QtObject {
        id: privateProperties

        property int filterIndex: 0
        property int sortIndex: 0
        property var filterModel: ["Search by product name", "Search by category name"]
        property var sortModel: ["Sort in ascending order", "Sort in descending order"]
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
                onTriggered: homePage.push(Qt.resolvedUrl("NewCreditorPage.qml"))
            },
            FluidControls.Action {
                icon.source: FluidControls.Utils.iconUrl("image/edit")
                text: qsTr("Manage creditors.")
            }
        ]
    }

    //********************* ON-DEMAND ITEMS ****************************
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

    Connections {
        enabled: homePage.RRUi.ApplicationWindow.window !== null
        target: homePage.RRUi.ApplicationWindow.window.snackBar
        onClicked: creditorListView.undoLastCommit()
    }

    contentItem: FocusScope {
        RRUi.Card {
            topPadding: 4
            bottomPadding: 0
            leftPadding: 4
            rightPadding: 4
            width: 800

            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: parent.bottom
            }

            RRUi.FloatingActionButton {
                icon.source: FluidControls.Utils.iconUrl("content/add")
                onClicked: homePage.push(Qt.resolvedUrl("NewCreditorPage.qml"))

                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    margins: 24
                }

            }

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
                    model: [privateProperties.filterModel[privateProperties.filterIndex], privateProperties.sortModel[privateProperties.sortIndex]]

                    anchors {
                        top: searchBar.bottom
                        left: parent.left
                        right: parent.right
                    }

                }

                CreditorListView {
                    id: creditorListView

                    autoQuery: false
                    filterText: searchBar.text
                    filterColumn: RRModels.CreditorModel.PreferredNameColumn
                    onSuccess: {
                        switch (result.code) {
                        case RRModels.CreditorModel.RemoveCreditorSuccess:
                            homePage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Creditor removed"), qsTr("Undo"));
                            break;
                        case RRModels.CreditorModel.UndoRemoveCreditorSuccess:
                            homePage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Undo successful"));
                            break;
                        }
                    }

                    anchors {
                        top: filterChipListView.bottom
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }

                    buttonRow: Row {
                        RRUi.ToolButton {
                            id: editButton

                            icon.source: FluidControls.Utils.iconUrl("image/edit")
                            text: qsTr("Edit creditor")
                            onClicked: homePage.push(Qt.resolvedUrl("NewCreditorPage.qml"), {
                                "creditorId": parent.parent.modelData.creditor_id
                            })
                        }

                        RRUi.ToolButton {
                            id: deleteButton

                            icon.source: FluidControls.Utils.iconUrl("action/delete")
                            text: qsTr("Delete creditor")
                            onClicked: deleteConfirmationDialog.show(parent.parent.modelData)
                        }

                    }

                }

            }

        }

    }

}
