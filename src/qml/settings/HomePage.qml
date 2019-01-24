import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi

RRUi.Page {
    id: homePage
    objectName: "settingsHomePage"

    signal linkActivated(var link, var properties)

    title: qsTr("Settings")
    topPadding: 0
    bottomPadding: 0
    leftPadding: 20
    rightPadding: 20

    HomeListView {
        id: listView
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
    }
}
