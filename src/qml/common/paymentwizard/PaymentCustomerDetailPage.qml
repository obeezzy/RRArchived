import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import Fluid.Controls 1.0 as FluidControls
import "../../rrui" as RRUi

QQC2.Page {
    id: paymentCustomerDetailPage
    objectName: "paymentCustomerDetailPage"

    property string customerName: ""
    property string customerPhoneNumber: ""
    readonly property bool detailValid: customerName.trim() != "" && customerPhoneNumber.trim() != ""

    padding: FluidControls.Units.smallSpacing
    title: qsTr("Enter customer credentials")

    Column {
        anchors {
            left: parent.left
            right: parent.right
        }

        spacing: FluidControls.Units.largeSpacing

        FluidControls.SubheadingLabel {
            anchors {
                left: parent.left
                right: parent.right
            }

            text: qsTr("In order to keep track of this balance, we must know a little more about the current customer. Please fill out the information below.")
            wrapMode: Text.Wrap
        }

        Column {
            id: textFieldColumn
            anchors.horizontalCenter: parent.horizontalCenter

            Row {
                spacing: 12

                FluidControls.Icon {
                    anchors.verticalCenter: parent.verticalCenter
                    name: "social/person"
                    size: 20
                }

                RRUi.TextField {
                    id: customerNameField
                    focus: true
                    width: 300
                    placeholderText: qsTr("Customer name")
                    text: paymentCustomerDetailPage.customerName
                    onTextEdited: paymentCustomerDetailPage.customerName = text;
                }
            }

            Row {
                spacing: 12

                FluidControls.Icon {
                    anchors.verticalCenter: parent.verticalCenter
                    name: "communication/phone"
                    size: 20
                }

                RRUi.TextField {
                    id: phoneNumberField
                    width: 300
                    placeholderText: qsTr("Customer phone number")
                    text: paymentCustomerDetailPage.customerPhoneNumber
                    onTextEdited: paymentCustomerDetailPage.customerPhoneNumber = text;
                }
            }
        }
    }
}
