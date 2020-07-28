import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

RRUi.Page {
    id: homePage

    signal pushRequested(var page, var properties, int operation)
    signal signedOut()

    objectName: "settingsHomePage"
    title: qsTr("Settings")
    topPadding: 0
    bottomPadding: 0
    leftPadding: 20
    rightPadding: 20

    HomeListView {
        id: listView

        onPushRequested: homePage.push(page, properties, operation)
        onSignedOut: homePage.signedOut()

        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

    }

    actions: FluidControls.Action {
        icon.source: FluidControls.Utils.iconUrl("action/search")
        text: qsTr("Search settings")
        toolTip: text
    }

}
