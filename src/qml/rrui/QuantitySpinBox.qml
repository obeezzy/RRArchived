import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.components 1.0 as RRComponents

Control {
    id: quantitySpinBox

    property real quantity: 1
    property real maximumQuantity: 0
    property string unit: qsTr("unit(s)")

    signal quantityUpdated(real newQuantity)

    implicitWidth: row.width + 16
    implicitHeight: 20

    background: Rectangle {
        radius: 2
        border {
            width: 2
            color: textField.activeFocus ? Material.accent : "darkgray"
        }
    }

    contentItem: FocusScope {
        Row {
            anchors.centerIn: parent
            id: row
            spacing: 4

            TextField {
                id: textField
                padding: 0
                topPadding: 15
                font.pixelSize: 14
                focus: true
                width: Math.max(contentWidth, 20)
                text: quantitySpinBox.quantity
                horizontalAlignment: Qt.AlignRight
                onEditingFinished: quantitySpinBox.quantityUpdated(text);

                background: null

                validator: RRComponents.DoubleValidator {
                    bottom: Math.min(1, quantitySpinBox.maximumQuantity)
                    top: quantitySpinBox.maximumQuantity
                    decimals: 4
                }
            }

            Label {
                id: label
                anchors.verticalCenter: parent.verticalCenter
                padding: 0
                font.pixelSize: 14
                width: Math.max(contentWidth, 20)
                text: quantitySpinBox.unit

                MouseArea {
                    anchors.fill: parent
                    onClicked: textField.forceActiveFocus();
                }
            }
        }
    }
}
