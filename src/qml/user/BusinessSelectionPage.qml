import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import com.gecko.rr 1.0 as RR

RRUi.Page {
    id: businessSelectionPage

    readonly property int businessStoreIndex: listView.currentIndex
    property RR.UserProfile userProfile: null

    objectName: "businessSelectionPage"
    padding: 16
    topPadding: 36

    RRUi.ErrorDialog {
    }

    contentItem: FocusScope {
        focus: true

        FluidControls.SubheadingLabel {
            id: descriptionLabel

            text: qsTr("Below is a list of all the businesses associated with this account. Select the business you want Record Rack to work with.")
            wrapMode: Text.WordWrap

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

        }

        ListView {
            id: listView

            model: businessSelectionPage.userProfile.businessStoreModel
            currentIndex: 0

            anchors {
                top: descriptionLabel.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                topMargin: 24
            }

            delegate: QQC2.RadioButton {
                text: name
                checked: ListView.view.currentIndex === index
            }

        }

    }

}
