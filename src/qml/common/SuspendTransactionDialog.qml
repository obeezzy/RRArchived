import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Layouts 1.3 as QQLayouts
import "../rrui" as RRUi

QQC2.Dialog {
    id: suspendTransactionDialog

    property int transactionId: -1
    property string note: ""

    x: (QQC2.ApplicationWindow.contentItem.width - width) / 2
    y: (QQC2.ApplicationWindow.contentItem.height - height) / 2
    parent: QQC2.ApplicationWindow.contentItem

    implicitWidth: 600

    focus: true
    modal: true
    title: qsTr("Suspend transaction")

    Column {
        id: column
        anchors {
            left: parent.left
            right: parent.right
        }

        FluidControls.SubheadingLabel {
            id: label
            anchors {
                left: parent.left
                right: parent.right
            }
            wrapMode: Text.Wrap
            text: qsTr("Add a description of this transaction below, if you wish to.")
        }

        RRUi.TextField {
            id: noteField
            anchors {
                left: parent.left
                right: parent.right
            }

            placeholderText: qsTr("Add short description (optional)")
            focus: true
        }

        Row {
            spacing: 8
            anchors.right: parent.right

            QQC2.Button {
                flat: true
                text: qsTr("Cancel")
                onClicked: suspendTransactionDialog.reject();
            }

            QQC2.Button {
                text: qsTr("Suspend")
                onClicked: {
                    suspendTransactionDialog.note = noteField.text;
                    suspendTransactionDialog.accept();
                }
            }
        }
    }
}
