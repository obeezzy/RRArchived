import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr 1.0 as RR
import "../rrui" as RRUi

RRUi.Page {
    id: loginPage

    signal loggedIn

    contentItem: FocusScope {
        focus: true

        RRUi.Card {
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
                            source: FluidControls.Utils.iconUrl("social/person")
                            size: 20
                        }

                        RRUi.TextField {
                            id: userNameField
                            focus: true
                            width: 200
                            placeholderText: qsTr("User name")
                        }
                    }

                    Row {
                        spacing: 12

                        FluidControls.Icon {
                            anchors.verticalCenter: parent.verticalCenter
                            source: FluidControls.Utils.iconUrl("action/lock")
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

                RRUi.PrimaryButton {
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

    RRUi.ErrorDialog { id: errorDialog }

    RRUi.BusyOverlay { visible: userProfile.busy }

    RR.UserProfile {
        id: userProfile
        onSuccess: loginPage.loggedIn();
        onError: {
            switch (errorCode) {
            case RR.UserProfile.NoUserNameProvided:
                errorDialog.show(qsTr("The user name field cannot be empty."), qsTr("Failed to sign in"), errorCode);
                break;
            case RR.UserProfile.IncorrectCredentials:
                errorDialog.show(qsTr("Your user name or password is incorrect."), qsTr("Failed to sign in"), errorCode);
                break;
            case RR.UserProfile.UserAccountIsLockedError:
                errorDialog.show(qsTr("This account has been locked. Please contact the administrator."), qsTr("Failed to sign in"), errorCode);
                break;
            default:
                errorDialog.show(qsTr("The cause of the error could not be determined."), qsTr("Failed to sign in"), errorCode);
                break;
            }
        }
    }
}
