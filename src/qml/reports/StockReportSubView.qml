import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../common"

RRUi.SubView {
    id: stockReportSubView
    objectName: "reports/StockReportSubView"

    QtObject {
        id: privateProperties

        property int filterIndex: 0
        property int sortIndex: 0
        property var filterModel: ["Search by item name", "Search by category name"]
        property var sortModel: ["Sort in ascending order", "Sort in descending order"]
    }

    QQC2.StackView.onActivating: stockReportTableView.refresh();

    contentItem: FocusScope {
        RRUi.Card {
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: parent.bottom
            }

            Material.elevation: 0
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

                StockReportTableView {
                    id: stockReportTableView
                    anchors {
                        top: filterChipListView.bottom
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }

                    autoQuery: stockReportSubView.QQC2.SwipeView.index === 0
                    buttonRow: Row {
                        spacing: 0

                        RRUi.ToolButton {
                            id: editButton
                            icon.source: FluidControls.Utils.iconUrl("image/remove_red_eye")
                            text: qsTr("View")
                            onClicked: homePage.push(Qt.resolvedUrl("NewDebtorPage.qml"),
                                                     { "debtorId": parent.parent.modelData.debtor_id });
                        }
                    }
                }
            }
        }
    }

    QQC2.BusyIndicator {
        anchors.centerIn: parent
        visible: stockReportTableView.busy
    }

    /********************** ON-DEMAND ITEMS *****************************/
    FluidControls.Placeholder {
        visible: stockReportTableView.rows == 0 && searchBar.text !== ""
        anchors.centerIn: parent
        icon.source: FluidControls.Utils.iconUrl("action/search")
        text: qsTr("No results for this search query.")
    }

    FluidControls.Placeholder {
        visible: stockReportTableView.rows == 0 && searchBar.text === ""
        anchors.centerIn: parent
        icon.source: Qt.resolvedUrl("qrc:/icons/truck.svg")
        text: qsTr("No transactions were made on this day.")
    }

    Connections {
        target: stockReportSubView.QQC2.SwipeView.view
        onCurrentIndexChanged: {
            if (stockReportSubView.QQC2.SwipeView.view.currentIndex === stockReportSubView.QQC2.SwipeView.index)
                stockReportTableView.refresh();
        }
    }
}
