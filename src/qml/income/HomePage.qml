import "../common"
import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

RRUi.Page {
    id: homePage

    title: qsTr("Income")
    topPadding: 0
    bottomPadding: 0
    leftPadding: 20
    rightPadding: 20
    actions: [
        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("navigation/more_vert")
            text: qsTr("Add a new entry.")
            onTriggered: bottomSheet.open()
            toolTip: qsTr("More options")
        }
    ]

    HomeListView {
        id: homeListView

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            bottom: parent.bottom
        }

    }

    RRUi.FloatingActionButton {
        icon.source: FluidControls.Utils.iconUrl("content/add")
        text: qsTr("New income transaction")
        onClicked: homePage.push(Qt.resolvedUrl("NewIncomePage.qml"))

        anchors {
            right: homeListView.right
            bottom: homeListView.bottom
            margins: 24
        }

    }

    FluidControls.BottomSheetList {
        id: bottomSheet

        title: qsTr("What would you like to do?")
        actions: [
            FluidControls.Action {
                icon.source: FluidControls.Utils.iconUrl("content/add")
                text: qsTr("Add an entry.")
                onTriggered: homePage.push(Qt.resolvedUrl("NewIncomePage.qml"))
            },
            FluidControls.Action {
                icon.source: FluidControls.Utils.iconUrl("image/edit")
                text: qsTr("Manage income transactions.")
                onTriggered: homePage.push(Qt.resolvedUrl("IncomeTransactionPage.qml"))
            }
        ]
    }

    FluidControls.Placeholder {
        visible: homeListView.count == 0
        anchors.centerIn: parent
        icon.source: FluidControls.Utils.iconUrl("editor/bubble_chart")
        text: qsTr("No data available.")
    }

}
