import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr 1.0 as RR
import "../rrui" as RRUi

Flickable {
    id: homeListView

    signal pushRequested(var page, var properties, int operation)
    signal signedOut

    width: 800
    height: 800
    topMargin: 32
    bottomMargin: 32
    contentWidth: column.width
    contentHeight: column.height

    RR.UserProfile {
        id: userProfile
        onSuccess: {
            switch (successCode) {
            case RR.UserProfile.SignOutSuccess:
                homeListView.signedOut();
                break;
            }
        }
    }

    RR.Settings { id: settings }

    Column {
        id: column
        width: parent.width
        spacing: 32

        HomeCard {
            width: parent.width
            title: qsTr("Accounts")

            HomeRow {
                title: qsTr("Manage my account")
                onClicked: console.log("Account clicked!");
            }

            HomeRow {
                title: qsTr("Manage other accounts")
                onClicked: homeListView.pushRequested(Qt.resolvedUrl("user/OtherUserAccountPage.qml"), { },
                                                      QQC2.StackView.Transition);
            }

            HomeRow {
                title: qsTr("Change password")
                onClicked: homeListView.pushRequested(Qt.resolvedUrl("../user/PasswordChangePage.qml"),
                                                      { isFirstTime: false },
                                                      QQC2.StackView.Transition);
            }

            HomeRow {
                title: qsTr("Sign out")
                subtitle: userProfile.userName
                onClicked: signOutConfirmationDialog.show();
            }
        }

        HomeCard {
            width: parent.width

            title: qsTr("Appearance")

            HomeRow {
                title: qsTr("Themes")
                onClicked: console.log("Themes clicked!");
            }

            HomeRow {
                title: qsTr("Dark mode")
                control: RRUi.Switch {
                    checked: settings.darkModeActive
                    onToggled: settings.darkModeActive = checked;
                }
            }

            HomeRow {
                title: qsTr("Customize receipt")
                subtitle: qsTr("Edit the appearance of the receipts")
            }
        }

        HomeCard {
            width: parent.width
            title: qsTr("Notifications and alerts")

            HomeRow {
                title: qsTr("Turn on notifications")
                control: RRUi.Switch { checked: true }
            }

            HomeRow {
                title: qsTr("Manage notifications settings")
                onClicked: console.log("Manage notifications clicked!");
            }
        }

        HomeCard {
            width: parent.width
            title: qsTr("Network")

            HomeRow {
                title: qsTr("Change network name")
                subtitle: qsTr("Network name")
            }

            HomeRow {
                title: qsTr("Turn on discovery mode")
                subtitle: qsTr("Allow users on the same local network to find you")
                control: RRUi.Switch { checked: false }
            }

            HomeRow {
                title: qsTr("Device lock")
                subtitle: qsTr("Allow this device to access the remote server.")
                control: RRUi.Switch { checked: false }
            }
        }

        HomeCard {
            width: parent.width
            title: qsTr("Help")

            HomeRow {
                title: qsTr("User manual")
                subtitle: qsTr("Stuck? Click here")
            }

            HomeRow {
                title: qsTr("About Record Rack")
                subtitle: qsTr("Show information about Record Rack")
            }
        }
        //        populate: Transition {
        //            id: populateTransition

        //            SequentialAnimation {
        //                PropertyAction {
        //                    property: "y"
        //                    value: (populateTransition.ViewTransition.index + 1) * (populateTransition.ViewTransition.destination.y + 400)
        //                }

        //                PropertyAction {
        //                    property: "opacity"
        //                    value: .1
        //                }

        //                PauseAnimation { duration: 300 }
        //                ParallelAnimation {
        //                    YAnimator {
        //                        duration: 300
        //                        from: (populateTransition.ViewTransition.index + 1) * (populateTransition.ViewTransition.destination.y + 400)
        //                        to: populateTransition.ViewTransition.destination.y
        //                        easing.type: Easing.OutCubic
        //                    }

        //                    NumberAnimation {
        //                        target: populateTransition.ViewTransition.item
        //                        property: "opacity"
        //                        duration: 300
        //                        to: 1
        //                        easing.type: Easing.OutCubic
        //                    }
        //                }
        //            }
        //        }
    }

    RRUi.AlertDialog {
        id: signOutConfirmationDialog

        property var modelData: null

        width: 300
        text: qsTr("Are you sure you want to sign out?");
        standardButtons: RRUi.AlertDialog.Yes | RRUi.AlertDialog.No
        onAccepted: userProfile.signOut();
    }
}
