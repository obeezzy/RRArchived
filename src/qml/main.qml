import QtQuick 2.10
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.2 as QQC2
import Fluid.Controls 1.0 as FluidControls
import "rrui" as RRUi
import "common"
import "user"
import "models"

FluidControls.ApplicationWindow {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Record Rack")
    visibility: FluidControls.ApplicationWindow.Maximized
    appBar.visible: false
    pageStack.anchors.leftMargin: sidebar.expanded ? sidebar.width : 0
    color: Qt.lighter("lightgray")

    Material.accent: Material.LightGreen

    initialPage: LoginPage {
        QQC2.StackView.onRemoved: {
            mainWindow.appBar.visible = true;
            sidebar.expanded = true;
        }
        onLoggedIn: mainWindow.pageStack.replace(sidebar.model.get(0).fileName);
    }

    RRUi.Sidebar {
        id: sidebar
        expanded: false
        onCurrentIndexChanged: mainWindow.pageStack.replace(null, model.get(currentIndex).fileName);
    }
}
