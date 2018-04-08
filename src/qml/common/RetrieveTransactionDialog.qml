import QtQuick 2.10
import QtQuick.Controls 2.2 as QQC2
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Layouts 1.3 as QQLayouts
import com.gecko.rr.models 1.0 as RRModels

QQC2.Dialog {
    id: retrieveTransactionDialog

    property int transactionId: -1

    x: (QQC2.ApplicationWindow.contentItem.width - width) / 2
    y: (QQC2.ApplicationWindow.contentItem.height - height) / 2
    parent: QQC2.ApplicationWindow.contentItem

    implicitWidth: 600

    focus: true
    modal: true
    title: qsTr("Retrieve sale transaction")

    Column {
        id: column
        anchors {
            left: parent.left
            right: parent.right
        }

        FluidControls.SubheadingLabel {
            id: label
            visible: listView.count > 0
            anchors {
                left: parent.left
                right: parent.right
            }

            wrapMode: Text.Wrap
            text: qsTr("Please select a transaction below.")
        }

        Item {
            anchors {
                left: parent.left
                right: parent.right
            }
            height: 300

            QQLayouts.RowLayout {
                id: listHeader
                anchors {
                    left: parent.left
                    right: parent.right
                }
                height: 40
                spacing: 16
                visible: listView.count > 0

                Item {
                    QQLayouts.Layout.preferredWidth: 20
                    QQLayouts.Layout.preferredHeight: 40
                }

                FluidControls.SubheadingLabel {
                    QQLayouts.Layout.alignment: Qt.AlignVCenter
                    QQLayouts.Layout.fillWidth: true
                    text: qsTr("Customer name")
                }

                FluidControls.SubheadingLabel {
                    QQLayouts.Layout.alignment: Qt.AlignVCenter
                    text: qsTr("Total cost")
                }
            }

            ListView {
                id: listView
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                    top: listHeader.bottom
                }

                currentIndex: 0
                clip: true
                model: RRModels.SaleTransactionModel { keys: RRModels.SaleTransactionModel.Suspended }

                QQC2.ButtonGroup { id: buttonGroup }

                FluidControls.ThinDivider {
                    visible: listView.count > 0
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                    }
                }

                delegate: FluidControls.ListItem {
                    width: ListView.view.width
                    height: Math.max(contentColumn.height, 40)
                    showDivider: true

                    onClicked: radioButton.checked = true;

                    QQLayouts.RowLayout {
                        anchors {
                            leftMargin: 8
                            fill: parent
                        }
                        spacing: 16

                        QQC2.RadioButton {
                            id: radioButton
                            checked: listView.currentIndex === index
                            QQLayouts.Layout.alignment: Qt.AlignVCenter
                            QQLayouts.Layout.preferredWidth: 20
                            QQLayouts.Layout.preferredHeight: 40
                            QQC2.ButtonGroup.group: buttonGroup

                            onCheckedChanged: listView.currentIndex = index;
                        }

                        Column {
                            id: contentColumn
                            QQLayouts.Layout.alignment: Qt.AlignVCenter
                            QQLayouts.Layout.fillWidth: true
                            spacing: 0

                            FluidControls.SubheadingLabel {
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                                text: customer_name
                            }

                            FluidControls.SubheadingLabel {
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                                visible: note !== ""
                                text: "\"" + note + "\""
                                color: "darkgray"
                            }
                        }

                        FluidControls.SubheadingLabel {
                            QQLayouts.Layout.alignment: Qt.AlignVCenter
                            text: Number(total_cost).toLocaleCurrencyString(Qt.locale("en_NG"))
                        }
                    }
                }
            }

            FluidControls.SubheadingLabel {
                visible: listView.count == 0
                anchors {
                    left: parent.left
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                horizontalAlignment: Qt.AlignHCenter
                text: qsTr("You have no saved transactions.")
            }
        }

        Row {
            spacing: 8
            anchors.right: parent.right

            QQC2.Button {
                flat: true
                text: qsTr("Cancel")
                onClicked: retrieveTransactionDialog.reject();
            }

            QQC2.Button {
                text: qsTr("Retrieve")
                enabled: listView.count > 0
                onClicked: {
                    retrieveTransactionDialog.transactionId = listView.model.get(listView.currentIndex).transaction_id;
                    retrieveTransactionDialog.accept();
                }
            }
        }
    }
}
