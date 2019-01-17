import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi

RRUi.Page {
    id: dashboardPage
    objectName: "dashboardPage"

    signal linkActivated(var link, var properties)

    title: qsTr("Dashboard")
    topPadding: 0
    bottomPadding: 0
    leftPadding: 20
    rightPadding: 20

    ListView {
        id: listView
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
        width: 800
        topMargin: 20
        bottomMargin: 20
        spacing: 24

        model: RRModels.DashboardHomeModel { }

        delegate: RRUi.HomeCard {
            width: ListView.view.width
            cardTitle: title
            shortDescription: short_description
            imageUrl: image_url

            onViewRequested: {
                dashboardPage.linkActivated(["stock/HomePage.qml", "stock/NewItemPage.qml"], { });
            }
        }

        populate: Transition {
            id: populateTransition

            SequentialAnimation {
                PropertyAction {
                    property: "y"
                    value: (populateTransition.ViewTransition.index + 1) * (populateTransition.ViewTransition.destination.y + 400)
                }

                PropertyAction {
                    property: "opacity"
                    value: .01
                }

                PauseAnimation { duration: 600 }
                ParallelAnimation {
                    YAnimator {
                        duration: 300
                        from: (populateTransition.ViewTransition.index + 1) * (populateTransition.ViewTransition.destination.y + 400)
                        to: populateTransition.ViewTransition.destination.y
                        easing.type: Easing.OutCubic
                    }

                    NumberAnimation {
                        property: "opacity"
                        duration: 300
                        to: 1
                        easing.type: Easing.OutCubic
                    }
                }
            }
        }
    }

    FluidControls.Placeholder {
        visible: listView.count == 0
        anchors.centerIn: parent
        icon.source: FluidControls.Utils.iconUrl("editor/bubble_chart")
        text: qsTr("No data available.")
    }
}
