import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import "../../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import com.gecko.rr.components 1.0 as RRComponents
import ".."

RRUi.WizardPage {
    id: paymentCustomerDetailPage
    objectName: "paymentCustomerDetailPage"

    property RRModels.SaleCartModel cartModel: null
    property string customerName: ""
    property string customerPhoneNumber: ""
    readonly property bool detailValid: customerName.trim() != ""
                                        && customerPhoneNumber.trim() != ""
                                        && !hasNameNumberMismatch

    readonly property bool hasNameNumberMismatch: (phoneNumberField.count === 1
                                                   && (phoneNumberField.text.trim() === phoneNumberField.model.get(0).phone_number)
                                                   && (customerNameField.text.trim() !== phoneNumberField.model.get(0).preferred_name))

    padding: FluidControls.Units.smallSpacing
    title: qsTr("Enter customer credentials")
    hasNext: true
    nextEnabled: errorLabel.text.length === 0

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
                    source: FluidControls.Utils.iconUrl("social/person")
                    size: 20
                }

                RRUi.DropdownField {
                    id: customerNameField
                    focus: true
                    width: 300
                    placeholderText: qsTr("Customer name")
                    text: paymentCustomerDetailPage.customerName
                    model: RRModels.ClientModel {
                        filterText: paymentCustomerDetailPage.customerName
                        filterColumn: RRModels.ClientModel.PreferredNameColumn
                    }
                    textRole: "preferred_name"
                    subTextRole: "phone_number"
                    onTextEdited: paymentCustomerDetailPage.customerName = text;
                    onItemSelected: {
                        paymentCustomerDetailPage.customerName = modelData.preferred_name;
                        paymentCustomerDetailPage.customerPhoneNumber = modelData.phone_number;
                    }
                }
            }

            Row {
                spacing: 12

                FluidControls.Icon {
                    anchors.verticalCenter: parent.verticalCenter
                    source: FluidControls.Utils.iconUrl("communication/phone")
                    size: 20
                }

                RRUi.DropdownField {
                    id: phoneNumberField
                    width: 300
                    placeholderText: qsTr("Phone number")
                    text: paymentCustomerDetailPage.customerPhoneNumber
                    model: RRModels.ClientModel {
                        filterText: paymentCustomerDetailPage.customerPhoneNumber
                        filterColumn: RRModels.ClientModel.PhoneNumberColumn
                    }
                    validator: RRComponents.DoubleValidator { bottom: 0 }
                    textRole: "preferred_name"
                    subTextRole: "phone_number"
                    onTextEdited: paymentCustomerDetailPage.customerPhoneNumber = text;
                    onItemSelected: {
                        paymentCustomerDetailPage.customerName = modelData.preferred_name;
                        paymentCustomerDetailPage.customerPhoneNumber = modelData.phone_number;
                    }
                }
            }
        }
    }

    FluidControls.SubheadingLabel {
        id: errorLabel
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            bottomMargin: 24
        }

        text: {
            if (customerNameField.text.trim() === "")
                qsTr("Customer name cannot be empty.");
            else if (phoneNumberField.text.trim() === "")
                qsTr("Customer phone number cannot be empty.");
            else if (paymentCustomerDetailPage.hasNameNumberMismatch)
                qsTr("Name and phone number must match if the customer exists already.");
            else
                ""
        }
        color: "red"
        wrapMode: Text.Wrap
        horizontalAlignment: Qt.AlignHCenter
    }

    onNext: {
        paymentCustomerDetailPage.cartModel.customerName = paymentCustomerDetailPage.customerName;
        paymentCustomerDetailPage.cartModel.customerPhoneNumber = paymentCustomerDetailPage.customerPhoneNumber;

        if (paymentCustomerDetailPage.cartModel.balance !== 0) {
            paymentCustomerDetailPage.nextPage.component = Qt.resolvedUrl("PaymentDueDatePage.qml");
            paymentCustomerDetailPage.nextPage.properties = {
                "cartModel": paymentCustomerDetailPage.cartModel
            }
        } else {
            paymentCustomerDetailPage.nextPage.component = Qt.resolvedUrl("PaymentSummaryPage.qml");
            paymentCustomerDetailPage.nextPage.properties = {
                "customerName": paymentCustomerDetailPage.cartModel.customerName,
                "customerPhoneNumber": paymentCustomerDetailPage.cartModel.customerPhoneNumber,
                "totalCost": paymentCustomerDetailPage.cartModel.totalCost,
                "amountPaid": paymentCustomerDetailPage.cartModel.amountPaid,
                "paymentModel": paymentCustomerDetailPage.cartModel.paymentModel
            };
        }
    }
}
