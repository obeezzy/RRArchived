import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../singletons"

FluidControls.FloatingActionButton {
    Material.background: Material.LightGreen
    Material.foreground: Material.theme === Material.Dark ? Stylesheet.black : Stylesheet.white
    display: QQC2.AbstractButton.IconOnly

    QQC2.ToolTip.visible: hovered
    QQC2.ToolTip.delay: 300
    QQC2.ToolTip.text: text
}
