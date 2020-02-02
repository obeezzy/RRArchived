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
    signal editTransactionRequested(var debtModelData)
    signal removeTransactionRequested(int debtIndex)
    signal addPaymentRequested(var paymentModel)
    signal editPaymentRequested(var paymentModelData)
    signal removePaymentRequested(int debtIndex, int paymentIndex, var paymentModel)

    function addAlternatePhoneNumber(alternatePhoneNumber) { debtTransactionListView.model.addAlternatePhoneNumber(alternatePhoneNumber); }
    function removeAlternatePhoneNumber(row) { debtTransactionListView.model.removeAlternatePhoneNumber(row); }

    function addAddress(address) { debtTransactionListView.model.addAddress(address); }
    function removeAddress(row) { debtTransactionListView.model.removeAddress(row); }

    function addEmailAddress(emailAddress) { debtTransactionListView.model.addEmailAddress(email); }
    function removeEmailAddress(row) { debtTransactionListView.model.removeEmailAddress(row); }

    function addDebt(totalDebt, dueDateTime, note) { debtTransactionListView.model.addDebt(totalDebt, dueDateTime, note); }
    function removeDebt(debtIndex) { debtTransactionListView.model.removeDebt(debtIndex); }

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
        id: transactionDelegate
        property bool isLastItem: index === ListView.view.count - 1
        readonly property bool paidInFull: (total_debt - debtPaymentListView.model.totalAmountPaid) === 0
        readonly property bool dueDateExceeded: {
            var today = new Date();
            return due_date.getTime() <= today.getTime();
        }
        readonly property bool dueDateHighlighted: dirty && dueDateExceeded
        readonly property var modelData: model
        readonly property int debtIndex: index


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

            Row {
                spacing: 8

                FluidControls.HeadlineLabel {
                    text: qsTr("Transaction #%1").arg(index + 1)
                }

                FluidControls.Icon {
                    anchors.verticalCenter: parent.verticalCenter
                    color: transactionDelegate.paidInFull ? Stylesheet.paidFullGreen : Stylesheet.debtorRed
                    source: transactionDelegate.paidInFull ? FluidControls.Utils.iconUrl("action/check_circle")
                                                           : FluidControls.Utils.iconUrl("alert/error")
                }
            }

            Item { width: 1; height: 4 } // Spacer

            FluidControls.SubheadingLabel {
                text: qsTr("Owes %1").arg(Number(total_debt - debtPaymentListView.model.totalAmountPaid)
                                          .toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName)))
            }

            FluidControls.SubheadingLabel {
                text: transactionDelegate.dueDateHighlighted ? qsTr("Due on %1 (Exceeded)").arg(Qt.formatDateTime(due_date, "ddd MMM d, yyyy"))
                                                             : qsTr("Due on %1").arg(Qt.formatDateTime(due_date, "ddd MMM d, yyyy"))
                font {
                    italic: true
                    bold: transactionDelegate.dueDateHighlighted
                }
                color: transactionDelegate.dueDateHighlighted ? Stylesheet.errorRed
                                                              : Material.theme === Material.Dark ? Stylesheet.white
                                                                                                 : Stylesheet.black
            }

            Item { width: 1; height: 16 }

            DebtPaymentListView {
                id: debtPaymentListView
                parentDelegate: transactionDelegate
                onEditPaymentRequested: debtTransactionListView.editPaymentRequested(paymentModelData);
                onRemovePaymentRequested: debtTransactionListView.removePaymentRequested(debtIndex,
                                                                                         paymentIndex,
                                                                                         paymentModel);
            }

            FluidControls.ThinDivider { visible: !isLastItem }
        }

        RRUi.ArrowButton {
            anchors {
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: 40
                left: parent.left
            }

            opacity: debtPaymentListView.canScrollLeft ? 1 : 0
            direction: "left"
            onClicked: debtPaymentListView.scrollLeft();
        }

        RRUi.ArrowButton {
            anchors {
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: 40
                right: parent.right
            }

            opacity: debtPaymentListView.canScrollRight ? 1 : 0
            direction: "right"
            onClicked: debtPaymentListView.scrollRight();
        }

        Row {
            spacing: 0
            anchors {
                right: parent.right
                top: parent.top
            }

            RRUi.ToolButton {
                visible: !transactionDelegate.paidInFull
                icon.source: FluidControls.Utils.iconUrl("content/add")
                text: qsTr("Add payment")
                onClicked: debtTransactionListView.addPaymentRequested(debtPaymentListView.model);
            }

            RRUi.ToolButton {
                icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                text: qsTr("View transaction details")
            }

            RRUi.ToolButton {
                icon.source: FluidControls.Utils.iconUrl("image/edit")
                text: qsTr("Edit transaction")
                onClicked: debtTransactionListView.editTransactionRequested(transactionDelegate.modelData);
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
