import QtQuick 2.10
import QtQuick.Controls 2.2 as QQC2
import Fluid.Controls 1.0 as FluidControls
import "../../singletons"

QQC2.Page {
    id: paymentBalancePage
    objectName: "paymentBalancePage"

    property string selectedOption: repeater.model.get(repeater.currentIndex).option
    property real totalCost: 0
    property real balance: 0
    property bool hasDebt: false
    property bool hasCredit: false
    property bool acceptAlternatePaymentMethod: false
    property bool isCashPayment: false

    title: qsTr("Handle %1").arg(hasCredit ? "credit" : "debt")
    padding: FluidControls.Units.smallSpacing

    contentItem: FocusScope {
        QQC2.ButtonGroup { id: buttonGroup }

        Column {
            anchors {
                left: parent.left
                right: parent.right
            }

            spacing: FluidControls.Units.smallSpacing

            FluidControls.SubheadingLabel {
                anchors {
                    left: parent.left
                    right: parent.right
                }

                text: {
                    if (paymentBalancePage.hasCredit)
                        return qsTr("You owe the customer <b>%1</b>. How would you like to handle this?").arg(Number(Math.abs(paymentBalancePage.balance)).toLocaleCurrencyString(Qt.locale(GlobalSettings.locale)));

                    return qsTr("The customer owes you <b>%1</b>. How would you like to handle this?").arg(Number(Math.abs(paymentBalancePage.balance)).toLocaleCurrencyString(Qt.locale(GlobalSettings.locale)));
                }

                wrapMode: Text.Wrap
            }

            Column {
                id: modelColumn
                anchors {
                    left: parent.left
                    right: parent.right
                }
                Repeater {
                    id: repeater
                    property int currentIndex: 0

                    model: hasCredit ? creditorModel : debtorModel

                    delegate: QQC2.RadioButton {
                        id: radioButton
                        width: modelColumn.width
                        text: model.text
                        checked: index === 0
                        visible: model.visible
                        QQC2.ButtonGroup.group: buttonGroup

                        contentItem: Text {
                            text: radioButton.text
                            font: radioButton.font
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: radioButton.indicator.width + radioButton.spacing
                            wrapMode: Text.Wrap
                        }

                        onClicked: repeater.currentIndex = index;
                    }
                }
            }
        }

        ListModel {
            id: debtorModel
            ListElement { option: "create_debtor"; text: qsTr("Add the customer to the list of people who owe me."); visible: true }
            ListElement { option: "pay_another_way"; text: qsTr("Pay the balance using a different method."); visible: false }
            ListElement { option: "overlook_balance"; text: qsTr("Overlook this balance. (Not recommended)"); visible: true}

            Component.onCompleted: setProperty(1, "visible", paymentBalancePage.acceptAlternatePaymentMethod);
        }

        ListModel {
            id: creditorModel
            ListElement { option: "give_change"; text: qsTr("Give the customer as change."); visible: true }
            ListElement { option: "create_creditor"; text: qsTr("Add the customer to the list of people I owe."); visible: true }
            ListElement { option: "overlook_balance"; text: qsTr("Overlook this balance. (Not recommended)"); visible: true }
        }
    }
}
