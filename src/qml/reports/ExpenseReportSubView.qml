import "../common"
import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

RRUi.SubView {
    id: expenseReportSubView

    objectName: "reports/expenseReportSubView"

    RRUi.ViewPreferences {
        id: viewPreferences

        filterModel: ["Filter by purpose", "Filter by amount"]
        sortColumnModel: ["Sort by purpose", "Sort by amount"]
    }

    QQC2.BusyIndicator {
        anchors.centerIn: parent
        visible: expenseReportTableView.busy
    }

    //********************* ON-DEMAND ITEMS ****************************
    FluidControls.Placeholder {
        visible: expenseReportTableView.rows === 0 && searchBar.text !== ""
        anchors.centerIn: parent
        icon.source: FluidControls.Utils.iconUrl("action/search")
        text: qsTr("No results for this search query.")
    }

    FluidControls.Placeholder {
        visible: expenseReportTableView.rows === 0 && searchBar.text === ""
        anchors.centerIn: parent
        icon.source: Qt.resolvedUrl("qrc:/icons/wallet.svg")
        text: qsTr("No transactions were made on this day.")
    }

    Connections {
        target: expenseReportSubView.QQC2.SwipeView.view
        onCurrentIndexChanged: {
            if (expenseReportSubView.QQC2.SwipeView.view.currentIndex === expenseReportSubView.QQC2.SwipeView.index)
                expenseReportTableView.refresh();

        }
    }

    contentItem: FocusScope {
        RRUi.Card {
            anchors.fill: parent
            Material.elevation: 0
            topPadding: 4
            bottomPadding: 0
            leftPadding: 4
            rightPadding: 4

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
                    model: viewPreferences.model

                    anchors {
                        top: searchBar.bottom
                        left: parent.left
                        right: parent.right
                    }

                }

                ExpenseReportTableView {
                    id: expenseReportTableView

                    autoQuery: expenseReportSubView.QQC2.SwipeView.index === 0

                    anchors {
                        top: filterChipListView.bottom
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }

                }

            }

        }

    }

}
