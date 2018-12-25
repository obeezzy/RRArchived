import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls

QQC2.Page {
    id: paymentFinishPage
    objectName: "paymentFinishPage"

    property string errorMessage: ""

    title: qsTr("Processing payment")
    padding: FluidControls.Units.smallSpacing

    contentItem: FocusScope {
        FluidControls.SubheadingLabel {
            text: qsTr("Please wait while the information provided is being processed.");
        }

        FluidControls.SubheadingLabel {
            anchors.centerIn: parent
            visible: paymentFinishPage.errorMessage
            text: qsTr("Please wait while the information provided is being processed.");
        }

        QQC2.BusyIndicator {
            anchors.centerIn: parent
            running: true
            visible: !paymentFinishPage.errorMessage
        }
    }
}
