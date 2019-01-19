import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Controls.Material 2.3

FluidControls.Card {
    id: homeCard

    property string cardTitle: ""
    property string subtitle: ""
    property string shortDescription: ""
    property url imageUrl: ""
    property url iconUrl: ""
    property var breadcrumb: []
    property var model: null
    property bool dismissable: true
    default property alias content: dataColumn.data

    signal viewRequested(var breadcrumbs)
    signal dismissRequested

    implicitWidth: 200
    implicitHeight: dataColumn.height + topPadding + bottomPadding + buttonRow.height
    padding: 16

    Column {
        id: dataColumn
        spacing: 0
        anchors {
            left: parent.left
            right: parent.right
        }

        FluidControls.HeadlineLabel {
            anchors {
                left: parent.left
                right: parent.right
            }
            text: homeCard.cardTitle
            visible: text != ""

            FluidControls.Icon {
                anchors {
                    margins: 2
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                }

                source: homeCard.iconUrl
                width: height
            }
        }

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            source: homeCard.imageUrl
            fillMode: Image.PreserveAspectFit
        }

        FluidControls.SubheadingLabel {
            anchors {
                left: parent.left
                right: parent.right
            }
            text: homeCard.subtitle
            color: "darkgray"
            visible: text != ""
        }

        Item { width: 1; height: 16 }

        FluidControls.BodyLabel {
            anchors {
                left: parent.left
                right: parent.right
            }
            text: homeCard.shortDescription
            visible: text != ""
            wrapMode: Text.WordWrap
            maximumLineCount: 6
        }
    }

    Row {
        id: buttonRow
        spacing: 8
        anchors {
            bottom: parent.bottom
            right: parent.right
        }

        QQC2.Button {
            Material.foreground: "darkgray"
            flat: true
            visible: homeCard.dismissable
            text: qsTr("Dismiss")
            onClicked: homeCard.dismissRequested();
        }

        QQC2.Button {
            flat: true
            text: qsTr("View")
            onClicked: homeCard.viewRequested(homeCard.breadcrumb);
        }
    }
}
