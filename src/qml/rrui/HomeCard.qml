import QtQuick 2.10
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Controls.Material 2.3

FluidControls.Card {
    id: homeCard

    property string title: ""
    property string subtitle: ""
    property var model: null
    property bool dismissable: true
    default property alias content: dataColumn.data

    signal viewButtonClicked
    signal dismissButtonClicked

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
            text: homeCard.title
            visible: text != ""
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
            onClicked: homeCard.dismissButtonClicked();
        }

        QQC2.Button {
            flat: true
            text: qsTr("View")
            onClicked: homeCard.viewButtonClicked();
        }
    }
}
