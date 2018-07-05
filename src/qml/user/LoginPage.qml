import QtQuick 2.9
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr 1.0 as RR
import "../rrui" as RRUi

RRUi.Page {
    id: loginPage

    signal loggedIn

    contentItem: FocusScope {
        focus: true

        FluidControls.Card {
            focus: true
            anchors.centerIn: parent
            width: 400
            height: 300
            Material.elevation: 2

            contentItem: FocusScope {
                focus: true

                FluidControls.TitleLabel {
                    id: titleLabel
                    height: parent.height * .2
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    verticalAlignment: Qt.AlignVCenter
                    leftPadding: 10
                    text: qsTr("Welcome to Record Rack")
                    color: "white"
                    background: Rectangle { color: Material.color(Material.LightGreen) }
                    font.weight: Font.Light
                }

                Column {
                    id: textFieldColumn
                    anchors {
                        top: titleLabel.bottom
                        topMargin: 36
                        horizontalCenter: parent.horizontalCenter
                    }

                    Row {
                        spacing: 12

                        FluidControls.Icon {
                            anchors.verticalCenter: parent.verticalCenter
                            name: "social/person"//"communication/email"
                            size: 20
                        }

                        RRUi.TextField {
                            id: userNameField
                            focus: true
                            width: 200
                            placeholderText: qsTr("User name") //qsTr("Email")
                        }
                    }

                    Row {
                        spacing: 12

                        FluidControls.Icon {
                            anchors.verticalCenter: parent.verticalCenter
                            name: "action/lock"
                            size: 20
                        }

                        RRUi.TextField {
                            id: passwordField
                            width: 200
                            echoMode: TextInput.Password
                            placeholderText: qsTr("Password")
                            passwordCharacter: "\u26ab"
                        }
                    }
                }

                QQC2.Button {
                    Material.background: Material.accent
                    Material.foreground: "white"
                    anchors {
                        top: textFieldColumn.bottom
                        margins: 24
                        right: parent.right
                    }
                    text: qsTr("Login")
                    onClicked: userProfile.signIn(userNameField.text, passwordField.text);
                }
            }
        }
    }

    RRUi.FailureAlertDialogLoader {
        id: failureAlertDialogLoader
        title: qsTr("Failed to sign in")
    }

    RRUi.BusyOverlay { visible: userProfile.busy }

    RR.UserProfile {
        id: userProfile
        onSuccess: loginPage.loggedIn();
        onError: {
            switch (errorCode) {
            case RR.UserProfile.NoUserNameProvided:
                failureAlertDialogLoader.message = qsTr("The user name field cannot be empty.");
                break;
            case RR.UserProfile.IncorrectCredentials:
                failureAlertDialogLoader.message = qsTr("Your user name or password is incorrect.");
                break;
            default:
                failureAlertDialogLoader.message = qsTr("The cause of the error could not be determined.");
                break;
            }

            failureAlertDialogLoader.create();
        }
    }
}
