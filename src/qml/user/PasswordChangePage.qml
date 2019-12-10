import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr 1.0 as RR
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi

RRUi.Page {
    id: passwordChangePage
    objectName: "passwordChangePage"

    property bool isFirstTime: true
    signal accepted

    function validateInput() {
        if (newPasswordTextField.text !== passwordConfirmationTextField.text) {
            errorDialog.show(qsTr("Passwords do not match."), qsTr("Error"));
            return false;
        } else if (oldPasswordTextField.text == "") {
            errorDialog.show(qsTr("Old password not set."), qsTr("Error"));
            return false;
        } else if (newPasswordTextField.text == "") {
            errorDialog.show(qsTr("New password not set."), qsTr("Error"));
            return false;
        } else if (passwordConfirmationTextField.text == "") {
            errorDialog.show(qsTr("Confirm password field not set."), qsTr("Error"));
            return false;
        } else if (oldPasswordTextField.text == newPasswordTextField.text) {
            errorDialog.show(qsTr("Old password and new password are the same."), qsTr("Error"));
            return false;
        }

        return true;
    }

    title: qsTr("Change password")

    contentItem: FocusScope {
        focus: true

        RRUi.Card {
            focus: true
            anchors.centerIn: parent
            width: 400
            height: 400
            Material.elevation: 2

            contentItem: FocusScope {
                focus: true

                Column {
                    id: textFieldColumn
                    anchors {
                        top: parent.top
                        margins: 16
                        topMargin: 36
                        left: parent.left
                        right: parent.right
                    }

                    FluidControls.SubheadingLabel {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        text: passwordChangePage.isFirstTime
                              ? qsTr("To ensure the highest security, Record Rack advises that you change your administrator password immediately.")
                              : qsTr("To ensure the highest security, Record Rack advises that you choose a strong password (one that has at least a capital letter and a number).")
                        wrapMode: Text.WordWrap
                    }

                    Item { width: 1; height: 24 }

                    RRUi.IconTextField {
                        id: oldPasswordTextField
                        anchors.horizontalCenter: parent.horizontalCenter
                        focus: true
                        width: 200
                        textField.placeholderText: qsTr("Old password")
                        textField.echoMode: TextInput.Password
                        textField.passwordCharacter: "\u26ab"
                    }

                    RRUi.IconTextField {
                        id: newPasswordTextField
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: 200
                        textField.placeholderText: qsTr("New password")
                        textField.echoMode: TextInput.Password
                        textField.passwordCharacter: "\u26ab"
                    }

                    RRUi.IconTextField {
                        id: passwordConfirmationTextField
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: 200
                        textField.placeholderText: qsTr("Confirm password")
                        textField.echoMode: TextInput.Password
                        textField.passwordCharacter: "\u26ab"
                    }
                }

                RRUi.PrimaryButton {
                    anchors {
                        bottom: parent.bottom
                        margins: 24
                        right: parent.right
                    }
                    text: qsTr("Submit")
                    onClicked: {
                        if (passwordChangePage.validateInput())
                            userProfile.changePassword(oldPasswordTextField.text, newPasswordTextField.text);
                    }
                }
            }
        }
    }

    RRUi.ErrorDialog { id: errorDialog }

    RRUi.BusyOverlay { visible: userProfile.busy }

    RR.UserProfile {
        id: userProfile
        onSuccess: passwordChangePage.accepted();
        onError: {
            switch (errorCode) {
            case RR.UserProfile.IncorrectCredentials:
                errorDialog.show(qsTr("The user's user name must be provided."), qsTr("Error"), errorCode);
                break;
            case RR.UserProfile.OldPasswordWrongError:
                errorDialog.show(qsTr("The old password provided is not correct."), qsTr("Error"), errorCode);
                break;
            default:
                errorDialog.show();
            }
        }
    }
}
