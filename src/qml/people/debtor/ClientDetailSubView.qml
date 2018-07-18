import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import com.gecko.rr.models 1.0 as RRModels
import com.gecko.rr.components 1.0 as RRComponents
import "../../rrui" as RRUi
import "../../common"

QQC2.Control {
    id: clientDetailSubView

    property int debtorId: -1

    implicitWidth: 800
    implicitHeight: 800

    Item {
        RRModels.DebtorDetailRecord {
            id: debtorDetailRecord
            debtorId: clientDetailSubView.debtorId
        }
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
                        id: itemImage
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        height: width
                        font.pixelSize: 30
                        name: "Name"

                        FluidControls.Icon {
                            anchors.centerIn: parent
                            visible: itemImage.name === ""
                            color: "white"
                            name: "image/photo_camera"
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

                        icon.name: "social/person"
                        textField.placeholderText: qsTr("Name")
                        textField.text: debtorDetailRecord.preferredName
                    }

                    RRUi.IconTextField {
                        id: phoneNumberTextField
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        icon.name: "communication/phone"
                        textField.placeholderText: qsTr("Phone number")
                        textField.validator: RRComponents.DoubleValidator { bottom: 0 }
                        textField.text: debtorDetailRecord.phoneNumber
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

                            icon.name: "communication/location_on"
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
                            icon.name: "communication/email"
                            textField.placeholderText: qsTr("Email")
                            textField.text: debtorDetailRecord.emailModel.length === 0 ? "" : modelData
                        }
                    }
                }
            }
        }
    }
}
