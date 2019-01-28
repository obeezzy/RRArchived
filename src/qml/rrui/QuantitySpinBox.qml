import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.components 1.0 as RRComponents
import "../rrui" as RRUi

QQC2.Control {
    id: quantitySpinBox

    property real quantity: 1
    property real maximumQuantity: 0
    property string unit: qsTr("unit(s)")

    signal quantityUpdated(real newQuantity)

    implicitWidth: row.width + 16
    implicitHeight: 20

    background: Rectangle {
        color: Material.theme === Material.Dark ? Material.color(Material.Grey, Material.Shade800) : "white"
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

            RRUi.TextField {
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

            QQC2.Label {
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
