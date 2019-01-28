import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls

QQC2.RadioButton {
    id: radioButton

    contentItem: Row {
        leftPadding: radioButton.indicator.width + radioButton.spacing + 8
        spacing: 8

        FluidControls.Icon {
            anchors.verticalCenter: parent.verticalCenter
            source: radioButton.icon.source
            size: FluidControls.Units.iconSizes.medium
        }

        FluidControls.BodyLabel {
            anchors.verticalCenter: parent.verticalCenter
            text: radioButton.text
            font: radioButton.font
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
