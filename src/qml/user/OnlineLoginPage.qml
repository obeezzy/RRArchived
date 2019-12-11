import QtQuick 2.13
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr 1.0 as RR
import "../rrui" as RRUi
import "../lottie" as Lottie
import "../singletons"

RRUi.Page {
    id: onlineLoginPage
    objectName: "onlineLoginPage"

    readonly property bool detailValid: emailAddressField.textField.text !== "" && passwordField.textField.text !== ""
    property RR.UserProfile userProfile: null
    signal loggedIn

    padding: 16
    topPadding: 36

    contentItem: FocusScope {
        focus: true

        FluidControls.SubheadingLabel {
            anchors {
                left: parent.left
                right: parent.right
            }

            text: qsTr("Welcome to Record Rack! To get started, we need to sync up your online account and your local account. Enter your username and password below. If you do not have an account, click the 'Sign up' link below. if you forgot your password, click the 'Forgot password' link below.")
            wrapMode: Text.WordWrap
        }

        Column {
            visible: !lottieCheckmark.visible && !busyIndicator.visible
            anchors.centerIn: parent

            RRUi.IconTextField {
                id: emailAddressField
                icon {
                    source: FluidControls.Utils.iconUrl("communication/email")
                    size: 20
                }

                textField {
                    focus: true
                    width: 320
                    placeholderText: qsTr("Email address")
                }
            }

            RRUi.IconTextField {
                id: passwordField
                icon {
                    source: FluidControls.Utils.iconUrl("action/lock")
                    size: 20
                }

                textField {
                    width: 320
                    echoMode: TextInput.Password
                    placeholderText: qsTr("Password")
                    passwordCharacter: "\u26ab"
                }
            }

            RRUi.PrimaryButton {
                anchors.right: parent.right
                text: qsTr("Login")
                onClicked: onlineLoginPage.userProfile.linkAccount(emailAddressField.textField.text,
                                                                   passwordField.textField.text);
            }

            Item { width: 1; height: 24 }

            FluidControls.SubheadingLabel {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("<a href='%1'>New to Record Rack?</a>").arg(onlineLoginPage.userProfile.newUserUrl)
                onLinkActivated: Qt.openUrlExternally(link);
            }

            Item { width: 1; height: 4 }

            FluidControls.SubheadingLabel {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("<a href='%1'>Forgot password?</a>").arg(onlineLoginPage.userProfile.passwordRetrievalUrl)
                onLinkActivated: Qt.openUrlExternally(link);
            }
        }

        Lottie.Checkmark {
            id: lottieCheckmark
            anchors.fill: parent
            text: qsTr("Account linked!")
            onFinished: onlineLoginPage.loggedIn();
        }

        RRUi.BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            visible: onlineLoginPage.userProfile.busy
            text: qsTr("Connecting to online server...")
        }
    }

    RRUi.ErrorDialog { id: errorDialog }

    Connections {
        target: onlineLoginPage.userProfile
        onSuccess: {
            switch (successCode) {
            case RR.UserProfile.LinkAccountSuccess:
                lottieCheckmark.animate();
                break;
            }
        }

        onError: {
            switch (errorCode) {
            case RR.UserProfile.ConnectionRefusedError:
                errorDialog.show(qsTr("Unable to connect to the server. Check your internet connection and try again."), qsTr("Error"));
                break;
            case RR.UserProfile.NoEmailAddressProvided:
                errorDialog.show(qsTr("Please enter your email address."), qsTr("Error"));
                break;
            case RR.UserProfile.NoPasswordProvided:
                errorDialog.show(qsTr("Please enter your password."), qsTr("Error"));
                break;
            case RR.UserProfile.IncorrectCredentials:
                errorDialog.show(qsTr("The email address or password provided is incorrect."), qsTr("Authentication error"));
                break;
            default:
                errorDialog.show(qsTr("An unknown error occurred."), qsTr("Error"));
                break;
            }
        }
    }
}
