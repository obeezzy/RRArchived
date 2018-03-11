import QtQuick 2.10
import QtQuick.Controls 2.2 as QQC2
import Fluid.Controls 1.0 as FluidControls

QQC2.Page {
    id: paymentMethodPage
    objectName: "paymentMethodPage"

    property string selectedOption: "cash"
    property bool acceptCash: true
    property bool acceptCard: true

    title: qsTr("Choose payment method")

    contentItem: FocusScope {
        QQC2.ButtonGroup { id: buttonGroup }

        Column {
            Repeater {
                anchors.fill: parent
                model: ListModel {
                    ListElement { iconName: "editor/attach_money"; option: "cash"; text: qsTr("Cash"); visible: true; checked: false }
                    ListElement { iconName: "action/credit_card"; option: "card"; text: qsTr("Debit/credit card"); visible: true; checked: false }
                    ListElement { iconName: "device/access_time"; option: "none"; text: qsTr("Pay later"); visible: true; checked: false }

                    Component.onCompleted: {
                        setProperty(0, "visible", paymentMethodPage.acceptCash);
                        setProperty(1, "visible", paymentMethodPage.acceptCard);
                        setProperty(paymentMethodPage.acceptCash ? 0 : 1, "checked", true);
                        paymentMethodPage.selectedOption = paymentMethodPage.acceptCash ? "cash" : "card";
                    }
                }

                delegate: QQC2.RadioButton {
                    id: radioButton
                    text: model.text
                    checked: model.checked
                    visible: model.visible
                    QQC2.ButtonGroup.group: buttonGroup

                    onClicked: paymentMethodPage.selectedOption = model.option;

                    contentItem: Row {
                        leftPadding: radioButton.indicator.width + radioButton.spacing + 8
                        spacing: 8

                        FluidControls.Icon {
                            anchors.verticalCenter: parent.verticalCenter
                            name: model.iconName
                            size: FluidControls.Units.iconSizes.medium
                        }

                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            text: radioButton.text
                            font: radioButton.font
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }
    }
}
