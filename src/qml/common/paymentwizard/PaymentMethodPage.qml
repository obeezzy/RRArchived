import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import Fluid.Controls 1.0 as FluidControls

QQC2.Page {
    id: paymentMethodPage
    objectName: "paymentMethodPage"

    property string selectedOption: selectedOption = paymentMethodPage.acceptCash ? "cash" : "card"
    property bool acceptCash: true
    property bool acceptCard: true

    title: qsTr("Choose payment method")

    contentItem: FocusScope {
        QQC2.ButtonGroup { id: buttonGroup }

        Column {
            Repeater {
                anchors.fill: parent
                model: PaymentOptionModel { }

                delegate: QQC2.RadioButton {
                    id: radioButton
                    checked: index === 0
                    text: model.text
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
