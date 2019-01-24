import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi

Flickable {
    id: homeListView

    width: 800
    height: 800
    topMargin: 32
    bottomMargin: 32
    contentWidth: column.width
    contentHeight: column.height

    Column {
        id: column
        width: parent.width
        spacing: 32

        HomeCard {
            anchors {
                left: parent.left
                right: parent.right
            }

            title: qsTr("Accounts")

            HomeRow {
                title: qsTr("Manage my account")
                onClicked: console.log("Account clicked!");
            }

            HomeRow {
                title: qsTr("Manage other accounts")
                onClicked: console.log("Account clicked!");
            }

            HomeRow {
                title: qsTr("Sign out")
                subtitle: qsTr("Oluchi")
            }
        }

        HomeCard {
            anchors {
                left: parent.left
                right: parent.right
            }

            title: qsTr("Appearance")

            HomeRow {
                title: qsTr("Themes")
                onClicked: console.log("Themes clicked!");
            }
        }

        HomeCard {
            anchors {
                left: parent.left
                right: parent.right
            }
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
            anchors {
                left: parent.left
                right: parent.right
            }
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
    }
}
