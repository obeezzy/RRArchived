import "../rrui" as RRUi
import "../singletons"
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import com.gecko.rr 1.0 as RR

RRUi.Page {
    id: onboardingPage

    signal finished()

    QtObject {
        id: privateProperties

        property int businessStoreIndex: -1
    }

    RR.DatabaseCreator {
        id: databaseCreator

        onSuccess: onboardingPage.finished()
    }

    RRUi.BusyOverlay {
        id: busyOverlay

        visible: databaseCreator.busy || (userProfile.busy && stackView.currentObjectName === "businessDetailPage")
    }

    RR.UserProfile {
        id: userProfile

        onSuccess: {
            switch (result.code) {
            case RR.UserProfile.LinkBusinessStoreSuccess:
                busyOverlay.text = qsTr("Building database...");
                databaseCreator.start();
                break;
            }
        }
    }

    contentItem: FocusScope {
        focus: true

        RRUi.Card {
            anchors.centerIn: parent
            width: 1024
            height: 960
            focus: true

            RRUi.HorizontalStepper {
                id: horizontalStepper

                model: [qsTr("Link to online account"), qsTr("Choose business"), qsTr("Confirm business details")]
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

                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }

            }

            RRUi.StackView {
                id: stackView

                focus: true
                clip: true
                Component.onCompleted: push(Qt.resolvedUrl("OnlineLoginPage.qml"), {
                    "userProfile": userProfile
                })

                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: footer.top
                    top: horizontalStepper.bottom
                }

                Connections {
                    enabled: stackView.currentObjectName === "onlineLoginPage"
                    ignoreUnknownSignals: true
                    target: stackView.currentItem
                    onLoggedIn: {
                        nextButton.enabled = true;
                        nextButton.focus = true;
                    }
                }

            }

            Item {
                id: footer

                height: 64

                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }

                FluidControls.ThinDivider {
                    anchors.top: parent.top
                }

                QQC2.Button {
                    Material.foreground: Material.Blue
                    visible: stackView.depth > 1
                    flat: true
                    text: qsTr("Back")
                    onClicked: stackView.pop()

                    anchors {
                        left: parent.left
                        leftMargin: 8
                        verticalCenter: parent.verticalCenter
                    }

                }

                RRUi.PrimaryButton {
                    id: nextButton

                    Material.background: Material.Blue
                    enabled: false
                    text: stackView.currentObjectName === "businessDetailPage" ? qsTr("Finish") : qsTr("Next")
                    onClicked: {
                        if (stackView.currentObjectName === "onlineLoginPage") {
                            stackView.push(Qt.resolvedUrl("BusinessSelectionPage.qml"), {
                                "userProfile": userProfile
                            });
                        } else if (stackView.currentObjectName === "businessSelectionPage") {
                            userProfile.businessStoreModel.currentIndex = stackView.currentItem.businessStoreIndex;
                            stackView.push(Qt.resolvedUrl("BusinessDetailPage.qml"), {
                                "userProfile": userProfile
                            });
                        } else if (stackView.currentObjectName === "businessDetailPage") {
                            busyOverlay.text = qsTr("Linking business store...");
                            userProfile.linkBusinessStore();
                        }
                    }

                    anchors {
                        right: parent.right
                        rightMargin: 8
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

    }

}
