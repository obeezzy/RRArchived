import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import com.gecko.rr.models 1.0 as RRModels
import com.gecko.rr.components 1.0 as RRComponents
import "../../rrui" as RRUi
import "../../common"
import "../../singletons"

RRUi.WizardPage {
    id: dueDatePage
    objectName: "dueDatePage"

    property date dueDate: new Date()

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
            anchors {
                left: parent.left
                right: parent.right
            }

            text: qsTr("Choose a date and time that you want the debtor to pay.")
        }

        FluidControls.DateTimePicker {
            anchors.horizontalCenter: parent.horizontalCenter
            prefer24Hour: false
            weekNumberVisible: false
            selectedDateTime: dueDatePage.dueDate
            onSelectedDateTimeChanged: dueDatePage.dueDate = selectedDateTime;
        }

        FluidControls.SubheadingLabel {
            id: invalidDateLabel
            anchors {
                left: parent.left
                right: parent.right
            }
            visible: {
                var date = new Date();
                return dueDatePage.dueDate.getDate() <= date.getDate();
            }

            text: qsTr("Due date must be set beyond today's date.")
            horizontalAlignment: Qt.AlignHCenter
            color: Stylesheet.errorRed
        }
    }
}
