import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import com.gecko.rr.models 1.0 as RRModels
import com.gecko.rr.components 1.0 as RRComponents
import "../../rrui" as RRUi
import "../../common"

RRUi.SubView {
    id: clientDetailSubView

    property int debtorId: -1
    property url imageUrl: debtorDetailRecord.imageUrl
    property string preferredName: debtorDetailRecord.preferredName
    property string primaryPhoneNumber: debtorDetailRecord.phoneNumber
    property string firstName: debtorDetailRecord.firstName
    property string lastName: debtorDetailRecord.lastName
    property string middleName: ""
    property string title: ""
    property ListModel phoneNumberModel: ListModel { }
    property ListModel addressModel: ListModel { }
    property ListModel emailModel: ListModel { }

    implicitWidth: 800
    implicitHeight: 800

    RRModels.DebtorDetailRecord {
        id: debtorDetailRecord
        debtorId: clientDetailSubView.debtorId
    }

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
                        id: clientImage
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        height: width
                        font.pixelSize: 30
                        name: "Name"
                        source: clientDetailSubView.imageUrl

                        FluidControls.Icon {
                            anchors.centerIn: parent
                            visible: clientImage.name === ""
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
                        id: nameTextField
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        icon.source: FluidControls.Utils.iconUrl("social/person")
                        textField.placeholderText: qsTr("Name")
                        textField.text: debtorDetailRecord.preferredName

                        Connections {
                            target: nameTextField.textField
                            onTextEdited: clientDetailSubView.preferredName = nameTextField.textField.text;
                        }
                    }

                    RRUi.IconTextField {
                        id: phoneNumberTextField
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        icon.source: FluidControls.Utils.iconUrl("communication/phone")
                        textField.placeholderText: qsTr("Primary phone number")
                        textField.validator: RRComponents.DoubleValidator { bottom: 0 }
                        textField.text: debtorDetailRecord.phoneNumber

                        Connections {
                            target: phoneNumberTextField.textField
                            onTextEdited: clientDetailSubView.primaryPhoneNumber = phoneNumberTextField.textField.text;
                        }
                    }

                    ListView {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        height: contentHeight
                        model: debtorDetailRecord.addressModel.length === 0 ? 1 : debtorDetailRecord.addressModel

                        delegate: RRUi.IconTextField {
                            width: ListView.view.width

                            icon.source: FluidControls.Utils.iconUrl("communication/location_on")
                            textField.placeholderText: qsTr("Address")
                            textField.text: debtorDetailRecord.addressModel.length === 0 ? "" : modelData
                        }
                    }

                    ListView {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        height: contentHeight
                        model: debtorDetailRecord.emailModel.length === 0 ? 1 : debtorDetailRecord.emailModel

                        delegate: RRUi.IconTextField {
                            icon.source: FluidControls.Utils.iconUrl("communication/email")
                            textField.placeholderText: qsTr("Email")
                            textField.text: debtorDetailRecord.emailModel.length === 0 ? "" : modelData
                        }
                    }
                }
            }
        }
    }
}
