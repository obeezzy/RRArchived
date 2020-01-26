import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
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

    property string preferredName: ""
    property string primaryPhoneNumber: ""

    signal success(int successCode)
    signal error(int errorCode)
    signal editTransactionRequested(int debtIndex)
    signal removeTransactionRequested(int debtIndex)
    signal addPaymentRequested(int debtIndex)
    signal editPaymentRequested(int debtIndex, int paymentIndex)
    signal removePaymentRequested(int debtIndex, int paymentIndex)

    function addAlternatePhoneNumber(alternatePhoneNumber) { debtTransactionListView.model.addAlternatePhoneNumber(alternatePhoneNumber); }
    function removeAlternatePhoneNumber(row) { debtTransactionListView.model.removeAlternatePhoneNumber(row); }

    function addAddress(address) { debtTransactionListView.model.addAddress(address); }
    function removeAddress(row) { debtTransactionListView.model.removeAddress(row); }

    function addEmailAddress(emailAddress) { debtTransactionListView.model.addEmailAddress(email); }
    function removeEmailAddress(row) { debtTransactionListView.model.removeEmailAddress(row); }

    function addDebt(totalDebt, dueDateTime, note) { debtTransactionListView.model.addDebt(totalDebt, dueDateTime, note); }
    function updateDebt(debtIndex, dueDateTime, note) { debtTransactionListView.model.updateDebt(debtIndex, dueDateTime, note); }
    function removeDebt(debtIndex) { debtTransactionListView.model.removeDebt(debtIndex); }
    function addPayment(debtIndex, amount, note) { debtTransactionListView.model.addPayment(debtIndex, amount, note); }
    function updatePayment(debtIndex, paymentIndex, amount, note) { debtTransactionListView.model.updatePayment(debtIndex, paymentIndex, amount, note); }
    function removePayment(debtIndex, paymentIndex) { debtTransactionListView.model.removePayment(debtIndex, paymentIndex); }

    function submit() { return debtTransactionListView.model.submit(); }
    function refresh() { debtTransactionListView.model.refresh(); }
    function undoLastCommit() { debtTransactionListView.model.undoLastCommit(); }

    topMargin: 8
    bottomMargin: 8
    clip: true
    height: Math.max(contentHeight, 200)
    interactive: contentHeight > height

    model: RRModels.DebtTransactionModel {
        preferredName: debtTransactionListView.preferredName
        primaryPhoneNumber: debtTransactionListView.primaryPhoneNumber
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
        property bool isLastItem: index === ListView.view.count - 1

        width: ListView.view.width
        height: transactionColumn.height + transactionColumn.anchors.topMargin

        Column {
            id: transactionColumn

            readonly property int debtIndex: index

            anchors {
                top: parent.top
                topMargin: 16
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
                id: paymentListView

                readonly property int topPadding: 4
                readonly property int bottomPadding: 8
                readonly property bool canScrollLeft: contentX > 0
                readonly property bool canScrollRight: contentX < (contentWidth - width)
                property bool animationEnabled: true

                function scrollRight() {
                    if (!moving && canScrollRight) {
                        var pos = contentX;
                        positionViewAtIndex(indexAt(contentX + width, height / 2), ListView.Beginning);
                        var dest = contentX;
                        animationEnabled = false;
                        contentX = pos;
                        animationEnabled = true;
                        contentX = dest;
                    }
                }

                function scrollLeft() {
                    if (!moving && canScrollLeft) {
                        var pos = contentX;
                        positionViewAtIndex(indexAt(contentX, height / 2), ListView.End);
                        var dest = contentX;
                        animationEnabled = false;
                        contentX = pos;
                        animationEnabled = true;
                        contentX = dest;
                    }
                }

                function scrollToEnd() {
                    if (!moving && canScrollRight) {
                        var pos = contentX;
                        positionViewAtIndex(indexAt(contentX + contentWidth, height / 2), ListView.End);
                        var dest = contentX;
                        animationEnabled = false;
                        contentX = pos;
                        animationEnabled = true;
                        contentX = dest;
                    }
                }

                anchors {
                    left: parent.left
                    right: parent.right
                }

                leftMargin: 24
                rightMargin: 24
                spacing: 16
                orientation: ListView.Horizontal
                height: contentItem.childrenRect.height + topPadding + bottomPadding
                model: payment_model
                visible: debtTransactionListView.count > 0
                delegate: RRUi.Card {
                    id: paymentDelegate

                    readonly property bool isFirstItem: index === 0
                    readonly property bool isLastItem: index === ListView.view.count - 1
                    readonly property int paymentIndex: index

                    padding: 4
                    width: 220
                    height: 220

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
                            onClicked: debtTransactionListView.editPaymentRequested(transactionColumn.debtIndex, paymentDelegate.paymentIndex);
                        }

                        RRUi.ToolButton {
                            icon.source: FluidControls.Utils.iconUrl("action/delete")
                            width: FluidControls.Units.iconSizes.medium
                            height: width
                            visible: !paymentDelegate.isFirstItem && paymentDelegate.isLastItem
                            text: qsTr("Remove payment")
                            onClicked: debtTransactionListView.removePaymentRequested(transactionColumn.debtIndex, paymentDelegate.paymentIndex);
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
                    SequentialAnimation {
                        ParallelAnimation {
                            NumberAnimation { property: "y"; from: 100; duration: 300; easing.type: Easing.OutCubic }
                            NumberAnimation { property: "opacity"; to: 1; duration: 300; easing.type: Easing.OutCubic }
                        }

                        ScriptAction { script: paymentListView.scrollToEnd(); }
                    }
                }

                remove: Transition {
                    NumberAnimation { property: "opacity"; to: 0; duration: 300; easing.type: Easing.OutCubic }
                }

                removeDisplaced: Transition {
                    NumberAnimation { properties: "x,y"; duration: 300 }
                }

                Behavior on contentX {
                    enabled: !paymentListView.dragging && !paymentListView.flicking && paymentListView.animationEnabled
                    NumberAnimation { easing.type: Easing.OutCubic }
                }
            }

            FluidControls.ThinDivider { visible: !isLastItem }
        }

        RRUi.ArrowButton {
            anchors {
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: 40
                left: parent.left
            }

            opacity: paymentListView.canScrollLeft ? 1 : 0
            direction: "left"
            onClicked: paymentListView.scrollLeft();
        }

        RRUi.ArrowButton {
            anchors {
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: 40
                right: parent.right
            }

            opacity: paymentListView.canScrollRight ? 1 : 0
            direction: "right"
            onClicked: paymentListView.scrollRight();
        }

        Row {
            spacing: 0
            anchors {
                right: parent.right
                top: parent.top
            }

            RRUi.ToolButton {
                icon.source: FluidControls.Utils.iconUrl("content/add")
                text: qsTr("Add payment")
                onClicked: debtTransactionListView.addPaymentRequested(transactionColumn.debtIndex);
            }

            RRUi.ToolButton {
                icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                text: qsTr("View transaction details")
            }

            RRUi.ToolButton {
                icon.source: FluidControls.Utils.iconUrl("image/edit")
                text: qsTr("Edit transaction")
                onClicked: debtTransactionListView.editTransactionRequested(transactionColumn.debtIndex);
            }

            RRUi.ToolButton {
                icon.source: FluidControls.Utils.iconUrl("action/delete")
                text: qsTr("Remove transaction")
                onClicked: debtTransactionListView.removeTransactionRequested(transactionColumn.debtIndex);
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

    FluidControls.Placeholder {
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
        }
        visible: debtTransactionListView.count === 0 && !debtTransactionListView.model.busy
        icon.source: FluidControls.Utils.iconUrl("action/credit_card")
        text: qsTr("No transactions have been made by this debtor.");
    }

    QQC2.BusyIndicator { visible: debtTransactionListView.model.busy }
}
