import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import "../../singletons"

RRUi.Page {
    id: newUserPage

    property int userId: -1
    property RRModels.UserModel model: null
    readonly property bool isExistingUser: userId > 0

    title: newUserPage.isExistingUser ? qsTr("Edit user") : qsTr("Add user")
    topPadding: 10
    bottomPadding: 10
    leftPadding: 20
    rightPadding: 20

    /*!
        \qmlsignal void goBack(var event)

        This signal is emitted when the back action is triggered or back key is released.

        By default, the page will be popped from the page stack. To change the default
        behavior, for example to show a confirmation dialog, listen for this signal using
        \c onGoBack and set \c event.accepted to \c true. To dismiss the page from your
        dialog without triggering this signal and re-showing the dialog, call
        \c page.forcePop().
    */

    actions: FluidControls.Action {
        icon.source: FluidControls.Utils.iconUrl("action/note_add")
        toolTip: qsTr("Add note")
        text: qsTr("Add note")
    }

    RRUi.TransitionView {
        id: transitionView
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        width: 800

        component: RRUi.Card {
            id: transitionItem

            function validateInput() {
                if (userDetailSubView.password !== userDetailSubView.passwordConfirmation) {
                    errorDialog.show(qsTr("The passwords provided do not match."), qsTr("Error"));
                    return false;
                }

                return true;
            }

            padding: 0
            bottomPadding: 0

            QQC2.TabBar {
                id: tabBar
                anchors {
                    left: parent.left
                    right: parent.right
                }
                currentIndex: swipeView.currentIndex

                QQC2.TabButton { text: qsTr("User Details") }
                QQC2.TabButton { text: qsTr("User Privileges") }
            }

            QQC2.SwipeView {
                id: swipeView
                clip: true
                currentIndex: tabBar.currentIndex
                interactive: false

                anchors {
                    top: tabBar.bottom
                    left: parent.left
                    right: parent.right
                    bottom: pageFooter.top
                }

                UserDetailSubView { id: userDetailSubView; userId: newUserPage.userId }
                UserPrivilegeSubView { id: userPrivilegeSubView; userId: newUserPage.userId }
            }

            Column {
                id: pageFooter
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                    leftMargin: 4
                    rightMargin: 4
                }

                spacing: 0

                FluidControls.ThinDivider {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                }

                QQLayouts.RowLayout {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    QQC2.Button {
                        visible: tabBar.currentIndex === 1
                        QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                        text: qsTr("Load preset")
                    }
                    QQC2.Button {
                        visible: tabBar.currentIndex === 1
                        QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                        text: qsTr("Save as preset")
                    }

                    Item {
                        QQLayouts.Layout.fillWidth: true
                        height: 1
                    }

                    QQC2.Button {
                        QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                        text: newUserPage.isExistingUser ? qsTr("Update user") : qsTr("Add user")
                        onClicked: {
                            if (!validateInput())
                                return;

                            userPrivilegeSubView.listView.imageUrl = userDetailSubView.imageUrl;
                            userPrivilegeSubView.listView.firstName = userDetailSubView.firstName;
                            userPrivilegeSubView.listView.lastName = userDetailSubView.lastName;
                            userPrivilegeSubView.listView.userName = userDetailSubView.userName;
                            userPrivilegeSubView.listView.password = userDetailSubView.password;
                            userPrivilegeSubView.listView.phoneNumber = userDetailSubView.phoneNumber;
                            userPrivilegeSubView.listView.emailAddress = userDetailSubView.emailAddress;
                            userPrivilegeSubView.listView.submit();
                        }
                    }
                }
            }

            RRUi.ErrorDialog {
                id: errorDialog;
                onAboutToHide: {
                    switch (errorCode) {
                    case RRModels.UserPrivilegeModel.NoFirstNameSetError:
                    case RRModels.UserPrivilegeModel.NoLastNameSetError:
                    case RRModels.UserPrivilegeModel.NoUserNameSetError:
                    case RRModels.UserPrivilegeModel.NoPasswordSetError:
                    case RRModels.UserPrivilegeModel.NoPhoneNumberSetError:
                    case RRModels.UserPrivilegeModel.NoEmailAddressSetError:
                        tabBar.currentIndex = 0;
                        break;
                    }
                }
            }

            Connections {
                target: userPrivilegeSubView.listView

                onSuccess: {
                    switch (successCode) {
                    case RRModels.UserPrivilegeModel.AddUserSuccess:
                        transitionView.trigger();
                        MainWindow.snackBar.show(qsTr("User added successfully."));
                        break;
                    case RRModels.UserPrivilegeModel.UpdateUserSuccess:
                        newUserPage.pop();
                        MainWindow.snackBar.show(qsTr("User updated successfully."));
                        break;
                    }
                }

                onError: {
                    switch (errorCode) {
                    case RRModels.UserPrivilegeModel.NoUserNameSetError:
                        errorDialog.show(qsTr("The user's user name must be provided."), qsTr("Error"), errorCode);
                        break;
                    case RRModels.UserPrivilegeModel.NoFirstNameSetError:
                        errorDialog.show(qsTr("The user's first name must be provided."), qsTr("Error"), errorCode);
                        break;
                    case RRModels.UserPrivilegeModel.NoLastNameSetError:
                        errorDialog.show(qsTr("The user's last name must be provided."), qsTr("Error"), errorCode);
                        break;
                    case RRModels.UserPrivilegeModel.NoPasswordSetError:
                        errorDialog.show(qsTr("The user's password must be provided."), qsTr("Error"), errorCode);
                        break;
                    case RRModels.UserPrivilegeModel.NoPhoneNumberSetError:
                        errorDialog.show(qsTr("The user's phone number must be provided."));
                        break;
                    case RRModels.UserPrivilegeModel.NoEmailAddressSetError:
                        errorDialog.show(qsTr("The user's email address must be provided."));
                        break;
                    case RRModels.UserPrivilegeModel.UserAlreadyExistsError:
                        errorDialog.show(qsTr("A user with the same user name already exists."));
                        break;
                    case RRModels.UserPrivilegeModel.ImageTooLargeError:
                        errorDialog.show(qsTr("The image provided is too large. The image must be less than 2 MB."));
                        break;
                    case RRModels.UserPrivilegeModel.UserPreviouslyArchivedError:
                        errorDialog.show(qsTr("A user with the same user name was previously archived. You must use a different user name or unarchive the user."));
                        break;
                    default:
                        errorDialog.show();
                    }
                }
            }
        }
    }
}
