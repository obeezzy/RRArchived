import ".."
import "../../common"
import "../../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import com.gecko.rr.models 1.0 as RRModels

RRUi.Page {
    //UserDetailDialog { id: userDetailDialog }

    id: otherUserAccountPage

    title: qsTr("Manage other user accounts")
    QQC2.StackView.onActivating: {
        searchBar.clear();
        userTableView.refresh();
    }

    RRUi.ViewPreferences {
        id: viewPreferences

        filterModel: ["Search by user name"]
        sortColumnModel: ["Sort by user name"]
    }

    contentItem: FocusScope {
        focus: true

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
                    model: viewPreferences.model

                    anchors {
                        top: searchBar.bottom
                        left: parent.left
                        right: parent.right
                    }

                }

                UserTableView {
                    id: userTableView

                    filterText: searchBar.text
                    filterColumn: RRModels.UserModel.UserColumn
                    onSuccess: {
                        switch (result.code) {
                        case RRModels.UserModel.RemoveUserSuccess:
                            MainWindow.snackBar.show(qsTr("User deleted."), qsTr("Undo"));
                            userTableView.refresh();
                            break;
                        case RRModels.UserModel.UndoRemoveUserSuccess:
                            MainWindow.snackBar.show(qsTr("Undo successful"));
                            userTableView.refresh();
                            break;
                        }
                    }
                    onError: {
                        switch (result.code) {
                        case RRModels.UserModel.InsufficientPrivilegeError:
                            break;
                        }
                    }

                    anchors {
                        top: filterChipListView.bottom
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                        topMargin: 20
                    }

                    buttonRow: Row {
                        spacing: 0

                        RRUi.ToolButton {
                            width: FluidControls.Units.iconSizes.medium
                            height: width
                            icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                            text: qsTr("View user details")
                        }

                        RRUi.ToolButton {
                            width: FluidControls.Units.iconSizes.medium
                            height: width
                            icon.source: FluidControls.Utils.iconUrl("image/edit")
                            text: qsTr("Edit user privileges")
                            onClicked: otherUserAccountPage.push(Qt.resolvedUrl("NewUserPage.qml"), {
                                "userId": modelData.user_id
                            })
                        }

                        RRUi.ToolButton {
                            width: FluidControls.Units.iconSizes.medium
                            height: width
                            icon.source: FluidControls.Utils.iconUrl("action/delete")
                            text: qsTr("Delete user")
                            onClicked: deleteConfirmationDialog.show(modelData)
                        }

                    }

                }

                RRUi.FloatingActionButton {
                    icon.source: FluidControls.Utils.iconUrl("content/add")
                    text: qsTr("Add user")
                    onClicked: otherUserAccountPage.push(Qt.resolvedUrl("NewUserPage.qml"))

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
                            text: qsTr("Add user.")
                        },
                        FluidControls.Action {
                            icon.source: FluidControls.Utils.iconUrl("image/edit")
                            text: qsTr("Manage income transactions.")
                        }
                    ]
                }

            }

        }

        RRUi.AlertDialog {
            id: deleteConfirmationDialog

            property var modelData: null

            function show(modelData) {
                if (Object(modelData).hasOwnProperty("user")) {
                    text = qsTr("Are you sure you want to delete \"<b>%1</b>\" from the list?").arg(modelData.user);
                    deleteConfirmationDialog.modelData = modelData;
                    open();
                }
            }

            width: 300
            text: qsTr("Are you sure you want to remove this user?")
            standardButtons: RRUi.AlertDialog.Yes | RRUi.AlertDialog.No
            onAccepted: {
                userTableView.removeUser(modelData.user);
                deleteConfirmationDialog.modelData = null;
            }
        }

    }

}
