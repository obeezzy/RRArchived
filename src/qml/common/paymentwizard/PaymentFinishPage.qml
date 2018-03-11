import QtQuick 2.10
import QtQuick.Controls 2.2 as QQC2
import Fluid.Controls 1.0 as FluidControls

QQC2.Page {
    id: paymentFinishPage
    objectName: "paymentFinishPage"

    title: qsTr("Processing payment")
    padding: FluidControls.Units.smallSpacing

    contentItem: FocusScope {
        FluidControls.SubheadingLabel {
            text: qsTr("Please wait while the information provided is being processed.");
        }

        QQC2.BusyIndicator {
            anchors.centerIn: parent
            running: true
        }
    }
}
