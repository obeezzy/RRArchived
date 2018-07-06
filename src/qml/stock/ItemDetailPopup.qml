import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Layouts 1.3 as QQLayouts
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels

RRUi.Popup {
    id: itemDetailPopup

    property int itemId: -1
    signal editRequested

    implicitWidth: 640
    implicitHeight: itemDetailPopup.QQC2.ApplicationWindow.window.contentItem.height - 160
    padding: 0
    rightPadding: -1

    function show(itemId) {
        if (itemId === undefined)
            itemId = -1;

        itemDetailPopup.itemId = itemId;
        open();
    }

    contentItem: Item {
        ListView {
            anchors.fill: parent
            clip: true
            header: headerComponent
            model: ItemDetailModel { id: itemDetailModel; itemId: itemDetailPopup.itemId }
            headerPositioning: ListView.PullBackHeader
            delegate: FluidControls.ListItem {
                QQLayouts.RowLayout {
                    anchors.centerIn: parent
                    spacing: 24

                    FluidControls.SubheadingLabel {
                        text: title
                        color: Qt.darker("darkgray")
                    }

                    FluidControls.SubheadingLabel {
                        text: value
                    }
                }

                showDivider: true
            }
        }
    }

    Component {
        id: headerComponent

        Rectangle {
            implicitWidth: ListView.view.width
            implicitHeight: 280
            color: Material.color(Material.DeepPurple)

            Row {
                anchors {
                    right: parent.right
                    top: parent.top
                }
                RRUi.ToolButton {
                    icon.source: FluidControls.Utils.iconUrl("image/edit")
                    icon.color: "white"
                    text: qsTr("Edit item")
                    onClicked: itemDetailPopup.editRequested();
                }

                RRUi.ToolButton {
                    icon.source: FluidControls.Utils.iconUrl("navigation/close")
                    icon.color: "white"
                    text: qsTr("Close")
                    onClicked: itemDetailPopup.close();
                }
            }

            RRUi.LetterCircleImage {
                anchors {
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter
                    topMargin: 24
                }
                width: 160
                height: width
                backgroundColor: Qt.lighter("lightgray")
                foregroundColor: "darkgray"
                name: itemDetailModel.record.item
                font.pixelSize: 24
            }

            Column {
                anchors {
                    left: parent.left
                    bottom: parent.bottom
                    margins: 24
                }

                FluidControls.SubheadingLabel {
                    text: itemDetailModel.record.category
                    color: "white"
                }

                FluidControls.HeadlineLabel {
                    text: itemDetailModel.record.item
                    color: "white"
                }
            }
        }
    }
}
