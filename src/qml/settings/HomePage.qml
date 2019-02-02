import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi

RRUi.Page {
    id: homePage
    objectName: "settingsHomePage"

    signal pushRequested(var page, var properties, int operation)
    signal signedOut

    title: qsTr("Settings")
    topPadding: 0
    bottomPadding: 0
    leftPadding: 20
    rightPadding: 20

    actions: FluidControls.Action {
        icon.source: FluidControls.Utils.iconUrl("action/search")
        text: qsTr("Search settings")
        toolTip: text
    }

    HomeListView {
        id: listView
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        onPushRequested: homePage.push(page, properties, operation);
        onSignedOut: homePage.signedOut();
    }
}
