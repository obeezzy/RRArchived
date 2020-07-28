import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import com.gecko.rr.models 1.0 as RRModels

QQC2.Dialog {
    id: retrieveTransactionDialog

    property int transactionId: 0

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
            wrapMode: Text.Wrap
            text: qsTr("Please select a transaction below.")

            anchors {
                left: parent.left
                right: parent.right
            }

        }

        Item {
            height: 300

            anchors {
                left: parent.left
                right: parent.right
            }

            QQLayouts.RowLayout {
                id: listHeader

                height: 40
                spacing: 16
                visible: listView.count > 0

                anchors {
                    left: parent.left
                    right: parent.right
                }

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

                currentIndex: 0
                clip: true

                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                    top: listHeader.bottom
                }

                QQC2.ButtonGroup {
                    id: buttonGroup
                }

                FluidControls.ThinDivider {
                    visible: listView.count > 0

                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                    }

                }

                model: RRModels.SaleTransactionModel {
                    keys: RRModels.SaleTransactionModel.Suspended
                }

                delegate: FluidControls.ListItem {
                    width: ListView.view.width
                    height: Math.max(contentColumn.height, 40)
                    showDivider: true
                    onClicked: radioButton.checked = true

                    QQLayouts.RowLayout {
                        spacing: 16

                        anchors {
                            leftMargin: 8
                            fill: parent
                        }

                        QQC2.RadioButton {
                            id: radioButton

                            checked: listView.currentIndex === index
                            QQLayouts.Layout.alignment: Qt.AlignVCenter
                            QQLayouts.Layout.preferredWidth: 20
                            QQLayouts.Layout.preferredHeight: 40
                            QQC2.ButtonGroup.group: buttonGroup
                            onCheckedChanged: listView.currentIndex = index
                        }

                        Column {
                            id: contentColumn

                            QQLayouts.Layout.alignment: Qt.AlignVCenter
                            QQLayouts.Layout.fillWidth: true
                            spacing: 0

                            FluidControls.SubheadingLabel {
                                text: customer_name

                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }

                            }

                            FluidControls.SubheadingLabel {
                                visible: note !== ""
                                text: "\"" + note + "\""
                                color: "darkgray"

                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }

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
                horizontalAlignment: Qt.AlignHCenter
                text: qsTr("You have no saved transactions.")

                anchors {
                    left: parent.left
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }

            }

        }

        Row {
            spacing: 8
            anchors.right: parent.right

            QQC2.Button {
                flat: true
                text: qsTr("Cancel")
                onClicked: retrieveTransactionDialog.reject()
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
