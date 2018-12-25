import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr 1.0 as RR
import "../rrui" as RRUi

RRUi.Page {
    id: businessDetailPage
    objectName: "businessDetailPage"
    padding: 16
    topPadding: 36

    signal loggedIn

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
            }

            RRUi.IconTextField {
                icon.source: FluidControls.Utils.iconUrl("communication/location_on")
                textField.placeholderText: qsTr("Business address")
            }

            RRUi.IconTextField {
                icon.source: FluidControls.Utils.iconUrl("communication/phone")
                textField.placeholderText: qsTr("Business phone number")
            }

            RRUi.IconTextField {
                icon.source: FluidControls.Utils.iconUrl("action/date_range")
                textField.placeholderText: qsTr("Establishment year")
            }
        }
    }

    RRUi.BusyOverlay { visible: userProfile.busy }

    RRUi.ErrorDialog { }

    RR.UserProfile {
        id: userProfile
    }
}
