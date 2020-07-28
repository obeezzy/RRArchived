import "../common"
import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import com.gecko.rr.models 1.0 as RRModels

RRUi.Page {
    //IncomeTransactionItemDialog { id: incomeTransactionItemDialog }

    id: incomeTransactionPage

    property date selectedDate: new Date()
    readonly property date todaysDate: new Date()

    function goToNextDay() {
        if (selectedDate < todaysDate) {
            searchBar.clear();
            incomeTransactionTableView.model.autoQuery = false;
            selectedDate = new Date(incomeTransactionPage.selectedDate.getFullYear(), incomeTransactionPage.selectedDate.getMonth(), incomeTransactionPage.selectedDate.getDate() + 1);
            singleDateDialog.selectedDate = selectedDate;
            incomeTransactionTableView.model.autoQuery = true;
        }
    }

    function goToPreviousDay() {
        searchBar.clear();
        incomeTransactionTableView.model.autoQuery = false;
        selectedDate = new Date(incomeTransactionPage.selectedDate.getFullYear(), incomeTransactionPage.selectedDate.getMonth(), incomeTransactionPage.selectedDate.getDate() - 1);
        singleDateDialog.selectedDate = selectedDate;
        incomeTransactionTableView.model.autoQuery = true;
    }

    title: qsTr("Income transactions made on %1").arg(Qt.formatDate(selectedDate, "dddd, MMMM d, yyyy"))

    QtObject {
        id: privateProperties

        property int filterIndex: 0
        property int sortIndex: 0
        property var filterModel: ["Search by product name", "Search by category name"]
        property var sortModel: ["Sort in ascending order", "Sort in descending order"]
    }

    actions: FluidControls.Action {
        icon.source: FluidControls.Utils.iconUrl("action/today")
        text: qsTr("Select date")
        onTriggered: singleDateDialog.open()
        toolTip: text
    }

    contentItem: FocusScope {
        focus: true

        RRUi.Card {
            topPadding: 4
            bottomPadding: 0
            leftPadding: 4
            rightPadding: 4
            width: 800

            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: parent.bottom
            }

            Column {
                id: pageFooter

                spacing: 0

                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }

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
                        onClicked: incomeTransactionPage.goToPreviousDay()
                    }

                    QQC2.Button {
                        QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                        enabled: incomeTransactionPage.selectedDate < incomeTransactionPage.todaysDate
                        text: qsTr("Next day")
                        onClicked: incomeTransactionPage.goToNextDay()
                    }

                }

            }

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
                    model: [privateProperties.filterModel[privateProperties.filterIndex], privateProperties.sortModel[privateProperties.sortIndex]]

                    anchors {
                        top: searchBar.bottom
                        left: parent.left
                        right: parent.right
                    }

                }

                IncomeTransactionTableView {
                    id: incomeTransactionTableView

                    filterText: searchBar.text
                    filterColumn: RRModels.IncomeTransactionModel.ClientNameColumn
                    from: new Date(incomeTransactionPage.selectedDate.getFullYear(), incomeTransactionPage.selectedDate.getMonth(), incomeTransactionPage.selectedDate.getDate())
                    to: new Date(incomeTransactionPage.selectedDate.getFullYear(), incomeTransactionPage.selectedDate.getMonth(), incomeTransactionPage.selectedDate.getDate() + 1)
                    onSuccess: {
                        switch (result.code) {
                        case RRModels.IncomeTransactionModel.ViewIncomeTransactionsSuccess:
                            incomeTransactionPage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Transaction deleted."), qsTr("Undo"));
                            break;
                        case RRModels.IncomeTransactionModel.UndoRemoveTransactionSuccess:
                            incomeTransactionPage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Undo successful"));
                            break;
                        }
                    }

                    anchors {
                        top: filterChipListView.bottom
                        left: parent.left
                        right: parent.right
                        bottom: pageFooter.top
                        topMargin: 20
                    }

                    buttonRow: Row {
                        RRUi.ToolButton {
                            //onClicked: incomeTransactionItemDialog.show(parent.parent.modelData);

                            id: viewButton

                            width: FluidControls.Units.iconSizes.medium
                            height: width
                            icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                            text: qsTr("View transaction details")
                        }

                        RRUi.ToolButton {
                            id: deleteButton

                            width: FluidControls.Units.iconSizes.medium
                            height: width
                            icon.source: FluidControls.Utils.iconUrl("action/delete")
                            text: qsTr("Delete transaction")
                            onClicked: deleteConfirmationDialog.show(parent.parent.modelData)
                        }

                    }

                }

            }

        }

        RRUi.AlertDialog {
            id: deleteConfirmationDialog

            property var modelData: null

            function show(modelData) {
                if (Object(modelData).hasOwnProperty("transaction_id")) {
                    text = qsTr("Are you sure you want to remove transaction <b>%1</b> from the list?").arg(modelData.transaction_id);
                    deleteConfirmationDialog.modelData = modelData;
                    open();
                }
            }

            width: 300
            text: qsTr("Are you sure you want to remove this transaction?")
            standardButtons: RRUi.AlertDialog.Yes | RRUi.AlertDialog.No
            onAccepted: {
                incomeTransactionTableView.removeTransaction(modelData.transaction_id);
                deleteConfirmationDialog.modelData = null;
            }
        }

        FluidControls.DatePickerDialog {
            id: singleDateDialog

            title: qsTr("Select date")
            selectedDate: incomeTransactionPage.selectedDate
            standardButtons: QQC2.Dialog.Ok | QQC2.Dialog.Cancel
            onAccepted: {
                incomeTransactionTableView.model.autoQuery = false;
                incomeTransactionPage.selectedDate = selectedDate;
                incomeTransactionTableView.model.autoQuery = true;
            }
        }

    }

}
