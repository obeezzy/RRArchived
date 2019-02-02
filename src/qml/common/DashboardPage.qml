import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi

RRUi.Page {
    id: dashboardPage
    objectName: "dashboardPage"

    signal pushRequested(var page, var properties, int operation)

    title: qsTr("Dashboard")
    topPadding: 0
    bottomPadding: 0
    leftPadding: 20
    rightPadding: 20

    DashboardListView {
        id: listView
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        onViewRequested: dashboardPage.pushRequested(breadcrumbs, { }, QQC2.StackView.Transition);
    }

    FluidControls.Placeholder {
        visible: listView.count == 0
        anchors.centerIn: parent
        icon.source: FluidControls.Utils.iconUrl("editor/bubble_chart")
        text: qsTr("No data available.")
    }
}
