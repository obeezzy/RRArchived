import ".."
import "../../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import com.gecko.rr.models 1.0 as RRModels

RRUi.WizardPage {
    id: paymentFinishPage

    property string errorMessage: ""
    property RRModels.SaleCartModel cartModel: null

    objectName: "paymentFinishPage"
    title: qsTr("Processing payment")
    padding: FluidControls.Units.smallSpacing
    hasNext: false

    Connections {
        target: paymentFinishPage.cartModel
        onError: {
            paymentFinishPage.errorMessage = "Oops! Something went wrong.";
        }
    }

    contentItem: FocusScope {
        FluidControls.SubheadingLabel {
            text: qsTr("Please wait while the information provided is being processed.")
        }

        FluidControls.SubheadingLabel {
            anchors.centerIn: parent
            visible: paymentFinishPage.errorMessage
            text: qsTr("Please wait while the information provided is being processed.")
        }

        QQC2.BusyIndicator {
            anchors.centerIn: parent
            running: true
            visible: !paymentFinishPage.errorMessage
        }

    }

}
