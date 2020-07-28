import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3

QQC2.RoundButton {
    id: arrowButton

    property string direction: "left"

    Material.background: "white"
    radius: FluidControls.Units.iconSizes.medium / 2
    icon.source: FluidControls.Utils.iconUrl("navigation/chevron_" + direction)
    visible: opacity != 0

    Behavior on opacity {
        NumberAnimation {
            easing.type: Easing.OutCubic
        }

    }

}
