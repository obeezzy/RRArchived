import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../common"

RRUi.Page {
    id: purchaseTransactionPage

    property date selectedDate: new Date()
    readonly property date todaysDate: new Date()

    function goToNextDay() {
        if (selectedDate < todaysDate) {
            searchBar.clear();
            purchaseTransactionListView.model.autoQuery = false;
            selectedDate = new Date(purchaseTransactionPage.selectedDate.getFullYear(),
                                    purchaseTransactionPage.selectedDate.getMonth(),
                                    purchaseTransactionPage.selectedDate.getDate() + 1);
            singleDateDialog.selectedDate = selectedDate;
            purchaseTransactionListView.model.autoQuery = true;
        }
    }

    function goToPreviousDay() {
        searchBar.clear();
        purchaseTransactionListView.model.autoQuery = false;
        selectedDate = new Date(purchaseTransactionPage.selectedDate.getFullYear(),
                                purchaseTransactionPage.selectedDate.getMonth(),
                                purchaseTransactionPage.selectedDate.getDate() - 1);
        singleDateDialog.selectedDate = selectedDate;
        purchaseTransactionListView.model.autoQuery = true;
    }

    title: qsTr("Purchase transactions made on %1").arg(Qt.formatDate(selectedDate, "dddd, MMMM d, yyyy"))

    actions: FluidControls.Action {
        icon.source: FluidControls.Utils.iconUrl("action/today")
        text: qsTr("Select date")
        onTriggered: singleDateDialog.open();
        toolTip: text
    }

    QtObject {
        id: privateProperties

        property int filterIndex: 0
        property int sortIndex: 0
        property var filterModel: ["Search by item name", "Search by category name"]
        property var sortModel: ["Sort in ascending order", "Sort in descending order"]
    }

    contentItem: FocusScope {
        focus: true

        RRUi.Card {
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: parent.bottom
            }

            topPadding: 4
            bottomPadding: 0
            leftPadding: 4
            rightPadding: 4

            width: 800

            contentItem: FocusScope {
                focus: true

                RRUi.SearchBar {
                    id: searchBar
                    focus: true
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                    }
                }

                RRUi.ChipListView {
                    id: filterChipListView
                    height: 30
                    anchors {
                        top: searchBar.bottom
                        left: parent.left
                        right: parent.right
                    }

                    model: [
                        privateProperties.filterModel[privateProperties.filterIndex],
                        privateProperties.sortModel[privateProperties.sortIndex]
                    ]
                }

                PurchaseTransactionListView {
                    id: purchaseTransactionListView
                    anchors {
                        top: filterChipListView.bottom
                        left: parent.left
                        right: parent.right
                        bottom: pageFooter.top
                        topMargin: 20
                    }

                    filterText: searchBar.text
                    filterColumn: RRModels.PurchaseTransactionModel.CustomerNameColumn

                    from: new Date(purchaseTransactionPage.selectedDate.getFullYear(),
                                   purchaseTransactionPage.selectedDate.getMonth(),
                                   purchaseTransactionPage.selectedDate.getDate())
                    to: new Date(purchaseTransactionPage.selectedDate.getFullYear(),
                                 purchaseTransactionPage.selectedDate.getMonth(),
                                 purchaseTransactionPage.selectedDate.getDate() + 1)

                    onSuccess: {
                        switch (successCode) {
                        case RRModels.PurchaseTransactionModel.RemoveTransactionSuccess:
                            purchaseTransactionPage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Transaction deleted."), qsTr("Undo"));
                            break;
                        case RRModels.PurchaseTransactionModel.UndoRemoveTransactionSuccess:
                            purchaseTransactionPage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Undo successful"));
                            break;
                        }
                    }

                    onError: {
                        switch (errorCode) {
                        case RRModels.PurchaseTransactionModel.RemoveTransactionError:
                            errorDialog.show(qsTr("Failed to remove transaction."), qsTr("Error"), errorCode);
                            break;
                        }
                    }

                    buttonRow: Row {
                        spacing: 0

                        RRUi.ToolButton {
                            id: viewButton
                            icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                            text: qsTr("View transaction details")
                            onClicked: purchaseTransactionItemDialog.show(parent.parent.modelData);
                        }

                        RRUi.ToolButton {
                            id: deleteButton
                            icon.source: FluidControls.Utils.iconUrl("action/delete")
                            text: qsTr("Delete transaction")
                            onClicked: deleteConfirmationDialog.show(parent.parent.modelData);
                        }
                    }
                }
            }

            Column {
                id: pageFooter
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }

                spacing: 0

                FluidControls.ThinDivider {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                }

                QQLayouts.RowLayout {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    QQC2.Button {
                        QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                        text: qsTr("Previous day")
                        onClicked: purchaseTransactionPage.goToPreviousDay();
                    }

                    QQC2.Button {
                        QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                        enabled: purchaseTransactionPage.selectedDate < purchaseTransactionPage.todaysDate
                        text: qsTr("Next day")
                        onClicked: purchaseTransactionPage.goToNextDay();
                    }
                }
            }
        }

        RRUi.AlertDialog {
            id: deleteConfirmationDialog

            property var modelData: null

            width: 300
            text: qsTr("Are you sure you want to remove this transaction?");
            standardButtons: RRUi.AlertDialog.Yes | RRUi.AlertDialog.No
            onAccepted: {
                purchaseTransactionListView.removeTransaction(modelData.row);
                deleteConfirmationDialog.modelData = null;
            }

            function show(modelData) {
                if (Object(modelData).hasOwnProperty("transaction_id")) {
                    text = qsTr("Are you sure you want to remove transaction <b>%1</b> from the list?").arg(modelData.transaction_id);
                    deleteConfirmationDialog.modelData = modelData;
                    open();
                }
            }
        }

        RRUi.ErrorDialog { id: errorDialog }

        PurchaseTransactionItemDialog { id: purchaseTransactionItemDialog }

        FluidControls.DatePickerDialog {
            id: singleDateDialog
            title: qsTr("Select date")
            selectedDate: purchaseTransactionPage.selectedDate
            standardButtons: QQC2.Dialog.Ok | QQC2.Dialog.Cancel
            onAccepted: {
                purchaseTransactionListView.model.autoQuery = false;
                purchaseTransactionPage.selectedDate = selectedDate;
                purchaseTransactionListView.model.autoQuery = true;
            }
        }
    }

    Connections {
        target: purchaseTransactionPage.RRUi.ApplicationWindow.window.snackBar !== undefined
                ? purchaseTransactionPage.RRUi.ApplicationWindow.window.snackBar : null
        onClicked: purchaseTransactionListView.undoLastCommit();
    }
}
