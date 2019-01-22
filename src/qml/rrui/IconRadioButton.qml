import QtQuick 2.12
import QtQuick.Controls 2.12
import Fluid.Controls 1.0 as FluidControls

RadioButton {
    id: radioButton

    contentItem: Row {
        leftPadding: radioButton.indicator.width + radioButton.spacing + 8
        spacing: 8

        FluidControls.Icon {
            anchors.verticalCenter: parent.verticalCenter
            source: radioButton.icon.source
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
