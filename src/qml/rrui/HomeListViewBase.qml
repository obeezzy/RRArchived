import QtQuick 2.12

ListView {
    id: listView

    signal viewRequested(var breadcrumbs, var properties)

    width: 640
    height: contentItem.childrenRect.height
    topMargin: 20
    bottomMargin: 20
    spacing: 24

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

            PauseAnimation { duration: 300 }
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
