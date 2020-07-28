import "../common"
import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

RRUi.Page {
    id: homePage

    objectName: "reports/HomePage"
    title: qsTr("Reports")
    topPadding: 10
    bottomPadding: 10
    leftPadding: 20
    rightPadding: 20
    actions: [
        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("navigation/more_vert")
            text: qsTr("Add a new debtor.")
            onTriggered: bottomSheet.open()
            toolTip: qsTr("More options")
        }
    ]

    FluidControls.BottomSheetList {
        id: bottomSheet

        title: qsTr("What would you like to do?")
        actions: [
            FluidControls.Action {
                icon.source: FluidControls.Utils.iconUrl("content/add")
                text: qsTr("Add a debtor.")
                onTriggered: homePage.push(Qt.resolvedUrl("NewDebtorPage.qml"))
            },
            FluidControls.Action {
                icon.source: FluidControls.Utils.iconUrl("image/edit")
                text: qsTr("Manage debtors.")
            }
        ]
    }

    contentItem: FocusScope {
        RRUi.Card {
            width: 960

            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: parent.bottom
            }

            contentItem: FocusScope {
                focus: true

                QQC2.TabBar {
                    id: tabBar

                    clip: true
                    currentIndex: swipeView.currentIndex

                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    QQC2.TabButton {
                        text: qsTr("Totals")
                        width: 120
                    }

                    QQC2.TabButton {
                        text: qsTr("Stock")
                        width: 120
                    }

                    QQC2.TabButton {
                        text: qsTr("Sales")
                        width: 120
                    }

                    QQC2.TabButton {
                        text: qsTr("Purchase")
                        width: 120
                    }

                    QQC2.TabButton {
                        text: qsTr("Income")
                        width: 120
                    }

                    QQC2.TabButton {
                        text: qsTr("Expenses")
                        width: 120
                    }

                    QQC2.TabButton {
                        text: qsTr("Debtor")
                        width: 120
                    }

                    QQC2.TabButton {
                        text: qsTr("Creditor")
                        width: 120
                    }

                }

                QQC2.SwipeView {
                    id: swipeView

                    clip: true
                    currentIndex: tabBar.currentIndex
                    interactive: false
                    topPadding: 4
                    bottomPadding: 0
                    leftPadding: 4
                    rightPadding: 4

                    anchors {
                        top: tabBar.bottom
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }

                    TotalReportSubView {
                        id: totalReportSubView
                    }

                    StockReportSubView {
                        id: stockReportSubView
                    }

                    SaleReportSubView {
                        id: saleReportSubView
                    }

                    PurchaseReportSubView {
                        id: purchaseReportSubView
                    }

                    IncomeReportSubView {
                        id: incomeReportSubView
                    }

                    ExpenseReportSubView {
                        id: expenseReportSubView
                    }

                    DebtorReportSubView {
                        id: debtorReportSubView
                    }

                    CreditorReportSubView {
                        id: creditReportSubView
                    }

                }

            }

        }

    }

}
