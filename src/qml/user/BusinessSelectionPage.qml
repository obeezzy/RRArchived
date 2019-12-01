import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr 1.0 as RR
import "../rrui" as RRUi

RRUi.Page {
    id: businessSelectionPage
    objectName: "businessSelectionPage"

    readonly property int businessStoreIndex: listView.currentIndex
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

            text: qsTr("Below is a list of all the businesses associated with this account. Select the business you want Record Rack to work with.")
            wrapMode: Text.WordWrap
        }

        ListView {
            id: listView
            anchors {
                top: descriptionLabel.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                topMargin: 24
            }

            model: businessSelectionPage.userProfile.businessStoreModel
            currentIndex: 0

            delegate: QQC2.RadioButton {
                text: name
                checked: ListView.view.currentIndex === index
            }
        }
    }

    RRUi.ErrorDialog { }
}
