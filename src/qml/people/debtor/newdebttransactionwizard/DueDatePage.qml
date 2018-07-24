import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import com.gecko.rr.models 1.0 as RRModels
import com.gecko.rr.components 1.0 as RRComponents
import "../../../rrui" as RRUi
import "../../../common"

RRUi.WizardPage {
    id: dueDatePage
    objectName: "dueDatePage"

    property date dueDate: new Date()

    padding: FluidControls.Units.smallSpacing
    title: qsTr("Choose due date")

    Column {
        padding: FluidControls.Units.smallSpacing
        spacing: 4

        anchors {
            left: parent.left
            right: parent.right
        }

        FluidControls.SubheadingLabel {
            anchors {
                left: parent.left
                right: parent.right
            }

            text: qsTr("Choose a date and time that you want the debtor to pay.")
        }

        FluidControls.DateTimePicker {
            anchors.horizontalCenter: parent.horizontalCenter
            prefer24Hour: false
            selectedDateTime: dueDatePage.dueDate
            onSelectedDateTimeChanged: dueDatePage.dueDate = selectedDateTime;
        }
    }
}
