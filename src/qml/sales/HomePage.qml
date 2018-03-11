import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels

FluidControls.Page {
    id: homePage
    title: qsTr("Sales")
    topPadding: 0
    bottomPadding: 0
    leftPadding: 20
    rightPadding: 20

    actions: [
        FluidControls.Action {
            icon.name: "navigation/more_vert"
            text: qsTr("Add a new item.")
            onTriggered: bottomSheet.open();
            toolTip: qsTr("More options")
        }
    ]

    FluidControls.FloatingActionButton {
        Material.background: Material.LightGreen
        Material.foreground: "white"
        icon.name: "content/add"
        mini: true

        onClicked: homePage.push(Qt.resolvedUrl("NewSalePage.qml"));

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
                icon.name: "content/add"
                text: qsTr("Add a entry.")
                onTriggered: homePage.push(Qt.resolvedUrl("NewSalePage.qml"));
            },

            FluidControls.Action {
                icon.name: "image/edit"
                text: qsTr("View all sales entries.")
            }
        ]
    }
}
