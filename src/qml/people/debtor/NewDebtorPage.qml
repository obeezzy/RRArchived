import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import com.gecko.rr.models 1.0 as RRModels
import com.gecko.rr.components 1.0 as RRComponents
import "../../rrui" as RRUi
import "../../common"

RRUi.Page {
    id: newDebtorPage

    property RRModels.DebtorModel model: null
    property int debtorId: -1

    title: qsTr("Add debtor")
    topPadding: 10
    bottomPadding: 10
    leftPadding: 20
    rightPadding: 20

    contentItem: FocusScope {
        RRUi.TransitionView {
            id: transitionView
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: parent.bottom
            }
            width: 800

            transitionComponent: FluidControls.Card {
                id: transitionItem
                padding: 0
                bottomPadding: 0

                QQC2.TabBar {
                    id: tabBar
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    currentIndex: swipeView.currentIndex

                    QQC2.TabButton { text: qsTr("Client details") }
                    QQC2.TabButton { text: qsTr("Transactions") }
                }

                QQC2.SwipeView {
                    id: swipeView
                    clip: true
                    currentIndex: tabBar.currentIndex
                    interactive: false

                    anchors {
                        top: tabBar.bottom
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }

                    ClientDetailSubView { debtorId: newDebtorPage.debtorId }
                    DebtTransactionSubView { debtorId: newDebtorPage.debtorId }
                }

                Column {
                    id: pageFooter
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                        leftMargin: 4
                        rightMargin: 4
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
                            visible: tabBar.currentIndex == 1
                            QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                            text: qsTr("Add transaction")
                        }

                        QQC2.Button {
                            QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                            text: newDebtorPage.debtorId > 0 ? qsTr("Update debtor") : qsTr("Add debtor")
                            onClicked: transitionView.trigger();
                        }
                    }
                }
            }
        }
    }
}
