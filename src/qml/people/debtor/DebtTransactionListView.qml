import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import "../../singletons"

ListView {
    id: debtTransactionListView

    property Component buttonRow: null
    property string filterText: ""
    property int filterColumn: -1
    property int debtorId: -1

    signal success(int successCode)
    signal error(int errorCode)

    topMargin: 8
    bottomMargin: 8
    clip: true
    height: Math.max(contentHeight, 200)
    interactive: false

    model: RRModels.DebtTransactionModel {
        debtorId: debtTransactionListView.debtorId
        filterText: debtTransactionListView.filterText
        filterColumn: debtTransactionListView.filterColumn
        onSuccess: debtTransactionListView.success(successCode);
        onError: debtTransactionListView.error(errorCode);
    }

    QQC2.ScrollBar.vertical: QQC2.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: debtTransactionListView.contentHeight > debtTransactionListView.height
        size: .3
        width: 5
        contentItem: Rectangle {
            color: Material.color(Material.Grey)
            radius: width / 2
        }
    }

    delegate: Item {
        width: ListView.view.width
        height: transactionColumn.height

        Column {
            id: transactionColumn
            anchors {
                left: parent.left
                right: parent.right
            }

            spacing: 0
            leftPadding: 24

            FluidControls.HeadlineLabel {
                text: qsTr("Transaction #%1").arg(index + 1)
            }

            Item { width: 1; height: 4 } // Spacer

            FluidControls.SubheadingLabel {
                text: qsTr("Owes %1").arg(Number(current_balance)
                                          .toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName)))
            }

            FluidControls.SubheadingLabel {
                text: qsTr("Due on %1").arg(Qt.formatDateTime(due_date, "ddd MMM d, yyyy"))
                font.italic: true
            }

            Item { width: 1; height: 16 }

            ListView {
                anchors {
                    left: parent.left
                    right: parent.right
                }

                leftMargin: 24
                rightMargin: 24
                orientation: ListView.Horizontal
                height: 220
                model: payment_model
                visible: debtTransactionListView.count > 0
                delegate: FluidControls.Card {
                    id: paymentDelegate

                    readonly property bool isLastItem: index === ListView.view.count - 1

                    padding: 4
                    width: 220
                    height: ListView.view.height - 24

                    Column {
                        anchors {
                            verticalCenter: parent.verticalCenter
                            left: parent.left
                            right: parent.right
                        }

                        FluidControls.SubheadingLabel {
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                            horizontalAlignment: Qt.AlignHCenter
                            text: qsTr("Payment #%1").arg(index + 1)
                            font.bold: true
                        }

                        FluidControls.SubheadingLabel {
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                            horizontalAlignment: Qt.AlignHCenter
                            text: qsTr("Made on %1").arg(Qt.formatDateTime(created, "ddd MMM d, yyyy"))
                            font.italic: true
                        }

                        FluidControls.SubheadingLabel {
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                            horizontalAlignment: Qt.AlignHCenter
                            text: qsTr("Paid %1").arg(Number(amount_paid)
                                                      .toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName)))
                        }
                    }

                    Row {
                        spacing: 0
                        anchors {
                            right: parent.right
                            bottom: parent.bottom
                        }

                        RRUi.ToolButton {
                            icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                            width: FluidControls.Units.iconSizes.medium
                            height: width
                            text: qsTr("View payment details")
                        }

                        RRUi.ToolButton {
                            icon.source: FluidControls.Utils.iconUrl("image/edit")
                            width: FluidControls.Units.iconSizes.medium
                            height: width
                            visible: paymentDelegate.isLastItem
                            text: qsTr("Edit payment")
                        }

                        RRUi.ToolButton {
                            icon.source: FluidControls.Utils.iconUrl("action/delete")
                            width: FluidControls.Units.iconSizes.medium
                            height: width
                            visible: paymentDelegate.isLastItem
                            text: qsTr("Delete payment")
                        }
                    }
                }

                RRUi.PlaceholderLabel {
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        right: parent.right
                    }
                    visible: payment_model.count === 0 && !debtTransactionListView.model.busy
                    text: qsTr("No payment made for this transaction.");
                }

                add: Transition {
                    NumberAnimation { property: "y"; from: 100; duration: 300; easing.type: Easing.OutCubic }
                    NumberAnimation { property: "opacity"; to: 1; duration: 300; easing.type: Easing.OutCubic }
                }

                remove: Transition {
                    NumberAnimation { property: "opacity"; to: 0; duration: 300; easing.type: Easing.OutCubic }
                }

                removeDisplaced: Transition {
                    NumberAnimation { properties: "x,y"; duration: 300 }
                }
            }
        }

        Row {
            spacing: 0
            anchors {
                right: parent.right
                top: parent.top
            }

            RRUi.ToolButton {
                icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                text: qsTr("View transaction details")
            }

            RRUi.ToolButton {
                icon.source: FluidControls.Utils.iconUrl("image/edit")
                text: qsTr("Edit transaction")
            }

            RRUi.ToolButton {
                icon.source: FluidControls.Utils.iconUrl("action/delete")
                text: qsTr("Delete transaction")
            }
        }
    }

    add: Transition {
        NumberAnimation { property: "y"; from: 100; duration: 300; easing.type: Easing.OutCubic }
        NumberAnimation { property: "opacity"; to: 1; duration: 300; easing.type: Easing.OutCubic }
    }

    remove: Transition {
        NumberAnimation { property: "opacity"; to: 0; duration: 300; easing.type: Easing.OutCubic }
    }

    removeDisplaced: Transition {
        NumberAnimation { properties: "x,y"; duration: 300 }
    }

    RRUi.PlaceholderLabel {
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
        }
        visible: debtTransactionListView.count === 0 && !debtTransactionListView.model.busy
        text: qsTr("No transactions have been made by this debtor.");
    }

    QQC2.BusyIndicator { visible: debtTransactionListView.model.busy }

    function refresh() { debtTransactionListView.model.refresh(); }
    function undoLastCommit() { debtTransactionListView.model.undoLastCommit(); }
}
