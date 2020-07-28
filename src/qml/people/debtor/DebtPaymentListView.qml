import "../../rrui" as RRUi
import "../../singletons"
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

ListView {
    id: debtPaymentListView

    readonly property int topPadding: 4
    readonly property int bottomPadding: 8
    readonly property bool canScrollLeft: contentX > 0
    readonly property bool canScrollRight: contentX < (contentWidth - width)
    property bool animationEnabled: true
    property var parentDelegate: null

    signal editPaymentRequested(var paymentModelData)
    signal removePaymentRequested(int debtIndex, int paymentIndex, var paymentModel)

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

    leftMargin: 24
    rightMargin: 24
    spacing: 16
    orientation: ListView.Horizontal
    height: contentItem.childrenRect.height + topPadding + bottomPadding
    visible: debtPaymentListView.parentDelegate.ListView.view.count > 0

    anchors {
        left: parent.left
        right: parent.right
    }

    RRUi.PlaceholderLabel {
        visible: debtPaymentListView.count === 0 && !debtPaymentListView.parentDelegate.ListView.view.model.busy
        text: qsTr("No payment made for this transaction.")

        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
        }

    }

    model: RRModels.DebtPaymentModel {
        debtTransactionId: debtPaymentListView.parentDelegate.modelData.transaction_id
        debtTransactionRef: debtPaymentListView.parentDelegate.modelData.ref
    }

    delegate: RRUi.Card {
        id: paymentDelegate

        readonly property bool isFirstItem: index === 0
        readonly property bool isLastItem: index === ListView.view.count - 1
        readonly property int paymentIndex: index
        readonly property bool fresh: model.fresh
        readonly property bool dirty: model.dirty
        readonly property var modelData: model

        padding: 4
        width: 220
        height: 220

        // NOTE: Update due dates of all payments
        Connections {
            target: parentDelegate.ListView.view.model
            onDataChanged: {
                if (topLeft.row === parentDelegate.debtIndex && (paymentDelegate.fresh || paymentDelegate.dirty))
                    paymentDelegate.modelData.due_date = parentDelegate.modelData.due_date;

            }
        }

        Column {
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: parent.right
            }

            FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignHCenter
                text: qsTr("Payment #%1").arg(index + 1)
                font.bold: true

                anchors {
                    left: parent.left
                    right: parent.right
                }

            }

            FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignHCenter
                text: qsTr("Made on %1").arg(Qt.formatDateTime(created, "ddd MMM d, yyyy"))
                font.italic: true

                anchors {
                    left: parent.left
                    right: parent.right
                }

            }

            FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignHCenter
                text: qsTr("Paid %1").arg(Number(amount_paid).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName)))

                anchors {
                    left: parent.left
                    right: parent.right
                }

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
                visible: paymentDelegate.isLastItem && (paymentDelegate.fresh || paymentDelegate.dirty)
                text: qsTr("Edit payment")
                onClicked: debtPaymentListView.editPaymentRequested(paymentDelegate.modelData)
            }

            RRUi.ToolButton {
                icon.source: FluidControls.Utils.iconUrl("action/delete")
                width: FluidControls.Units.iconSizes.medium
                height: width
                visible: isLastItem && (paymentDelegate.fresh || paymentDelegate.dirty)
                text: qsTr("Remove payment")
                onClicked: debtPaymentListView.removePaymentRequested(debtPaymentListView.parentDelegate.debtIndex, paymentDelegate.paymentIndex, paymentDelegate.ListView.view.model)
            }

        }

    }

    add: Transition {
        SequentialAnimation {
            ParallelAnimation {
                NumberAnimation {
                    property: "y"
                    from: 100
                    duration: 300
                    easing.type: Easing.OutCubic
                }

                NumberAnimation {
                    property: "opacity"
                    to: 1
                    duration: 300
                    easing.type: Easing.OutCubic
                }

            }

            ScriptAction {
                script: debtPaymentListView.scrollToEnd()
            }

        }

    }

    remove: Transition {
        NumberAnimation {
            property: "opacity"
            to: 0
            duration: 300
            easing.type: Easing.OutCubic
        }

    }

    removeDisplaced: Transition {
        NumberAnimation {
            properties: "x,y"
            duration: 300
        }

    }

    Behavior on contentX {
        enabled: !debtPaymentListView.dragging && !debtPaymentListView.flicking && debtPaymentListView.animationEnabled

        NumberAnimation {
            easing.type: Easing.OutCubic
        }

    }

}
