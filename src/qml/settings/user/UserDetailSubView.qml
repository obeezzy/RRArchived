import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../../rrui" as RRUi

RRUi.SubView {
    id: userDetailSubView

    readonly property bool isExistingUser: userId > 0
    property int userId: -1

    property string imageUrl: ""
    property string firstName: ""
    property string lastName: ""
    property string userName: ""
    property string password: ""
    property string passwordConfirmation: ""
    property string phoneNumber: ""
    property string emailAddress: ""

    implicitWidth: 800
    implicitHeight: 800

    RRModels.UserDetailRecord { id: userDetailRecord; userId: userDetailSubView.userId }

    contentItem: FocusScope {
        QQC2.ScrollView {
            id: scrollView
            anchors {
                top: parent.top
                bottom: parent.bottom
                topMargin: 24
            }

            Row {
                id: mainRow
                anchors {
                    left: parent.left
                    leftMargin: 64
                }

                spacing: 84

                Column {
                    id: imageColumn
                    width: 160
                    spacing: 4

                    RRUi.LetterCircleImage {
                        id: itemImage
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        height: width
                        font.pixelSize: 30
                        name: userNameTextField.text

                        FluidControls.Icon {
                            anchors.centerIn: parent
                            visible: itemImage.name === ""
                            color: "white"
                            source: FluidControls.Utils.iconUrl("image/photo_camera")
                        }
                    }

                    Row {
                        anchors.horizontalCenter: parent.horizontalCenter

                        RRUi.ToolButton {
                            id: takePhotoButton
                            icon.source: FluidControls.Utils.iconUrl("image/photo_camera")
                            text: qsTr("Take a photo")
                        }

                        RRUi.ToolButton {
                            id: selectPhotoButton
                            icon.source: FluidControls.Utils.iconUrl("image/photo")
                            text: qsTr("Select image")
                        }

                        RRUi.ToolButton {
                            id: deviceSearchButton
                            icon.source: FluidControls.Utils.iconUrl("hardware/phonelink")
                            text: qsTr("Start DeviceLink")
                        }
                    }
                }

                Column {
                    id: detailColumn
                    width: 300

                    RRUi.IconTextField {
                        id: userNameTextField
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        enabled: !userDetailSubView.isExistingUser
                        icon.source: FluidControls.Utils.iconUrl("social/person")
                        textField.placeholderText: qsTr("User name")
                        text: userDetailRecord.userName

                        Connections {
                            target: userNameTextField.textField
                            onTextEdited: userDetailSubView.userName = userNameTextField.textField.text;
                        }
                    }

                    RRUi.IconTextField {
                        id: firstNameTextField
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        enabled: !userDetailSubView.isExistingUser
                        textField.placeholderText: qsTr("First name")
                        text: userDetailRecord.firstName

                        Connections {
                            target: firstNameTextField.textField
                            onTextEdited: userDetailSubView.firstName = firstNameTextField.textField.text;
                        }
                    }

                    RRUi.IconTextField {
                        id: lastNameTextField
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        enabled: !userDetailSubView.isExistingUser
                        textField.placeholderText: qsTr("Last name")
                        text: userDetailRecord.lastName

                        Connections {
                            target: lastNameTextField.textField
                            onTextEdited: userDetailSubView.lastName = lastNameTextField.textField.text;
                        }
                    }

                    RRUi.IconTextField {
                        id: passwordTextField
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        visible: !userDetailSubView.isExistingUser
                        icon.source: FluidControls.Utils.iconUrl("action/lock")
                        textField.placeholderText: qsTr("Password")
                        textField.echoMode: TextInput.Password
                        textField.passwordCharacter: "\u26ab"

                        Connections {
                            target: passwordTextField.textField
                            onTextEdited: userDetailSubView.password = passwordTextField.textField.text;
                        }
                    }

                    RRUi.IconTextField {
                        id: confirmPasswordTextField
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        visible: !userDetailSubView.isExistingUser

                        textField.placeholderText: qsTr("Confirm password")
                        textField.echoMode: TextInput.Password
                        textField.passwordCharacter: "\u26ab"

                        Connections {
                            target: confirmPasswordTextField.textField
                            onTextEdited: userDetailSubView.passwordConfirmation = confirmPasswordTextField.textField.text;
                        }
                    }

                    RRUi.IconTextField {
                        id: phoneNumberTextField
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        enabled: !userDetailSubView.isExistingUser

                        icon.source: FluidControls.Utils.iconUrl("communication/phone")
                        textField.placeholderText: qsTr("Phone number")
                        text: userDetailRecord.phoneNumber

                        Connections {
                            target: phoneNumberTextField.textField
                            onTextEdited: userDetailSubView.phoneNumber = phoneNumberTextField.textField.text;
                        }
                    }

                    RRUi.IconTextField {
                        id: emailTextField
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        enabled: !userDetailSubView.isExistingUser

                        icon.source: FluidControls.Utils.iconUrl("communication/email")
                        textField.placeholderText: qsTr("Email address")
                        text: userDetailRecord.emailAddress

                        Connections {
                            target: emailTextField.textField
                            onTextEdited: userDetailSubView.emailAddress = emailTextField.textField.text;
                        }
                    }
                }
            }
        }
    }
}
