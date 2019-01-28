import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr 1.0 as RR
import "../rrui" as RRUi

Flickable {
    id: homeListView

    signal linkActivated(var link, var properties)

    width: 800
    height: 800
    topMargin: 32
    bottomMargin: 32
    contentWidth: column.width
    contentHeight: column.height

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
                onClicked: homeListView.linkActivated(Qt.resolvedUrl("user/OtherUserAccountPage.qml"), { });
            }

            HomeRow {
                title: qsTr("Sign out")
                subtitle: qsTr("Oluchi")
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
}
