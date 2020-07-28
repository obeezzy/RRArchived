import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts

QQLayouts.RowLayout {
    id: searchBar

    readonly property alias text: searchField.text

    function clear() {
        searchField.clear();
    }

    FluidControls.Icon {
        QQLayouts.Layout.alignment: Qt.AlignVCenter
        source: FluidControls.Utils.iconUrl("action/search")
        size: FluidControls.Units.iconSizes.medium
    }

    RRUi.TextField {
        id: searchField

        QQLayouts.Layout.fillWidth: true
        placeholderText: qsTr("Search")
        rightPadding: FluidControls.Units.iconSizes.smallMedium

        RRUi.ToolButton {
            id: clearButton

            opacity: searchField.text !== "" ? 1 : 0
            visible: opacity > 0
            width: FluidControls.Units.iconSizes.medium
            height: width
            icon.name: "content/clear"
            text: qsTr("Clear")
            onClicked: searchField.clear()

            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }

            Behavior on opacity {
                NumberAnimation {
                    property: "opacity"
                }

            }

        }

    }

}
