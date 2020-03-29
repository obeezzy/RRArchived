import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Layouts 1.3 as QQLayouts
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels

RRUi.Popup {
    id: productDetailPopup

    property int productId: 0
    property bool editButtonVisible: true
    signal editRequested

    implicitWidth: 640
    implicitHeight: productDetailPopup.QQC2.ApplicationWindow.window.contentItem.height - 160
    padding: 0
    rightPadding: -1

    function show(productId) {
        if (productId === undefined)
            productId = -1;

        productDetailPopup.productId = productId;
        open();
    }

    contentItem: Item {
        ListView {
            anchors.fill: parent
            clip: true
            header: headerComponent
            model: ProductDetailModel { id: productDetailModel; productId: productDetailPopup.productId }
            headerPositioning: ListView.PullBackHeader
            delegate: FluidControls.ListItem {
                QQLayouts.RowLayout {
                    anchors {
                        leftMargin: 64
                        rightMargin: 64
                        fill: parent
                    }
                    spacing: 0

                    FluidControls.SubheadingLabel {
                        QQLayouts.Layout.preferredWidth: parent.width / 2
                        QQLayouts.Layout.fillHeight: true
                        text: title
                        color: Material.theme === Material.Dark ? "darkgray" : Qt.darker("darkgray")
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                    }

                    FluidControls.SubheadingLabel {
                        QQLayouts.Layout.preferredWidth: parent.width / 2
                        QQLayouts.Layout.fillHeight: true
                        text: value
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
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
                    visible: productDetailPopup.editButtonVisible
                    icon.source: FluidControls.Utils.iconUrl("image/edit")
                    icon.color: "white"
                    text: qsTr("Edit product")
                    onClicked: productDetailPopup.editRequested();
                }

                RRUi.ToolButton {
                    icon.source: FluidControls.Utils.iconUrl("navigation/close")
                    icon.color: "white"
                    text: qsTr("Close")
                    onClicked: productDetailPopup.close();
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
                name: productDetailModel.record.product
                source: productDetailModel.record.imageUrl
                sourceSize: Qt.size(width, height)
                font.pixelSize: 24
            }

            Column {
                anchors {
                    left: parent.left
                    bottom: parent.bottom
                    margins: 24
                }

                FluidControls.SubheadingLabel {
                    text: productDetailModel.record.category
                    color: "white"
                }

                FluidControls.HeadlineLabel {
                    text: productDetailModel.record.product
                    color: "white"
                }
            }
        }
    }
}
