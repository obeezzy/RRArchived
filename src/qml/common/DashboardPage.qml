import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi

RRUi.Page {
    title: qsTr("Dashboard")
    topPadding: 0
    bottomPadding: 0
    leftPadding: 20
    rightPadding: 20

    ListView {
        anchors.fill: parent
        topMargin: 20
        bottomMargin: 20

        //model: RR.DashboardHomeModel { }

        delegate: FluidControls.Card {
            width: ListView.view.width
            height: 40
        }
    }

    FluidControls.Placeholder {
        anchors.centerIn: parent
        icon.source: FluidControls.Utils.iconUrl("editor/bubble_chart")
        text: qsTr("No data available.")
    }
}
