import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../common"

RRUi.Page {
    id: homePage
    title: qsTr("Sales")
    topPadding: 0
    bottomPadding: 0
    leftPadding: 20
    rightPadding: 20

    actions: [
        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("navigation/more_vert")
            text: qsTr("Add a new item.")
            onTriggered: bottomSheet.open();
            toolTip: qsTr("More options")
        }
    ]

    contentItem: Item {
        HomeListView {
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: parent.bottom
            }
        }

        RRUi.FloatingActionButton {
            anchors {
                right: parent.right
                bottom: parent.bottom
                margins: 24
            }

            icon.source: FluidControls.Utils.iconUrl("content/add")
            text: qsTr("New sale transaction")
            onClicked: homePage.push(Qt.resolvedUrl("NewSalePage.qml"));
        }

        FluidControls.BottomSheetList {
            id: bottomSheet
            title: qsTr("What would you like to do?")

            actions: [
                FluidControls.Action {
                    icon.source: FluidControls.Utils.iconUrl("content/add")
                    text: qsTr("Add a entry.")
                    onTriggered: homePage.push(Qt.resolvedUrl("NewSalePage.qml"));
                },

                FluidControls.Action {
                    icon.name: FluidControls.Utils.iconUrl("image/edit")
                    text: qsTr("View all sales entries.")
                }
            ]
        }
    }
}
