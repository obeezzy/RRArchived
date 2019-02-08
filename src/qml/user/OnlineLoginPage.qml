import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr 1.0 as RR
import "../rrui" as RRUi

RRUi.Page {
    id: onlineLoginPage
    objectName: "onlineLoginPage"

    readonly property bool detailValid: emailAddressField.textField.text !== "" && passwordField.textField.text !== ""
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
            anchors.centerIn: parent

            RRUi.IconTextField {
                id: emailAddressField
                focus: true
                icon.source: FluidControls.Utils.iconUrl("communication/email")

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

            QQC2.Button {
                Material.background: Material.accent
                Material.foreground: "white"
                anchors.right: parent.right
                text: qsTr("Login")
                onClicked: userProfile.signInOnline(emailAddressField.textField.text, passwordField.textField.text);
            }

            Item { width: 1; height: 24 }

            FluidControls.SubheadingLabel {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("<a href='https://www.github.com/obeezzy/RecordRackDesktop/'>New to Record Rack?</a>")
                onLinkActivated: Qt.openUrlExternally(link);
            }

            Item { width: 1; height: 4 }

            FluidControls.SubheadingLabel {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("<a href='https://www.github.com/obeezzy/Seppuku/'>Forgot password?</a>")
                onLinkActivated: Qt.openUrlExternally(link);
            }
        }
    }

    RRUi.BusyOverlay { visible: userProfile.busy }

    RRUi.ErrorDialog { }

    RR.UserProfile { id: userProfile }
}
