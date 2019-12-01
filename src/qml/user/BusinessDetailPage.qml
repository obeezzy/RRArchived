import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr 1.0 as RR
import "../rrui" as RRUi

RRUi.Page {
    id: businessDetailPage
    objectName: "businessDetailPage"

    property RR.UserProfile userProfile: null

    padding: 16
    topPadding: 36

    contentItem: FocusScope {
        focus: true

        FluidControls.SubheadingLabel {
            id: descriptionLabel
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            text: qsTr("Confirm the business details below. Note that this information would be displayed on all receipts (configurable through 'Settings').")
            wrapMode: Text.WordWrap
        }

        Column {
            anchors.centerIn: parent

            RRUi.IconTextField {
                icon.source: FluidControls.Utils.iconUrl("social/domain")
                textField.placeholderText: qsTr("Business name")
                textField.text: businessDetailPage.userProfile.businessDetails.name
                textField.onTextEdited: userProfile.businessDetails.name = text;
            }

            RRUi.IconTextField {
                icon.source: FluidControls.Utils.iconUrl("communication/location_on")
                textField.placeholderText: qsTr("Business address")
                textField.text: businessDetailPage.userProfile.businessDetails.address
                textField.onTextEdited: userProfile.businessDetails.address = text;
            }

            RRUi.IconTextField {
                icon.source: FluidControls.Utils.iconUrl("communication/phone")
                textField.placeholderText: qsTr("Business phone number")
                textField.text: businessDetailPage.userProfile.businessDetails.phoneNumber
                textField.onTextEdited: businessDetailPage.userProfile.businessDetails.phoneNumber = text;
            }

            RRUi.IconTextField {
                icon.source: FluidControls.Utils.iconUrl("action/date_range")
                textField.placeholderText: qsTr("Establishment year")
                textField.text: businessDetailPage.userProfile.businessDetails.establishmentYear
                textField.onTextEdited: businessDetailPage.userProfile.businessDetails.establishmentYear = text;
            }
        }
    }

    RRUi.ErrorDialog { id: errorDialog }

    Connections {
        target: businessDetailPage.userProfile
        onError: {
            switch (errorCode) {
            case RR.UserProfile.ConnectionRefusedError:
                errorDialog.show(qsTr("Unable to connect to the server. Check your internet connection and try again."), qsTr("Error"));
                break;
            default:
                errorDialog.show(qsTr("An unknown error occurred."), qsTr("Error"));
                break;
            }
        }
    }
}
