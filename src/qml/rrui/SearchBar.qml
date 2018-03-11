import QtQuick 2.10
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls

QQLayouts.RowLayout {
    id: searchBar

    readonly property alias text: searchField.text

    FluidControls.Icon {
        anchors.verticalCenter: parent.verticalCenter
        name: "action/search"
        size: FluidControls.Units.iconSizes.medium
    }

    QQC2.TextField {
        id: searchField
        QQLayouts.Layout.fillWidth: true
        placeholderText: qsTr("Search")
        rightPadding: FluidControls.Units.iconSizes.smallMedium

        FluidControls.ToolButton {
            id: clearButton
            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            opacity: searchField.text != "" ? 1 : 0
            visible: opacity > 0
            width: FluidControls.Units.iconSizes.medium
            height: width
            icon.name: "content/clear"
            onClicked: searchField.clear();

            Behavior on opacity { NumberAnimation { property: "opacity" } }
        }
    }

    function clear() { searchField.clear(); }
}
