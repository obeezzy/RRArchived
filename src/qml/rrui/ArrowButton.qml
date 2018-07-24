import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Controls.Material 2.3

QQC2.RoundButton {
    id: arrowButton

    property string direction: "left"

    Material.background: "white"
    radius: FluidControls.Units.iconSizes.medium / 2
    icon.source: FluidControls.Utils.iconUrl("navigation/chevron_" + direction)
    visible: opacity != 0

    Behavior on opacity { OpacityAnimator { } }
}
