import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

RRUi.Page {
    id: dashboardPage

    signal pushRequested(var page, var properties, int operation)

    objectName: "dashboardPage"
    title: qsTr("Dashboard")
    topPadding: 0
    bottomPadding: 0
    leftPadding: 20
    rightPadding: 20

    DashboardListView {
        id: listView

        onViewRequested: dashboardPage.pushRequested(breadcrumbs, {
        }, QQC2.StackView.Transition)

        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

    }

    FluidControls.Placeholder {
        visible: listView.count == 0
        anchors.centerIn: parent
        icon.source: FluidControls.Utils.iconUrl("editor/bubble_chart")
        text: qsTr("No data available.")
    }

}
