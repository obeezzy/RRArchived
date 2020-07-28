import "../../common"
import "../../rrui" as RRUi
import "../../singletons"
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import com.gecko.rr.components 1.0 as RRComponents
import com.gecko.rr.models 1.0 as RRModels

RRUi.WizardPage {
    id: dueDatePage

    property date dueDate: new Date()

    objectName: "dueDatePage"
    padding: FluidControls.Units.smallSpacing
    title: qsTr("Choose due date")
    nextButtonText: dueDatePage.QQC2.StackView.view && dueDatePage.QQC2.StackView.view.depth > 1 ? qsTr("Finish") : qsTr("Update")
    nextEnabled: !invalidDateLabel.visible
    hasNext: false
    onNext: {
        dueDatePage.QQC2.StackView.view.wizard.dueDate = dueDatePage.dueDate;
        dueDatePage.finish();
    }

    Column {
        padding: FluidControls.Units.smallSpacing
        spacing: 8

        anchors {
            left: parent.left
            right: parent.right
        }

        FluidControls.SubheadingLabel {
            text: qsTr("Choose a date and time that you want the debtor to pay.")

            anchors {
                left: parent.left
                right: parent.right
            }

        }

        FluidControls.DateTimePicker {
            anchors.horizontalCenter: parent.horizontalCenter
            prefer24Hour: false
            weekNumberVisible: false
            selectedDateTime: dueDatePage.dueDate
            onSelectedDateTimeChanged: dueDatePage.dueDate = selectedDateTime
        }

        FluidControls.SubheadingLabel {
            id: invalidDateLabel

            visible: {
                var today = new Date();
                return dueDatePage.dueDate.getTime() <= today.getTime();
            }
            text: qsTr("Due date must be set beyond today's date.")
            horizontalAlignment: Qt.AlignHCenter
            color: Stylesheet.errorRed

            anchors {
                left: parent.left
                right: parent.right
            }

        }

    }

}
