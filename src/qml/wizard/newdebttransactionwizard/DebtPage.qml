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
    id: debtPage
    objectName: "debtPage"

    readonly property real amountOwed: parseFloat(amountOwedTextField.text)
    //readonly property real amountPaid: parseFloat(amountPaidTextField.text)
    //readonly property real balance: Number(parseFloat(amountOwedTextField.text) - parseFloat(amountPaidTextField.text))
    //readonly property string balanceString: Number(balance).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))

    title: qsTr("Enter amount owed")
    focus: true
    nextButtonText: qsTr("Next")
    hasNext: true
    onNext: {
        debtPage.QQC2.StackView.view.wizard.amountOwed = debtPage.amountOwed;
        debtPage.nextPage.component = Qt.resolvedUrl("DueDatePage.qml");
    }

    Grid {
        anchors.centerIn: parent
        columns: 2
        rowSpacing: 16
        columnSpacing: 16

        FluidControls.SubheadingLabel {
            text: qsTr("Amount owed:")
            height: amountOwedTextField.height
            verticalAlignment: Qt.AlignBottom
            bottomPadding: 8
        }

        RRUi.TextField {
            id: amountOwedTextField
            text: "0.00"
            horizontalAlignment: Qt.AlignRight
            validator: RRComponents.DoubleValidator { bottom: 0 }
        }

//        FluidControls.SubheadingLabel {
//            text: qsTr("Amount paid:")
//            height: amountPaidTextField.height
//            verticalAlignment: Qt.AlignBottom
//            bottomPadding: 8
//        }

//        RRUi.TextField {
//            id: amountPaidTextField
//            text: "0.00"
//            horizontalAlignment: Qt.AlignRight
//            verticalAlignment: Qt.AlignVCenter
//            bottomPadding: 12
//            validator: RRComponents.DoubleValidator { bottom: 0 }
//        }

//        FluidControls.SubheadingLabel {
//            text: qsTr("Balance:")
//            height: amountPaidTextField.height
//            verticalAlignment: Qt.AlignBottom
//            bottomPadding: 8
//        }

//        FluidControls.SubheadingLabel {
//            text: debtPage.balanceString
//            width: amountOwedTextField.width
//            horizontalAlignment: Qt.AlignRight
//            height: amountPaidTextField.height
//            verticalAlignment: Qt.AlignBottom
//            bottomPadding: 8
//        }
    }
}
