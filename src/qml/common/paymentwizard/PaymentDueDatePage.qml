import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import Fluid.Controls 1.0 as FluidControls

QQC2.Page {
    id: paymentDueDatePage
    objectName: "paymentDueDatePage"

    property date dueDate: new Date()

    padding: FluidControls.Units.smallSpacing
    title: qsTr("Choose due date")

    Column {
        padding: FluidControls.Units.smallSpacing

        anchors {
            left: parent.left
            right: parent.right
        }

        FluidControls.SubheadingLabel {
            anchors {
                left: parent.left
                right: parent.right
            }

            text: qsTr("Choose a date and time that you want the customer to pay.")
        }

        FluidControls.DateTimePicker {
            prefer24Hour: false
        }
    }
}
