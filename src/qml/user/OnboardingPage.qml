import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr 1.0 as RR
import "../rrui" as RRUi

RRUi.Page {
    id: onboardingPage

    signal finished

    contentItem: FocusScope {
        FluidControls.Card {
            anchors.centerIn: parent
            width: 1024
            height: 960

            RRUi.HorizontalStepper {
                id: horizontalStepper
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }

                model: [ qsTr("Link to online account"), qsTr("Choose business"), qsTr("Confirm business details") ]
                currentIndex: {
                    switch (stackView.currentObjectName) {
                    case "onlineLoginPage":
                        0;
                        break;
                    case "businessSelectionPage":
                        1;
                        break;
                    case "businessDetailPage":
                        2;
                        break;
                    default:
                        0;
                    }
                }
            }

            RRUi.StackView {
                id: stackView
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: footer.top
                    top: horizontalStepper.bottom
                }

                clip: true
                initialItem: Qt.resolvedUrl("OnlineLoginPage.qml")
            }

            Item {
                id: footer
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                height: 64

                FluidControls.ThinDivider { anchors.top: parent.top }

                QQC2.Button {
                    Material.foreground: Material.Blue

                    visible: stackView.depth > 1
                    flat: true
                    anchors {
                        left: parent.left
                        leftMargin: 8
                        verticalCenter: parent.verticalCenter
                    }

                    text: qsTr("Back")
                    onClicked: stackView.pop();
                }

                QQC2.Button {
                    Material.background: Material.Blue
                    Material.foreground: "white"

                    anchors {
                        right: parent.right
                        rightMargin: 8
                        verticalCenter: parent.verticalCenter
                    }


                    text: stackView.currentObjectName === "businessDetailPage" ? qsTr("Finish") : qsTr("Next")
                    onClicked: {
                        if (stackView.currentObjectName === "onlineLoginPage") {
                            stackView.push(Qt.resolvedUrl("BusinessSelectionPage.qml"));
                        } else if (stackView.currentObjectName === "businessSelectionPage") {
                            stackView.push(Qt.resolvedUrl("BusinessDetailPage.qml"));
                        } else if (stackView.currentObjectName === "businessDetailPage") {
                            databaseCreator.start();
                        }
                    }
                }
            }
        }
    }

    RR.DatabaseCreator {
        id: databaseCreator
        onSuccess: onboardingPage.finished();
    }

    RRUi.BusyOverlay { visible: databaseCreator.busy }
}
