import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../common"

RRUi.Page {
    id: expenseTransactionPage

    property date selectedDate: new Date()
    readonly property date todaysDate: new Date()

    function goToNextDay() {
        if (selectedDate < todaysDate) {
            searchBar.clear();
            expenseTransactionTableView.model.autoQuery = false;
            selectedDate = new Date(expenseTransactionPage.selectedDate.getFullYear(),
                                    expenseTransactionPage.selectedDate.getMonth(),
                                    expenseTransactionPage.selectedDate.getDate() + 1);
            singleDateDialog.selectedDate = selectedDate;
            expenseTransactionTableView.model.autoQuery = true;
        }
    }

    function goToPreviousDay() {
        searchBar.clear();
        expenseTransactionTableView.model.autoQuery = false;
        selectedDate = new Date(expenseTransactionPage.selectedDate.getFullYear(),
                                expenseTransactionPage.selectedDate.getMonth(),
                                expenseTransactionPage.selectedDate.getDate() - 1);
        singleDateDialog.selectedDate = selectedDate;
        expenseTransactionTableView.model.autoQuery = true;
    }

    title: qsTr("Income transactions made on %1").arg(Qt.formatDate(selectedDate, "dddd, MMMM d, yyyy"))

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
        property var filterModel: ["Search by product name", "Search by category name"]
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

                ExpenseTransactionTableView {
                    id: expenseTransactionTableView
                    anchors {
                        top: filterChipListView.bottom
                        left: parent.left
                        right: parent.right
                        bottom: pageFooter.top
                        topMargin: 20
                    }

                    filterText: searchBar.text
                    filterColumn: RRModels.ExpenseTransactionTableView.ClientNameColumn

                    from: new Date(expenseTransactionPage.selectedDate.getFullYear(),
                                   expenseTransactionPage.selectedDate.getMonth(),
                                   expenseTransactionPage.selectedDate.getDate())
                    to: new Date(expenseTransactionPage.selectedDate.getFullYear(),
                                 expenseTransactionPage.selectedDate.getMonth(),
                                 expenseTransactionPage.selectedDate.getDate() + 1)

                    onSuccess: {
                        switch (result.code) {
                        case RRModels.IncomeTransactionModel.ViewIncomeTransactionsSuccess:
                            expenseTransactionPage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Transaction deleted."), qsTr("Undo"));
                            break;
                        case RRModels.IncomeTransactionModel.UndoRemoveTransactionSuccess:
                            expenseTransactionPage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Undo successful"));
                            break;
                        }
                    }

                    buttonRow: Row {
                        RRUi.ToolButton {
                            id: viewButton
                            width: FluidControls.Units.iconSizes.medium
                            height: width
                            icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                            text: qsTr("View transaction details")
                            //onClicked: incomeTransactionItemDialog.show(parent.parent.modelData);
                        }

                        RRUi.ToolButton {
                            id: deleteButton
                            width: FluidControls.Units.iconSizes.medium
                            height: width
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
                        onClicked: expenseTransactionPage.goToPreviousDay();
                    }

                    QQC2.Button {
                        QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                        enabled: expenseTransactionPage.selectedDate < expenseTransactionPage.todaysDate
                        text: qsTr("Next day")
                        onClicked: expenseTransactionPage.goToNextDay();
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
                expenseTransactionTableView.removeTransaction(modelData.transaction_id);
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

        //IncomeTransactionItemDialog { id: incomeTransactionItemDialog }

        FluidControls.DatePickerDialog {
            id: singleDateDialog
            title: qsTr("Select date")
            selectedDate: expenseTransactionPage.selectedDate
            standardButtons: QQC2.Dialog.Ok | QQC2.Dialog.Cancel
            onAccepted: {
                expenseTransactionTableView.model.autoQuery = false;
                expenseTransactionPage.selectedDate = selectedDate;
                expenseTransactionTableView.model.autoQuery = true;
            }
        }
    }
}
