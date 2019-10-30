import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../common"

RRUi.SubView {
    id: purchaseReportSubView
    objectName: "reports/purchaseReportSubView"

    RRUi.ViewPreferences {
        id: viewPreferences

        filterModel: [
            "Filter by item",
            "Filter by category"
        ]

        sortColumnModel: [
            "Sort by item",
            "Sort by category"
        ]
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
                    anchors {
                        top: searchBar.bottom
                        left: parent.left
                        right: parent.right
                    }

                    model: viewPreferences.model
                }

                PurchaseReportTableView {
                    id: purchaseReportTableView
                    anchors {
                        top: filterChipListView.bottom
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }

                    autoQuery: purchaseReportSubView.QQC2.SwipeView.index === 0
                }
            }
        }
    }

    QQC2.BusyIndicator {
        anchors.centerIn: parent
        visible: purchaseReportTableView.busy
    }

    /********************** ON-DEMAND ITEMS *****************************/
    FluidControls.Placeholder {
        visible: purchaseReportTableView.rows === 0 && searchBar.text !== ""
        anchors.centerIn: parent
        icon.source: FluidControls.Utils.iconUrl("action/search")
        text: qsTr("No results for this search query.")
    }

    FluidControls.Placeholder {
        visible: purchaseReportTableView.rows === 0 && searchBar.text === ""
        anchors.centerIn: parent
        icon.source: Qt.resolvedUrl("qrc:/icons/cart.svg")
        text: qsTr("No transactions were made on this day.")
    }

    Connections {
        target: purchaseReportSubView.QQC2.SwipeView.view
        onCurrentIndexChanged: {
            if (purchaseReportSubView.QQC2.SwipeView.view.currentIndex === purchaseReportSubView.QQC2.SwipeView.index)
                purchaseReportTableView.refresh();
        }
    }
}
